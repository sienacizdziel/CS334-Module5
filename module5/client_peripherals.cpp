#include "include/client_peripherals.h"

#include <Arduino.h>

#include <chrono>

#include "include/config.h"

namespace cs334::Client {

/**
 * @brief Construct a new Peripherals:: Peripherals object
 *
 * Initializes the ESP32 pins for reading / writing.
 */
Peripherals::Peripherals() {
  // ! TODO: set analogread, etc. like you normally would in setup() file

  // initialize pinmode for each photoresistor
  for (const auto &photoresistor : m_photoresistors) {
    pinMode(photoresistor.pin, INPUT);
  }

  // initialize LED pins
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  // initialize button pin
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // begin checking the button pin for press duration
  xTaskCreate(Peripherals::_checkButtonImpl,
              "button check client_peripherals ", 1024,
              &m_button_press_duration, 2, &m_button_check_handle);
}

/**
 * @brief Destroy the Peripherals:: Peripherals object
 *
 * Shuts down any long-running tasks, like the LED flashing.
 */
Peripherals::~Peripherals() {
  if (m_led_flash_handle != NULL) {
    vTaskDelete(m_led_flash_handle);
    m_led_flash_handle = NULL;
  }
  if (m_photoresistor_calibrate_handle != NULL) {
    vTaskDelete(m_photoresistor_calibrate_handle);
    m_photoresistor_calibrate_handle = NULL;
  }
  if (m_button_check_handle != NULL) {
    vTaskDelete(m_button_check_handle);
    m_button_check_handle = NULL;
  }
}

/* -------------------------------------------------------------------------- */
/*                               PERIPHERAL: LED                              */
/* -------------------------------------------------------------------------- */

/** [PERIPHERAL : LED]
 * @brief Sets the LED color.
 *
 * If flashRate is set to anything other than 0, a FreeRTOS task is created
 * (or recreated) to handle turning the LED on and off at a specified frequency.
 *
 * @param r [0-255] the R value
 * @param g [0-255] the G value
 * @param b [0-255] the B value
 * @param flashRate Time between flashes, in ms
 */
void Peripherals::setLED(uint8_t r, uint8_t g, uint8_t b, uint16_t flashRate) {
  // delete the previous LED flashing task, if it existed
  if (m_led_flash_handle != NULL) {
    vTaskDelete(m_led_flash_handle);
    m_led_flash_handle = NULL;
  }
  // if we don't want any flashing, just set the LED output
  if (!flashRate) {
    Peripherals::_setLEDImpl(r, g, b);
    // otherwise, begin the LED task flashing
  } else {
    // only takes 1 argument, so we need to pass in a struct
    led_flash_task_input_t taskInput{
        .r = r, .g = g, .b = b, .flashRate = flashRate, ._this = this};
    // begin task (static)
    xTaskCreate(Peripherals::_flashLEDImpl, "LED flash client_peripherals", 1024,
                &taskInput, 1, &m_led_flash_handle);
  }
}

/** [PERIPHERAL : LED]
 * @brief Flashes the LED on and off at flashRate in the background
 *
 * @param pvParameter
 */
void Peripherals::_flashLEDImpl(void *pvParameter) {
  led_flash_task_input_t *taskInput =
      static_cast<led_flash_task_input_t *>(pvParameter);
  bool on = false;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (true) {
    if (!on)
      Peripherals::_setLEDImpl(taskInput->r, taskInput->g, taskInput->b);
    else
      Peripherals::_setLEDImpl(0, 0, 0);
    on = !on;
    vTaskDelayUntil(&xLastWakeTime, taskInput->flashRate / portTICK_PERIOD_MS);
  }
}

/** [PERIPHERAL : LED]
 * @brief Sets the LED directly
 *
 * @param r [0-255] the R value
 * @param g [0-255] the G value
 * @param b [0-255] the B value
 */
void Peripherals::_setLEDImpl(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}

/* -------------------------------------------------------------------------- */
/*                             PERIPHERALS: BUTTON                            */
/* -------------------------------------------------------------------------- */

void Peripherals::_checkButtonImpl(void *pvParameter) {
  double *button_press_duration = static_cast<double *>(pvParameter);
  bool buttonPressed = false;
  std::chrono::time_point<std::chrono::system_clock> start;
  int buttonState;
  while (true) {
    buttonState = digitalRead(PIN_BUTTON);
    if (buttonState == LOW) {
      if (!buttonPressed) {  // on initial button press
        start = std::chrono::system_clock::now();
        buttonPressed = true;
      }
      *button_press_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() / 1000.f;
    } else {
      buttonPressed = false;
      *button_press_duration = 0.f;
    }
    delay(100);
  }
}

/* -------------------------------------------------------------------------- */
/*                         PERIPHERALS: Photoresistor                         */
/* -------------------------------------------------------------------------- */

/** [PERIPHERAL : Photoresistors]
 * @brief Gets the average 0-1 value across all photoresistors
 *
 * @return float [0-1] The current reading from the photoresistors
 */
float Peripherals::getPhotoresistorInput() {
  // sum up the inputs from all photoresistors, from 0-1
  float value = 0;
  for (const auto &photoresistor : m_photoresistors) {
    value += map(analogRead(photoresistor.pin), photoresistor.low,
                 photoresistor.high, 0, 100) /
             100.f;
  }
  // return the average photoresistor value
  return value / m_photoresistors.size();
}

/** [PERIPHERAL : Photoresistors]
 * @brief Begins photoresistor calibration
 *
 * Should be called when the initialization period begins.
 */
void Peripherals::beginPhotoresistorCalibration() {
  // clean up calibration task if previously existed
  endPhotoResistorCalibration();
  // begin task (static)
  xTaskCreate(Peripherals::_calibratePhotoresistorsImpl,
              "photoresistor calibration client_peripherals", 1024,
              &m_photoresistors, 1, &m_photoresistor_calibrate_handle);
}

/** [PERIPHERAL : Photoresistors]
 * @brief Ends photoresistor calibration state
 *
 * Should be called once the initialization period ends.
 */
void Peripherals::endPhotoResistorCalibration() {
  if (m_photoresistor_calibrate_handle != NULL) {
    vTaskDelete(m_photoresistor_calibrate_handle);
    m_photoresistor_calibrate_handle = NULL;
  }
}

/** [PERIPHERAL : Photoresistors]
 * @brief Static task for calibrating photoresistors
 *
 * Continually loops and updates low / high values for readings from
 * photoresistors.
 */
void Peripherals::_calibratePhotoresistorsImpl(void *pvParameter) {
  std::vector<photoresistor_t> *photoresistors =
      static_cast<std::vector<photoresistor_t> *>(pvParameter);
  // for each photoresistor, prepare for new values
  for (auto &photoresistor : *photoresistors) {
    photoresistor.low = INFINITY;
    photoresistor.high = -INFINITY;
  }
  while (true) {
    // for each photoresistor
    for (auto &photoresistor : *photoresistors) {
      // update its high/low values from the current reading value
      int value = analogRead(photoresistor.pin);
      photoresistor.low = min(photoresistor.low, value);
      photoresistor.high = max(photoresistor.high, value);
    }
    vTaskDelay(100);
  }
}

}  // namespace cs334::Client
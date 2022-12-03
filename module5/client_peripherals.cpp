#include "include/client_peripherals.h"

#include <Arduino.h>

#include <chrono>

#include "include/config.h"

namespace cs334::Client {

// variables set by the task functions
static std::chrono::time_point<std::chrono::system_clock> _button_press_start;
static bool _button_pressed = false;

// pre-declare the task functions
static void _flashLED();
static void _checkButton();
static void _photoresistorCalibrate(){};

/* -------------------------------------------------------------------------- */
/*                                    SETUP                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief Construct a new Peripherals:: Peripherals object
 *
 * Initializes the ESP32 pins for reading / writing.
 */
Peripherals::Peripherals() : m_taskLEDFlash(TASK_SECOND * 1, TASK_FOREVER, &_flashLED),
                             m_taskButtonCheck(TASK_SECOND * 0.1, TASK_FOREVER, &_checkButton),
                             m_taskPhotoresistorCalibrate(TASK_SECOND * 0.1, TASK_FOREVER, &_photoresistorCalibrate) {
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

  // set up the LED flash task, do not enable yet though
  m_scheduler.addTask(m_taskLEDFlash);
  m_scheduler.addTask(m_taskButtonCheck);
  m_scheduler.addTask(m_taskPhotoresistorCalibrate);
  // DO enable checking for button press duration
  m_taskButtonCheck.enable();
}

/**
 * @brief Destroy the Peripherals:: Peripherals object
 *
 * Shuts down any long-running tasks.
 */
Peripherals::~Peripherals() {
  m_taskLEDFlash.disable();
  m_taskButtonCheck.disable();
  m_taskPhotoresistorCalibrate.disable();
}

/* -------------------------------------------------------------------------- */
/*                               PERIPHERAL: LED                              */
/* -------------------------------------------------------------------------- */

// holders for input to the LED flash function
static struct led_flash_task_input_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  bool on;
} led_flash_task_input;

/** [PERIPHERAL : LED]
 * @brief The LED flashing task to be scheduled
 *
 * Run by m_ledScheduledFlashTask
 */
static void _flashLED() {
  if (led_flash_task_input.on) {
    analogWrite(PIN_LED_R, 0);
    analogWrite(PIN_LED_G, 0);
    analogWrite(PIN_LED_B, 0);
  } else {
    analogWrite(PIN_LED_R, led_flash_task_input.r);
    analogWrite(PIN_LED_G, led_flash_task_input.g);
    analogWrite(PIN_LED_B, led_flash_task_input.b);
  }
  led_flash_task_input.on = !led_flash_task_input.on;
}

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
  if (m_taskLEDFlash.isEnabled()) {
    m_taskLEDFlash.disable();
  }
  // if we don't want any flashing, just set the LED output
  if (!flashRate) {
    analogWrite(PIN_LED_R, r);
    analogWrite(PIN_LED_G, g);
    analogWrite(PIN_LED_B, b);
    // otherwise, begin the LED task flashing
  } else {
    led_flash_task_input.r = r;
    led_flash_task_input.g = g;
    led_flash_task_input.b = b;
    led_flash_task_input.on = false;
    m_taskLEDFlash.setInterval(flashRate);
    m_taskLEDFlash.enable();
  }
}

/* -------------------------------------------------------------------------- */
/*                             PERIPHERALS: BUTTON                            */
/* -------------------------------------------------------------------------- */

/**
 * @brief Updates button_press_start to keep track of the duration of a button press
 *
 * Sets _button_press_start and _button_pressed to keep track of whether or not
 * the button is currerntly pressed and if so when it started being pressed.
 */
static void _checkButton() {
  int buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == LOW) {
    if (!_button_pressed) {  // on initial button press
      _button_press_start = std::chrono::system_clock::now();
      _button_pressed = true;
    }
  } else {
    _button_press_start = std::chrono::system_clock::now();
    _button_pressed = false;
  }
}

/**
 * @brief Checks whether the button has been pressed for a specific amount of time
 *
 * If true, resets the button as if the button were unpressed –– so the same
 * button press cannot return true from this function more than once.
 *
 * @param duration Desired button press duration, in ms
 * @return true - If the button has been pressed for duration ms
 * @return false - otherwise
 */
bool Peripherals::checkButtonPressDuration(uint32_t duration) {
  // if it matches
  if (_button_pressed && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _button_press_start).count() >= duration) {
    // simulate the button being unpressed
    _button_press_start = std::chrono::system_clock::now();
    _button_pressed = false;
    return true;
  } else {
    return false;
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
uint32_t Peripherals::getPhotoresistorInput() {
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

// /** [PERIPHERAL : Photoresistors]
//  * @brief Begins photoresistor calibration
//  *
//  * Should be called when the initialization period begins.
//  */
// void Peripherals::beginPhotoresistorCalibration() {
//   // clean up calibration task if previously existed
//   endPhotoResistorCalibration();
//   // begin task (static)
//   xTaskCreate(Peripherals::_calibratePhotoresistorsImpl,
//               "photoresistor calibration client_peripherals", 1024,
//               &m_photoresistors, 1, &m_photoresistor_calibrate_handle);
// }

// /** [PERIPHERAL : Photoresistors]
//  * @brief Ends photoresistor calibration state
//  *
//  * Should be called once the initialization period ends.
//  */
// void Peripherals::endPhotoResistorCalibration() {
//   if (m_photoresistor_calibrate_handle != NULL) {
//     vTaskDelete(m_photoresistor_calibrate_handle);
//     m_photoresistor_calibrate_handle = NULL;
//   }
// }

// /** [PERIPHERAL : Photoresistors]
//  * @brief Static task for calibrating photoresistors
//  *
//  * Continually loops and updates low / high values for readings from
//  * photoresistors.
//  */
// void Peripherals::_calibratePhotoresistorsImpl(void *pvParameter) {
//   std::vector<photoresistor_t> *photoresistors =
//       static_cast<std::vector<photoresistor_t> *>(pvParameter);
//   // for each photoresistor, prepare for new values
//   for (auto &photoresistor : *photoresistors) {
//     photoresistor.low = INFINITY;
//     photoresistor.high = -INFINITY;
//   }
//   while (true) {
//     // for each photoresistor
//     for (auto &photoresistor : *photoresistors) {
//       // update its high/low values from the current reading value
//       int value = analogRead(photoresistor.pin);
//       photoresistor.low = min(photoresistor.low, value);
//       photoresistor.high = max(photoresistor.high, value);
//     }
//     vTaskDelay(100);
//   }
// }

}  // namespace cs334::Client
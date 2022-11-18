#include "include/client_peripherals.h"

namespace cs334::Client {

/**
 * @brief Construct a new Peripherals:: Peripherals object
 * 
 * Initializes the ESP32 pins for reading / writing.
 */
Peripherals::Peripherals() {
  // ! TODO: set analogread, etc. like you normally would in setup() file
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
void Peripherals::setLED(uint8_t r, uint8_t g, uint8_t b, uint16_t flashRate = 0) {
  // delete the previous LED flashing task, if it existed
  if (m_led_flash_handle) {
    vTaskDelete(m_led_flash_handle);
    m_led_flash_handle = NULL;
  }
  // if we don't want any flashing, just set the LED output
  if (!flashRate) {
    Peripherals::_setLED(r, g, b);
  // otherwise, begin the LED task flashing
  } else {
    // only takes 1 argument, so we need to pass in a struct
    led_flash_task_input_t taskInput{
      .r = r,
      .g = g,
      .b = b,
      .flashRate = flashRate,
      ._this = this
    };
    // begin task (static)
    xTaskCreate(
      Peripherals::_flashLEDImpl,
      "client_peripherals LED flash",
      4,
      &taskInput,
      1,
      &m_led_flash_handle
    );
  }
}

/**
 * @brief Flashes the LED on and off at flashRate in the background
 * 
 * @param pvParameter 
 */
void Peripherals::_flashLEDImpl(void *pvParameter) {
  led_flash_task_input_t* taskInput = static_cast<led_flash_task_input_t*>(pvParameter);
  while (true) {
    Peripherals::_setLED(taskInput->r, taskInput->g, taskInput->b);
    vTaskDelay(taskInput->flashRate);
    Peripherals::_setLED(0, 0, 0);
    vTaskDelay(taskInput->flashRate);
  }
}

/**
 * @brief Sets the LED directly
 * 
 * @param r [0-255] the R value
 * @param g [0-255] the G value
 * @param b [0-255] the B value
 */
void Peripherals::_setLED(uint8_t r, uint8_t g, uint8_t b) {
  // ! TODO: Fill out this function to write to the LED pins
}

/* -------------------------------------------------------------------------- */
/*                             PERIPHERALS: BUTTON                            */
/* -------------------------------------------------------------------------- */

uint16_t Peripherals::getButtonPressDuration() {
  // ! TODO: Fill out this function to get how long the button has been pressed for, in ms.
  // you may need to implement a FreeRTOS task queue to do this to keep track of the
  // button in the background. when the button has not been pressed, returns 0.
  return 0;
}

/* -------------------------------------------------------------------------- */
/*                         PERIPHERALS: Photoresistor                         */
/* -------------------------------------------------------------------------- */

float Peripherals::getPhotoresistorInput() {
  // ! TODO: Fill out this function to get total photoresistor input [0-1]
  return 0;
}


}
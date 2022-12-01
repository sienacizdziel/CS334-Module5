#ifndef CLIENT_PERIPHERALS_H
#define CLIENT_PERIPHERALS_H

#include <math.h>

#include <vector>

#include "config.h"
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"

namespace cs334::Client {

class Peripherals {
 public:  // METHODS
  Peripherals();
  ~Peripherals();

  /** [PERIPHERAL : LED]
   * @brief Sets the LED color.
   *
   * If flashRate is set to anything other than 0, a FreeRTOS task is created
   * (or recreated) to handle turning the LED on and off at a specified
   * frequency.
   *
   * @param r [0-255] the R value
   * @param g [0-255] the G value
   * @param b [0-255] the B value
   * @param flashRate Time of and between flashes, in ms
   */
  void setLED(uint8_t r, uint8_t g, uint8_t b, uint16_t flashRate = 0);

  /** [PERIPHERAL : Button]
   * @brief Get the button press duration, in ms
   *
   * @return 0 if button has not been pressed, otherwise duration of button
   * press in ms
   */
  double getButtonPressDuration();

  /** [PERIPHERAL : Photoresistor]
   * @brief Get the average photoresistor input value
   *
   * Reads in the readings from both photoresistors, maps them to their
   * corresponding values on a scale from [0-1], and averages them together.
   *
   * @return float [0-1] the average photoresistor reading, from 0 (darkness) to
   * 1 (full light)
   */
  float getPhotoresistorInput();

  /** [PERIPHERAL : Photoresistor]
   * @brief Begins photoresistor calibration task
   *
   * Continually checks the value inputted from the photoresistor. These values
   * are used to define a mapping range from pure darkness to bright light,
   * which are then used in the play state.
   */
  void beginPhotoresistorCalibration();

  /** [PERIPHERAL : Photoresistor]
   * @brief Ends photoresistor calibration
   *
   * Saves the photoresistor states so can be used later on for getting input
   * from each
   */
  void endPhotoResistorCalibration();

  /* --------------------------- PERIPHERAL: BUTTON --------------------------- */

  // publicly keep track of button press duration
  double m_button_press_duration = 0.f;

 protected:
  // Task created to flash the LED, if flashing is requested
  TaskHandle_t m_led_flash_handle = NULL;
  // Task created to calibrate the photoresistor
  TaskHandle_t m_photoresistor_calibrate_handle = NULL;
  // Task created to check the button state
  TaskHandle_t m_button_check_handle = NULL;

 private:
  /* ----------------------------- PERIPHERAL: LED ----------------------------
   */

  // Private struct used as input to _flashLEDImpl
  typedef struct led_flash_task_input {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint16_t flashRate;
    Peripherals *_this;
  } led_flash_task_input_t;

  // Flashes the LED value. Wrapped by setLED()
  static void _flashLEDImpl(void *pvParameter);

  // Sets the LED value to the pin directly.
  static void _setLEDImpl(uint8_t r, uint8_t g, uint8_t b);

  // Sets the button value to the pin directly.
  static void _checkButtonImpl(void *pvParameter);

  /* ------------------------ PERIPHERAL: Photoresistor -----------------------
   */

  // Private struct for managing photoresistors
  typedef struct photoresistor {
    int low;
    int high;
    uint8_t pin;
  } photoresistor_t;

  // keep a vector of all of the photoresistors
  //  :{ low, high, pin }
  std::vector<photoresistor_t> m_photoresistors{
      {0, 4096, PIN_PHOTORESISTOR_1},
      {0, 4096, PIN_PHOTORESISTOR_2},
  };

  // The task for calibrating the photoresistors
  static void _calibratePhotoresistorsImpl(void *pvParameter);
};

}  // namespace cs334::Client

#endif /* CLIENT_PERIPHERALS_H */

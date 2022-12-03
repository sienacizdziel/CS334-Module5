#ifndef CLIENT_PERIPHERALS_H
#define CLIENT_PERIPHERALS_H

#include <math.h>

#include <chrono>
#include <vector>

#include "config.h"
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "painlessMesh.h"

namespace cs334::Client {

class Peripherals {
 public:  // METHODS
  Peripherals();
  ~Peripherals();

  // updates the task scheduler
  void update() { m_scheduler.execute(); }

  /* ----------------------------- PERIPHERAL: LED ---------------------------- */

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

  /* ----------------------- PERIPHERAL: Photoresistors ----------------------- */

  /** [PERIPHERAL : Photoresistor]
   * @brief Get the average photoresistor input value
   *
   * Reads in the readings from both photoresistors, maps them to their
   * corresponding values on a scale from [0-1], and averages them together.
   *
   * @return uint32_t [0-100] the average photoresistor reading, from 0 (darkness) to
   * 100 (full light)
   */
  uint32_t getPhotoresistorInput();

  // /** [PERIPHERAL : Photoresistor]
  //  * @brief Begins photoresistor calibration task
  //  *
  //  * Continually checks the value inputted from the photoresistor. These values
  //  * are used to define a mapping range from pure darkness to bright light,
  //  * which are then used in the play state.
  //  */
  // void beginPhotoresistorCalibration();

  // /** [PERIPHERAL : Photoresistor]
  //  * @brief Ends photoresistor calibration
  //  *
  //  * Saves the photoresistor states so can be used later on for getting input
  //  * from each
  //  */
  // void endPhotoResistorCalibration();

  /* --------------------------- PERIPHERAL: BUTTON --------------------------- */

  /** [PERIPHERAL : Button]
   * @brief Get whether or not the button has been pressed for a duration
   *
   * If this returns true, it resets the button press counter (it assumes that
   * you are handling something that you only want to happen once per
   * button press duration).
   *
   * @return true iff the button has been pressed for that long
   */
  bool checkButtonPressDuration(uint32_t duration = 5000);

 private:
  /* ----------------------------- TASK SCHEDULING ---------------------------- */

  // Scheduler for peripheral tasks
  Scheduler m_scheduler;

  // Peripheral-specific tasks
  Task m_taskLEDFlash;
  Task m_taskButtonCheck;
  Task m_taskPhotoresistorCalibrate;

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
      {0, 4095, PIN_PHOTORESISTOR_1},
      {0, 4095, PIN_PHOTORESISTOR_2},
  };

  // The task for calibrating the photoresistors
  // static void _calibratePhotoresistorsImpl(void *pvParameter);
};

}  // namespace cs334::Client

#endif /* CLIENT_PERIPHERALS_H */

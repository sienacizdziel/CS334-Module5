## ScaledLightSensor

The sketch found in this directory (*ScaledLightSensor.ino*) contains an example that illustrates the usage of the photoresistor calibration code. The light sensor takes the form of a struct, which can be initialized by typing:

```Sensor sensor_name(pin_number);```

Three functions are then provided for working with the sensor:

```
sensor_setup(Sensor *sensor); // Sets the pinmode of the light sensor pin (should be called in arduino setup() function)

sensor_calibrate(Sensor *sensor); // Adjusts the upper and lower bounds used by the sensor by reading values on the pin, should be called continuously during calibration phase for all sensors

unsigned int sensor_read(Sensor *sensor); // Gets a reading from the sensor that's mapped based on upper and lower bounds to a value between 0-100 (with 0 being darkest, 100 lightest)
```

For instance, reading a value from a sensor might look a little like this:


```
Sensor sensor(33);
sensor_setup(&sensor);

for(10 secodns) {
    sensor_calibrate(&sensor);
}

Serial.println(sensor_read(&sensor));
```

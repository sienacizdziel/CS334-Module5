typedef struct Sensor {
  unsigned int pin;
  int low;
  int high;
  Sensor(int);
} Sensor;

Sensor::Sensor(int pin_num) : pin(pin_num), low(-1), high(-1) {};

unsigned int clamp(int value, int low, int high) {
  return max(min(value, high), low);
}

void calibrate_sensor(Sensor *sensor) {
  unsigned int value = analogRead(sensor->pin);
  if(sensor->low == -1) sensor->low = value;
  if(sensor->high == -1) sensor->high = value;
  if(value > sensor->high) sensor->high = value;
  else if(value < sensor->low) sensor->low = value;
}

unsigned int read_sensor(Sensor *sensor) {
  int low = sensor->low == -1 ? 0 : sensor->low;
  int high = sensor->high == -1 ? 4096 : sensor->high;
  
  unsigned int value = analogRead(sensor->pin);
  return clamp(map(value, low, high, 0, 100), 0, 100);
}

void setup_sensor(Sensor *sensor) {
  pinMode(sensor->pin, INPUT);  
}

Sensor sensor(33);

void setup() {
  Serial.begin(9600);
  setup_sensor(&sensor);
}

void loop() {
  if(millis() < 10000) {
    calibrate_sensor(&sensor);
    Serial.print(sensor.low);
    Serial.print(" ");
    Serial.println(sensor.high);
  } else {
    Serial.println(read_sensor(&sensor));
  }

  delay(10);
}

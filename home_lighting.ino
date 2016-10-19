bool hallway_motion = false;
const int hallway_led_1 = 3;
const int hallway_led_2 = 5;
const int hallway_motion_sensor = 15;

bool wardrobe_motion = false;
const int wardrobe_led_1 = 9;
const int wardrobe_led_2 = 10;
const int wardrobe_motion_sensor = 14;

const int bright_sensor = 2;
const int light_duration = 15000;

unsigned long hallway_start_time = 0;
unsigned long wardrobe_start_time = 0;

void setup() {
  Serial.begin(9600);

  pinMode(hallway_led_1, OUTPUT);
  pinMode(hallway_led_2, OUTPUT);
  pinMode(hallway_motion_sensor, INPUT);
  pinMode(bright_sensor, INPUT);
  pinMode(wardrobe_led_1, OUTPUT);
  pinMode(wardrobe_led_2, OUTPUT);
  pinMode(wardrobe_motion_sensor, INPUT);
}

void loop() {
  hallway_motion = portToBool(hallway_motion_sensor);
  wardrobe_motion = portToBool(wardrobe_motion_sensor);

  perform(hallway_motion, hallway_led_1, hallway_led_2, hallway_start_time);
  perform(wardrobe_motion, wardrobe_led_1, wardrobe_led_2, wardrobe_start_time);

  delay(500);
}

void perform(bool motion, int led_pin, int led_pin_1, unsigned long &start_time) {

  if (isBrightly())
  {
    //room lights are on -> just disable led
    Serial.println("no darkness -> disabling");
    checkAndDisable(led_pin, led_pin_1);
    return;
  }

  if (motion)
  {
    Serial.println("motion detected");
    //there is motion and darkness

    if (digitalRead(led_pin) == LOW)
    {
      Serial.println("led is off -> need to enable");
      //led is off - need to enable
      fadeIn(led_pin, led_pin_1);
    }

    start_time = millis(); //delay turning off the led
    Serial.print("start time: ");
    Serial.println(start_time);
  }
  else
  {
    Serial.println("no motion detected");

    //there is no motion and darkness
    //just check time and disable
    Serial.println("darkness");
    Serial.print("time remaining: ");
    Serial.println(getRemainingTime(start_time));
    if (getRemainingTime(start_time) <= 0)
    {
      Serial.println("time is out - disabling led");
      //time is out - need to disable
      checkAndDisable(led_pin, led_pin_1);
    }
  }
}

void checkAndDisable(int port, int port_1)
{
  if (digitalRead(port) == HIGH) //no need to fadeOut if port already disabled
  {
    fadeOut(port, port_1);
  }
}

bool portToBool(int port) {
  return analogRead(port) > 50 ? true : false;
}

void fadeIn(int port, int port_1) {
  Serial.println("fade in...");
  int brightness = 0;
  while (brightness <= 255) {
    analogWrite(port, brightness);
    analogWrite(port_1, brightness);
    brightness += 1;
    delay(10);
  }
}

void fadeOut(int port, int port_1) {
  Serial.println("fade out...");
  int brightness = 255;
  while (brightness >= 0) {
    analogWrite(port, brightness);
    analogWrite(port_1, brightness);
    brightness -= 1;
    delay(15);
  }
}

bool isBrightly()
{
  return digitalRead(bright_sensor) == LOW;
}

long getRemainingTime(long startTime)
{
  unsigned long remaining = (startTime + light_duration) - millis();
  if (remaining >= 0)
  {
    return remaining;
  }
  else
  {
    return 0;
  }
}

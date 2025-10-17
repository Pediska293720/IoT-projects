#include <Arduino.h>

#define PIN_CAR_RED 21
#define PIN_CAR_YELLOW 22
#define PIN_CAR_GREEN 23
#define PIN_PED_GREEN 16
#define PIN_PED_RED 4
#define PIN_BUTTON 14

#define RED_TIME 2500
#define YELLOW_CAR_TIME 2000

bool button_pressed = false;
int curr_state = 0;
unsigned long prev_millis = 0;
//0 - green for car, red for ped
//1 - yellow(after green) for car, red for ped
//2 - red for car, green for ped
//3 - yellow(after red) for car, red for ped

void SetGreenCarState() {
  digitalWrite(PIN_CAR_RED, LOW);
  digitalWrite(PIN_CAR_YELLOW, LOW);
  digitalWrite(PIN_CAR_GREEN, HIGH);
  digitalWrite(PIN_PED_RED, HIGH);
  digitalWrite(PIN_PED_GREEN, LOW);
}
void SetYellowCarState() {
  digitalWrite(PIN_CAR_RED, LOW);
  digitalWrite(PIN_CAR_YELLOW, HIGH);
  digitalWrite(PIN_CAR_GREEN, LOW);
  digitalWrite(PIN_PED_RED, HIGH);
  digitalWrite(PIN_PED_GREEN, LOW);
}
void SetGreenPedState() {
  digitalWrite(PIN_CAR_RED, HIGH);
  digitalWrite(PIN_CAR_YELLOW, LOW);
  digitalWrite(PIN_CAR_GREEN, LOW);
  digitalWrite(PIN_PED_RED, LOW);
  digitalWrite(PIN_PED_GREEN, HIGH);
}
void blink(int pin){
  for (int i = 0; i < 5; i++){
    digitalWrite(pin,HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_CAR_RED, OUTPUT);
  pinMode(PIN_CAR_YELLOW, OUTPUT);
  pinMode(PIN_CAR_GREEN, OUTPUT);
  pinMode(PIN_PED_GREEN, OUTPUT);
  pinMode(PIN_PED_RED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLDOWN);
}

void loop() {
  if (digitalRead(PIN_BUTTON) == HIGH && !button_pressed && curr_state == 0){
    delay(1000);
    button_pressed = true;
    curr_state = 1;
    prev_millis = millis();
  }

  switch (curr_state){
    case 0:
      SetGreenCarState();
      break;

    case 1:
      SetYellowCarState();
      if (millis() - prev_millis >= YELLOW_CAR_TIME){
        curr_state = 2;
        prev_millis = millis();
      }
      break;
    
    case 2:
      SetGreenPedState();
      if (millis() - prev_millis >= RED_TIME){
        blink(PIN_PED_GREEN);
        curr_state = 3;
        prev_millis = millis();
      }
      break;
    
    case 3:
      SetYellowCarState();
      if (millis() - prev_millis >= YELLOW_CAR_TIME){
        curr_state = 0;
        button_pressed = false;
        prev_millis = millis();
      }
      break;
  }

}

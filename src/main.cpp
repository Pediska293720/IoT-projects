#include <Arduino.h>

//for real microcontroller
#define BUTTON_RED_1 14
#define BUTTON_GREEN_2 27
#define BUTTON_BLUE_3 26
#define BUTTON_YELLOW_4 25

#define LED 2
#define RELAY 23
#define LDR 33

int PASSWORD[4] = {1, 2, 3, 4};
int INPUT_CODE[4];
int idx = 0;
bool message_print = false;

void blink(){
  for (int i = 0; i < 10; i++){
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
  }
}

enum State{
  OPEN,
  CLOSE,
  DAY
};
State state_door = CLOSE;

bool CheckingCode(){
  for (int i = 0; i < 4; i++){
    if(INPUT_CODE[i] != PASSWORD[i]){
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_RED_1, INPUT_PULLUP);
  pinMode(BUTTON_GREEN_2, INPUT_PULLUP);
  pinMode(BUTTON_BLUE_3, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW_4, INPUT_PULLUP);

  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  }

void loop() {
  int light_level = analogRead(LDR);
  if (light_level < 1000){
    state_door = DAY;
  }
  
  switch(state_door){
    case DAY:
      Serial.println("DAY.");
      digitalWrite(LED, LOW);
      digitalWrite(RELAY, LOW);
      delay(2000);
      state_door = CLOSE;
      message_print = false;
      break;

    case CLOSE:
      digitalWrite(RELAY, HIGH);
      digitalWrite(LED, HIGH);

      if (!message_print){
        Serial.println("Input code. Press button.");
        message_print = true;
      }

      if (digitalRead(BUTTON_RED_1) == 0){
        Serial.println("RED press");
        INPUT_CODE[idx] = 1;
        idx++;
        delay(500);
      }
  

      else if (digitalRead(BUTTON_GREEN_2) == 0){
        Serial.println("GREEN press");
        INPUT_CODE[idx] = 2;
        idx++;
        delay(500);
      }
      else if (digitalRead(BUTTON_BLUE_3) == 0){
        Serial.println("BLUE press");
        INPUT_CODE[idx] = 3;
        idx++;
        delay(500);
      }
      else if (digitalRead(BUTTON_YELLOW_4) == 0){
        Serial.println("YELLOW press");
        INPUT_CODE[idx] = 4;
        idx++;
        delay(500);
      }

      if (idx == 4){
        if (CheckingCode()){
          Serial.println("Correct code. Door open.");
          state_door = OPEN;
        }

        else{
          Serial.println("Incorrect code. Door close.");
        idx = 0;
        }
        message_print = false;
      }
      break;

    case OPEN:
      digitalWrite(RELAY, LOW);
      blink();
      digitalWrite(RELAY, HIGH);
      Serial.println("Door close.");
      state_door = CLOSE;

      idx = 0;
      break;
  }
}

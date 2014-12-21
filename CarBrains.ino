/*
  CarBrains - This is to control an RC car using the RX-2 Chip.
  See README.md for Wiring
  
  Author: Adam Freeman
*/
#define PIN_FORWARD 10
#define PIN_BACKWARD 11
#define PIN_LEFT 12
#define PIN_RIGHT 13

#define STOP 0
#define SLOW 50
#define HALF_SPEED 150
#define FULL_SPEED 250

int serial_part = 0;

int speed_y = 0; // 0 - 255
int dir_y = 0; // -1, 0, 1 for backwards, stationary, forwards

int speed_x = 0; // 0 - 255
int dir_x = 0; // -1, 0, 1 for left, center, right

void setup() {
  Serial.begin(9600);

  // Reset all the Chanels  
  pinMode(PIN_FORWARD, OUTPUT);
  pinMode(PIN_BACKWARD, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);

  analogWrite(PIN_FORWARD, STOP);
  analogWrite(PIN_BACKWARD, STOP);
  analogWrite(PIN_LEFT, STOP);
  analogWrite(PIN_RIGHT, STOP);
}


void loop() {
  // Nothing to see here
}

void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    switch (inChar) {
      case 'w':
        // Go Forward
        analogWrite(PIN_FORWARD, HALF_SPEED);
        analogWrite(PIN_BACKWARD, STOP);
        break;

      case 's':
        // Go Backward
        analogWrite(PIN_FORWARD, STOP);
        analogWrite(PIN_BACKWARD, HALF_SPEED);
        break;

      case 'a':
        // Go Forward
        analogWrite(PIN_LEFT, HALF_SPEED);
        analogWrite(PIN_RIGHT, STOP);
        break;

      case 'd':
        // Go Backward
        analogWrite(PIN_LEFT, STOP);
        analogWrite(PIN_RIGHT, HALF_SPEED);
        break;

      case 'f':
        // Stop
        analogWrite(PIN_FORWARD, STOP);
        analogWrite(PIN_BACKWARD, STOP);
        analogWrite(PIN_LEFT, STOP);
        analogWrite(PIN_RIGHT, STOP);
        break;
    }

    if (inChar == '\n') {
      serial_part = 0;
    } else {
      serial_part++;
    }
  }
}

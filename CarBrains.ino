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

#define DIRECTION_FORWARD  -1
#define DIRECTION_NEUTRAL   0
#define DIRECTION_BACKWARD  1

#define DIRECTION_LEFT     -1
#define DIRECTION_CENTER    0
#define DIRECTION_RIGHT     1

int serial_part = 0;

int speed_y = 0; // 0 - 255
int dir_y = 0; // -1, 0, 1 for forwards, neutral, backwards

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
  // Set y direction and speed.
  if(dir_y == DIRECTION_FORWARD) {
    // Forward 
    analogWrite(PIN_FORWARD, speed_y);
    analogWrite(PIN_BACKWARD, STOP);
  } 
  else if (dir_y == DIRECTION_BACKWARD) {
    // Backward
    analogWrite(PIN_FORWARD, STOP);
    analogWrite(PIN_BACKWARD, speed_y);    
  } 
  else {
    // Stopped
    analogWrite(PIN_FORWARD, STOP);
    analogWrite(PIN_BACKWARD, STOP);
  }

  // Set y direction and speed.
  if(dir_x == DIRECTION_LEFT) {
    // Forward 
    analogWrite(PIN_RIGHT, STOP);
    analogWrite(PIN_LEFT, speed_x);
  } 
  else if (dir_x == DIRECTION_RIGHT) {
    // Backward
    analogWrite(PIN_RIGHT, speed_x);
    analogWrite(PIN_LEFT, STOP);    
  } 
  else {
    // Stopped
    analogWrite(PIN_RIGHT, STOP);
    analogWrite(PIN_LEFT, STOP);
  }

  // Delay, may be too slow
  delay(300);  
}

void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    switch (inChar) {
    case 'w':
      // Go Forward
      speed_y = HALF_SPEED;
      dir_y = DIRECTION_FORWARD;

      speed_x = STOP;
      dir_x = DIRECTION_CENTER;
      break;

    case 's':
      // Go Backward
      speed_y = HALF_SPEED;
      dir_y = DIRECTION_BACKWARD;

      speed_x = STOP;
      dir_x = DIRECTION_CENTER;
      break;

    case 'a':
      // Go Forward
      speed_x = HALF_SPEED;
      dir_x = DIRECTION_LEFT;
      break;

    case 'd':
      // Go Backward
      speed_x = FULL_SPEED;
      dir_x = DIRECTION_RIGHT;
      break;

    case 'f':
      // Stop
      speed_y = STOP;
      speed_x = STOP;
      dir_x = DIRECTION_NEUTRAL;
      dir_x = DIRECTION_CENTER;
      break;

      // Change y speed         
    case 'q':
      // Increase y speed
      speed_y += 50;
      if(speed_y > 250) {
        speed_y = 250;  
      }
      break;

    case 'z':
      // Decrease y speed
      speed_y -= 50;
      if(speed_y < 0) {
        speed_y = 0;  
      }
      break;
    }
    if (inChar == '\n') {
      serial_part = 0;
    } 
    else {
      serial_part++;
    }
  }
}


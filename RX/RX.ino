/*
  CarBrains - This is to control an RC car using the RX-2 Chip.
 See README.md for Wiring
 
 Author: Adam Freeman
 */

/*
 * Defines for motor control
 */
#define PIN_FORWARD 10
#define PIN_BACKWARD 9
#define PIN_LEFT 6
#define PIN_RIGHT 5

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

int speed_y = 0; // 0 - 255
int dir_y = 0; // -1, 0, 1 for forwards, neutral, backwards

int speed_x = 0; // 0 - 255
int dir_x = 0; // -1, 0, 1 for left, center, right

/*
 *  RF Bits begin
 */
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(8,7);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {
  Serial.begin(115200);

  // Reset all the Chanels  
  pinMode(PIN_FORWARD, OUTPUT);
  pinMode(PIN_BACKWARD, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  pinMode(PIN_RIGHT, OUTPUT);

  analogWrite(PIN_FORWARD, STOP);
  analogWrite(PIN_BACKWARD, STOP);
  analogWrite(PIN_LEFT, STOP);
  analogWrite(PIN_RIGHT, STOP);


  /*
   *  Init Radio
   */  
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  // Start listening
  radio.startListening();

  Serial.println("Initialized.");

}


void loop() {
  // Set y direction and speed.
  if(dir_y == DIRECTION_FORWARD) {
    // Forward 
    analogWrite(PIN_BACKWARD, STOP);
    analogWrite(PIN_FORWARD, speed_y);
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
    analogWrite(PIN_LEFT, FULL_SPEED);
  } 
  else if (dir_x == DIRECTION_RIGHT) {
    // Backward
    analogWrite(PIN_LEFT, STOP);    
    analogWrite(PIN_RIGHT, speed_x);
  } 
  else {
    // Stopped
    analogWrite(PIN_RIGHT, STOP);
    analogWrite(PIN_LEFT, STOP);
  }

  // Delay, may be too slow
  delay(20); 
 
  // if there is data ready
  if ( radio.available() ) {
    // Dump the payloads until we've gotten everything
    unsigned long got_time;
    char buff[32];

    bool done = false;
    while (!done) {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &buff, sizeof(buff) );
      processControlChar(buff[0]);
      Serial.print("We got : ");
      Serial.println(buff);

      delay(20);
    }
    
  }  
}

void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    processControlChar(inChar);

  }
}

void processControlChar(char inChar) {
  Serial.println(inChar);
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
      
      Serial.println("Going back");
      
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
}

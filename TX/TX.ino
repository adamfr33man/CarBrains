/*
  CarBrains - This is to control an RC car using the RX-2 Chip.
 See README.md for Wiring
 
 Author: Adam Freeman
 */

/*
 *  PS2 Controller stuff
 */
#include <PS2X_lib.h>  //for v1.6

#define PS2_DAT        2
#define PS2_CLK        3 
#define PS2_CMD        4
#define PS2_ATTN       5

PS2X ps2x; // create PS2 Controller Class
int  ps2_error = 0;
byte ps2_type = 0;

boolean ps2_stickmode = true;

/*
 * Defines for motor control
 */
int serial_part;
char command;

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

  /*
   *  Init Radio
   */
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  // Start listening
  radio.startListening();

  Serial.println("Initialized.");

  // Set PS2 Controller
  delay(20);

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  ps2_error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATTN, PS2_DAT, false, false);

  switch (ps2_error) {
  case 0:
    Serial.println("Found Controller, configured successful.");
    break;

  case 1:
    Serial.println("No controller found, check wiring");
    break;

  case 2:
    Serial.println("Controller found but not accepting commands.");
    break;

  case 3:
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    break;
  }
}


void loop() {
  char lastCommand = command;
  // Read the PS2 Controller
  if(ps2_error != 1) {
    readController();
  }

  if(command != lastCommand) {
    Serial.println("Radio sending " + command);
    sendChar(command);
  } 
  else {
    //Serial.println("Command still " + command);
  }
  // Delay, may be too slow
  delay(100);
  //Serial.println("Loop");  
}

void readController() {
  ps2x.read_gamepad(false, false); 
  //Serial.println("Reading controller");
  // Read the sticks if enabled.
  if(ps2_stickmode) {

    int stick_y = ps2x.Analog(PSS_LY);
    int stick_x = ps2x.Analog(PSS_RX);

    if(stick_y == 130) {
      command = 'f';
    } 
    else if(stick_y < 130) {
      /*
      speed_y = ((130 - stick_y) * 2);
       if(speed_y > 255) {
       speed_y = 255;  
       } else if(speed_y < SLOW) {
       speed_y = SLOW;
       };*/
      command = 'w';
      Serial.println("Forward");    
    } 
    else {
      /*
      speed_y = stick_y;
       dir_y = DIRECTION_BACKWARD;
       */
      Serial.println("Back");

      command = 's';    
    }
    Serial.println("X: " + stick_x);

    if(stick_x < 100) {
      //speed_x = FULL_SPEED;
      //dir_x = DIRECTION_LEFT;
      Serial.println("Left");
      command = 'a';      
    } 
    else if(stick_x > 140) {
      //speed_x = FULL_SPEED;
      //dir_x = DIRECTION_RIGHT; 
      Serial.println("Right");   
      command = 'd';      
    } else {
      //speed_x = STOP;
      //dir_x = DIRECTION_CENTER;
      // this happens above command = 'f';      
    } 

  }
}

void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    sendChar(inChar);
  }
}

void sendChar(char inChar) {
  char buff[10];
  buff[0] = inChar;
  buff[1] = '\n';

  Serial.println("Processing Serial command" + inChar);
  // Send the data
  radio.stopListening();
  bool ok = radio.write(&buff, sizeof(buff));
  if (ok) {
    printf("ok...");
  } 
  else {
    printf("failed.\n\r");
  }
  // Now, continue list
}





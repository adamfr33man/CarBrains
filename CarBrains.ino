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

boolean ps2_stickmode = false;

/*
 * Defines for motor control
 */
#define PIN_FORWARD 5
#define PIN_BACKWARD 4
#define PIN_LEFT 3
#define PIN_RIGHT 2

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
  // Read the PS2 Controller
  if(ps2_error != 1) {
    readController();
  }
  
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
    analogWrite(PIN_LEFT, speed_x);
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
}

void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    processControlChar(inChar);

    if (inChar == '\n') {
      serial_part = 0;
    } 
    else {
      serial_part++;
    }
  }
}

void processControlChar(char inChar) {
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
}

void readController() {
  ps2x.read_gamepad(false, false); 
  
  if(ps2x.Button(PSB_PAD_UP)) {
    processControlChar('w');
  }
  
  if(ps2x.Button(PSB_PAD_DOWN)) {
    processControlChar('s');
  }

  if(ps2x.Button(PSB_PAD_LEFT)) {
    processControlChar('a');
  }

  if(ps2x.Button(PSB_PAD_RIGHT)) {
    processControlChar('d');
  }

  if(ps2x.Button(PSB_L1)) {
    processControlChar('z');    
  }

  if(ps2x.Button(PSB_R1)) {
    processControlChar('q');    
  }

  if(ps2x.ButtonReleased(PSB_SQUARE)) {
    processControlChar('f');    
  }

  if(ps2x.ButtonReleased(PSB_TRIANGLE)) {
    ps2_stickmode = !ps2_stickmode;
    Serial.print("Stick mode: ");
    Serial.println(ps2_stickmode);    
  }
  
  // Read the sticks if enabled.
  if(ps2_stickmode) {
    int stick_y = ps2x.Analog(PSS_LY);
    int stick_x = ps2x.Analog(PSS_RX);
    
   if(stick_y == 130) {
      speed_y = STOP;
      dir_y = DIRECTION_NEUTRAL;      
    } else if(stick_y < 130) {
      speed_y = ((130 - stick_y) * 2);
      if(speed_y > 255) {
        speed_y = 255;  
      } else if(speed_y < SLOW) {
        speed_y = SLOW;
      };
      dir_y = DIRECTION_FORWARD;      
      Serial.println("Forward");    
    } else {
      speed_y = stick_y;
      dir_y = DIRECTION_BACKWARD;
      Serial.println("Back");    
    }
    
    if(stick_x == 128) {
      speed_x = STOP;
      dir_x = DIRECTION_CENTER;      
    } else if(stick_x < 130) {
      speed_x = FULL_SPEED;
      dir_x = DIRECTION_LEFT;
      Serial.println("Left");          
    } else {
      speed_x = FULL_SPEED;
      dir_x = DIRECTION_RIGHT; 
      Serial.println("Right");   
    }
    
    Serial.print("(");
    Serial.print(stick_x);
    Serial.print(", ");
    Serial.print(stick_y);
    Serial.println(")");
  }
}

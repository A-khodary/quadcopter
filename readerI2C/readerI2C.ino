// Arduino sketch for sending PWM sampling via I2C
// Quadcopter project ENSEA
// October 2014
// Nicolas de Maubeuge, all rights reserved


#include <Wire.h>

// Some hardware defines :

# define RUDDER_PIN 5
# define THROTTLE_PIN 2
# define AILERONS_PIN 3
# define ELEVATOR_PIN 4
# define AUX1_PIN 5
# define AUX2_PIN 6
# define AUX3_PIN 7
# define AUX4_PIN 8
# define AUX5_PIN 9

// Sampling global variables :

unsigned long PWM_duration[9];
byte* PWM_pointer;

// Handling function :


void i2cHandler()
{
  Wire.write(PWM_pointer, 9*sizeof(unsigned long));
}

void setup()

{
  
 pinMode(RUDDER_PIN, INPUT);
 pinMode(THROTTLE_PIN, INPUT);
 pinMode(AILERONS_PIN, INPUT);
 pinMode(ELEVATOR_PIN, INPUT);
 pinMode(AUX1_PIN, INPUT);
 pinMode(AUX2_PIN, INPUT);
 pinMode(AUX3_PIN, INPUT);
 pinMode(AUX4_PIN, INPUT);
 pinMode(AUX5_PIN, INPUT);
 
 PWM_duration[0]=0;
 PWM_duration[1]=0;
 PWM_duration[2]=0;
 PWM_duration[3]=0;
 PWM_duration[4]=0;
 PWM_duration[5]=0;
 PWM_duration[6]=0;
 PWM_duration[7]=0;
 PWM_duration[8]=0;
 
 Wire.begin(1);
 Wire.onRequest(i2cHandler);
 PWM_pointer = (byte*) &PWM_duration;
 
 //Serial.begin(9600);
}



void loop()
{
  PWM_duration[0]=pulseIn(RUDDER_PIN, HIGH);
  PWM_duration[1]=pulseIn(THROTTLE_PIN, HIGH);
  PWM_duration[2]=pulseIn(AILERONS_PIN, HIGH);
  PWM_duration[3]=pulseIn(ELEVATOR_PIN, HIGH);
  PWM_duration[4]=pulseIn(AUX1_PIN, HIGH);
  PWM_duration[5]=pulseIn(AUX2_PIN, HIGH);
  PWM_duration[6]=pulseIn(AUX3_PIN, HIGH);
  PWM_duration[7]=pulseIn(AUX4_PIN, HIGH);
  PWM_duration[8]=pulseIn(AUX5_PIN, HIGH);
  
  PWM_duration[0] = PWM_duration[0]*1000/1303; 
  PWM_duration[1] = PWM_duration[1]*1000/1303;
  PWM_duration[2] = PWM_duration[2]*1000/1303;
  PWM_duration[3] = PWM_duration[3]*1000/1303;
  PWM_duration[4] = PWM_duration[4]*1000/1303;
  PWM_duration[5] = PWM_duration[5]*1000/1303;
  PWM_duration[6] = PWM_duration[6]*1000/1303;
  PWM_duration[7] = PWM_duration[7]*1000/1303;
  PWM_duration[8] = PWM_duration[8]*1000/1303;
  
  // Uncomment for debugging output :
  
  // Serial.print("RUDDER :");
  //Serial.print(PWM_duration[0]);
  //Serial.print("\n");
  
    // Serial.print("THROTTLE:");
  //Serial.print(PWM_duration[1]);
  //Serial.print("\n");
  
    // Serial.print("AILERONS :");
  //Serial.print(PWM_duration[2]);
  //Serial.print("\n");
  
    // Serial.print("ELEVATOR :");
  //Serial.print(PWM_duration[3]);
  //Serial.print("\n");
  
    // Serial.print("AUX1 :");
  //Serial.print(PWM_duration[4]);
  //Serial.print("\n");
  
    // Serial.print("AUX2 :");
  //Serial.print(PWM_duration[5]);
  //Serial.print("\n");
  
    // Serial.print("AUX3 :");
  //Serial.print(PWM_duration[6]);
  //Serial.print("\n");
  
    // Serial.print("AUX4 :");
  //Serial.print(PWM_duration[7]);
  //Serial.print("\n");
  
    // Serial.print("AUX5 :");
  //Serial.print(PWM_duration[8]);
  //Serial.print("\n");
  
  // Use to restrain consumption or adding ultrasonic sensor
  // delay(500);
}

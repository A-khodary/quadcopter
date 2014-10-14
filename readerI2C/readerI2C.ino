// Arduino sketch for sending PWM sampling via I2C
// Quadcopter project ENSEA
// October 2014
// Nicolas de Maubeuge, all rights reserved


#include <Wire.h>

// Some hardware defines :

# define RUDDER_PIN 7
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
unsigned long value;

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
 
 Serial.begin(9600);
 
 
}



void loop()
{
  value=pulseIn(RUDDER_PIN, HIGH);
  //PWM_duration[1]=pulseIn(THROTTLE_PIN, HIGH);
  //PWM_duration[2]=pulseIn(AILERONS_PIN, HIGH);
  //PWM_duration[3]=pulseIn(ELEVATOR_PIN, HIGH);
  //PWM_duration[4]=pulseIn(AUX1_PIN, HIGH);
  //PWM_duration[5]=pulseIn(AUX2_PIN, HIGH);
  //PWM_duration[6]=pulseIn(AUX3_PIN, HIGH);
  //PWM_duration[7]=pulseIn(AUX4_PIN, HIGH);
  //PWM_duration[7]=pulseIn(AUX5_PIN, HIGH);
  
  Serial.print(value);
  Serial.print("\n");
  delay(500);
}

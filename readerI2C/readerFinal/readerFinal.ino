#include <Event.h>
#include <Timer.h>

// Arduino sketch for PWM sampling, Ultrasonic Telemetry, Ublox GPS Management, voltage and current Measurement and in the future RGB Leds
// Quadcopter project ENSEA
// October 2014
// Nicolas de Maubeuge, all rights reserved

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Timing management macros :

//Frequencies in Hertz :


#define PWM_FREQ 20
#define GPS_FREQ 1
#define ULTRASONIC_FREQ 15
#define VOLTAGE_CURRENT_FREQ 1

int PWMcounter=0;
int GPScounter=0;
int ULTRASONICcounter=0;
int VOLTAGECURRENTcounter=0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Timing management Global variables :

Timer t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  PWM Macros

# define RUDDER_PIN 28
# define THROTTLE_PIN 26
# define AIL_PIN 24
# define ELEV_PIN 22
# define AUX1_PIN 30
# define AUX2_PIN 32
# define AUX3_PIN 34
# define AUX4_PIN 36

#define PWM_TIMEOUT 20000

#define PWM_MAX 1890
#define PWM_MIN 1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  PWM Global variables :

float pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Ultrasonic Macros

#define MAXIMUM_RANGE 500
#define MINIMUM_RANGE 4
#define ULTRASONIC_TIMEOUT 30000

#define trigPin 43
#define echoPin 45

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Voltage and current Macros :

#define CURRENT_PIN 2
#define VOLTAGE_PIN 3

#define AMP_COEFF 20.5530 // Amp / Volt 
#define VOLT_COEFF 10.125635 //4 Volt battery / Volt measured

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Voltage and current global variables :

float voltage=0;
float current=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  Ultrasonic Global variables :

float duration, distance;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GPS Configuration AREA :

byte  gpsConfigsSent;  // number of cfg msgs sent
byte  gpsConfigTimer;  // 0 = no more work, 1 = send now, >1 wait

static const unsigned char UBX_5HZ[] = {0xb5,0x62,0x06,0x08,0x06,0x00,0xc8,0x00,0x01,0x00,0x01,0x00,0xde,0x6a};
const unsigned long gpsBaudRates[] = { 9600L, 19200L, 38400L, 57600L, 115200L};
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GPS Macros
enum {
  GPS_INVALID_ACCURACY = 0xFFFFFFFF, 
  GPS_INVALID_AGE = 0xFFFFFFFF, 
  GPS_INVALID_ANGLE = 0x7FFFFFFF, 
  GPS_INVALID_ALTITUDE = 2147483647,//999999999, 
  GPS_INVALID_DATE = 0,
  GPS_INVALID_TIME = 0xFFFFFFFF, 
  GPS_INVALID_SPEED = 999999999, 
  GPS_INVALID_FIX_TIME = 0xFFFFFFFF
};

enum { 
    GPS_DETECTING = 0, 
    GPS_NOFIX = 1,
    GPS_FIX2D = 2,
    GPS_FIX3D = 3,
    GPS_FIX3DD = 4 // differential fix 
};
#define UBLOX_5HZ   {UBX_5HZ,sizeof(UBX_5HZ)}
#define UBLOX_38400 {(unsigned char *)"$PUBX,41,1,0003,0003,38400,0*24\r\n",0}
#define UBLOX_CONFIGS UBLOX_5HZ,UBLOX_38400
#define GPS_MAXIDLE_DETECTING 200 // 2 seconds at 100Hz
#define GPS_MAXIDLE 500           // 5 seconds at 100Hz

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GPS Structures
struct GeodeticPosition {
  long latitude;
  long longitude;
  long altitude;
};
struct gpsData {
    int32_t  lat,lon;  // position as degrees (*10E7)
    int32_t  course;   // degrees (*10E5)
    uint32_t speed;    // cm/s
    int32_t  height;   // mm (from ellipsoid)
    uint32_t accuracy; // mm
    uint32_t fixage;   // fix 
    uint32_t fixtime;  // fix 
    uint32_t sentences; // sentences/packets processed from gps (just statistics)
    uint8_t  state;    // gps state
    uint8_t  sats;     // number of satellites active
    uint8_t  baudrate; // current baudrate (index) - used by autodetection
    uint8_t  type;     // current type - used by autodetection
    uint32_t idlecount; // how many times gpsUpdate has been called without getting a valid message
};

struct gpsConfigEntry {
  const unsigned char *data;
  const unsigned char len;
};

struct gpsData gpsData; // This is accessed by the parser functions directly !
struct gpsConfigEntry gpsConfigEntries[] = {UBLOX_CONFIGS};
GeodeticPosition currentPosition;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GPS Functions :
void gpsSendConfig() {

  if (gpsConfigEntries[gpsConfigsSent].data) {
    if (gpsConfigEntries[gpsConfigsSent].len) {
      for (int i=0; i<gpsConfigEntries[gpsConfigsSent].len; i++) {
        Serial2.write(gpsConfigEntries[gpsConfigsSent].data[i]);
      }
      gpsConfigTimer=gpsConfigEntries[gpsConfigsSent].len;
    }
    else {
      Serial2.print((char*)gpsConfigEntries[gpsConfigsSent].data);
      gpsConfigTimer=strlen((char*)gpsConfigEntries[gpsConfigsSent].data);
    }
   
   if (gpsConfigTimer<10) {
      gpsConfigTimer=10;
    }
    gpsConfigsSent++;
  }

}

void updateGps() {
  noInterrupts();
  
  gpsData.idlecount++;
   if(gpsData.idlecount == 200)    {
     //Serial.println("NO data");
     gpsData.idlecount = 0;
   }
  while (Serial2.available()) {
    unsigned char c = Serial2.read();
    int ret=0;

    if (gpsData.state == GPS_DETECTING) {
        ret = ubloxProcessData(c);
        if (ret) {
          // found GPS device start sending configuration
          gpsConfigsSent = 0;
          gpsConfigTimer = 1;
          break;
        }
      
    }
    else {
      // Normal operation just execute the detected parser
      ret = ubloxProcessData(c);
    }

  if (gpsConfigTimer) {
    if (gpsConfigTimer==1) {
      gpsSendConfig();
    }
    gpsConfigTimer--;
  }
    // Upon a successfully parsed sentence, zero the idlecounter and update position data
    if (ret) {
      if (gpsData.state == GPS_DETECTING) {
         gpsData.state = GPS_NOFIX; // make sure to lose detecting state (state may not have been updated by parser)
      }
      gpsData.idlecount=0;
      currentPosition.latitude=gpsData.lat;
      currentPosition.longitude=gpsData.lon;
      currentPosition.altitude=gpsData.height;
      //Serial.print("Latitutde = ");Serial.print(currentPosition.latitude);Serial.print(" Longitude = ");Serial.print(currentPosition.longitude);Serial.print(" Altitude = ");Serial.println(currentPosition.altitude);
    }
  }

  // Schedule confg sending if needed
  if (gpsConfigTimer) {
    if (gpsConfigTimer==1) {
      gpsSendConfig();
    }
    gpsConfigTimer--;
  }
  interrupts();
  }

void initializeGpsData() {

  gpsData.lat = GPS_INVALID_ANGLE;
  gpsData.lon = GPS_INVALID_ANGLE;
  gpsData.course = GPS_INVALID_ANGLE;
  gpsData.speed = GPS_INVALID_SPEED;
  gpsData.height = GPS_INVALID_ALTITUDE;
  gpsData.accuracy = GPS_INVALID_ACCURACY;
  gpsData.fixage = GPS_INVALID_AGE;
  gpsData.state = GPS_DETECTING;
  gpsData.sentences = 0;
  gpsData.sats = 0;
  gpsData.fixtime = 0xFFFFFFFF;
}

void initializeGps() {

    gpsData.baudrate = 2;
    Serial2.begin(gpsBaudRates[gpsData.baudrate]);
    ubloxInit();
    initializeGpsData();
 }

////////////////////////////////////////////////////////////////////////////////////////////////////
struct ublox_NAV_STATUS { // 01 03 (16)
  uint32_t iTow;
  uint8_t  gpsFix;
  uint8_t  flags;
  uint8_t  fixStat;
  uint8_t  flags2;
  uint32_t ttfx;
  uint32_t msss;
};

struct ublox_NAV_POSLLH { // 01 02 (28)
  uint32_t iTow;
  int32_t lon; // 1e-7 degrees
  int32_t lat; // 1e-7 degrees
  int32_t height; // mm
  int32_t hMSL; // mm
  uint32_t hAcc; //mm
  uint32_t vAcc; //mm
};

struct ublox_NAV_SOL { // 01 6 (52)
  uint32_t iTow;
  int32_t  fTow;
  int16_t  week;
  uint8_t  gspFix;
  uint8_t  flags;
  int32_t  ecefX;
  int32_t  ecefY;
  int32_t  ecefZ;
  int32_t  pAcc;
  int32_t  ecefVX;
  int32_t  ecefVY;
  int32_t  ecefVZ;
  int32_t  sAcc;
  uint16_t pDOP;
  uint8_t  res1;
  uint8_t  numSV;
  uint32_t res2;
};

struct ublox_NAV_VELNED { // 01 12h (36)
  uint32_t iTow;
  int32_t  velN; // cm/s
  int32_t  velE; // cm/s
  int32_t  velD; // cm/s
  uint32_t  speed; // cm/s
  uint32_t  gSpeed; // cm/s
  int32_t  heading; // dev 1e-5
  uint32_t sAcc; // cm/s
  uint32_t cAcc; // deg 1e-5
};

unsigned short ubloxClass,ubloxId;
unsigned char  ubloxCKA,ubloxCKB;
unsigned short ubloxExpectedDataLength;
unsigned short ubloxDataLength;

union ublox_message {
  struct ublox_NAV_STATUS nav_status;
  struct ublox_NAV_POSLLH nav_posllh;
  struct ublox_NAV_VELNED nav_velned;
  struct ublox_NAV_SOL nav_sol;
  unsigned char raw[52];
} ubloxMessage;

/////////////////////////////////////////////////////////////////////////////////////////////////////
enum ubloxState{ WAIT_SYNC1, WAIT_SYNC2, GET_CLASS, GET_ID,
GET_LL, GET_LH, GET_DATA, GET_CKA, GET_CKB  } ubloxProcessDataState;

void ubloxInit() {
  
  ubloxProcessDataState = WAIT_SYNC1;
}

void ubloxParseData() {// uses publib vars

  gpsData.sentences++;
  if (ubloxClass==1) { // NAV
    if (ubloxId==2) { // NAV:POSLLH
      gpsData.lat = ubloxMessage.nav_posllh.lat;
      gpsData.lon = ubloxMessage.nav_posllh.lon;
      gpsData.height = ubloxMessage.nav_posllh.height;
      gpsData.accuracy = ubloxMessage.nav_posllh.hAcc;
      //Serial.print(" Accuracy: ");Serial.print(gpsData.accuracy);
      gpsData.fixtime = ubloxMessage.nav_posllh.iTow;
    }
    else if (ubloxId==3) { //NAV:STATUS
      switch (ubloxMessage.nav_status.gpsFix) {
        case 2: 
          gpsData.state = GPS_FIX2D;
          //Serial.print(" 2D Fix ");
          break;
		  
        case 3:
          gpsData.state = GPS_FIX3D;
          //Serial.print(" 3D Fix ");
          break;
		  
        default:
          gpsData.state = GPS_NOFIX;
          //Serial.print(" No Fix ");
          break;
      }
    }
    else if (ubloxId==6) { // NAV:SOL
      gpsData.sats = ubloxMessage.nav_sol.numSV;
      //Serial.print(" NO. os Sats: "); Serial.print(gpsData.sats);
    }
    else if (ubloxId==18) { // NAV:VELNED
      gpsData.course = ubloxMessage.nav_velned.heading / 100; // 10E-5 to millidegrees
      gpsData.speed = ubloxMessage.nav_velned.gSpeed;
    }
  } 
}

int ubloxProcessData(unsigned char data) {
  
  int parsed = 0;
  
  switch (ubloxProcessDataState) {
  case WAIT_SYNC1:
    if (data == 0xb5) {
      ubloxProcessDataState = WAIT_SYNC2;
    }
    break;
	
  case WAIT_SYNC2:
    if (data == 0x62) {
      ubloxProcessDataState = GET_CLASS;
    }
    else if (data == 0xb5) {
      // ubloxProcessDataState = GET_SYNC2;
    }
    else {
      ubloxProcessDataState = WAIT_SYNC1;
    }
    break;
  case GET_CLASS:
    ubloxClass=data;
    ubloxCKA=data;
    ubloxCKB=data;
    ubloxProcessDataState = GET_ID;
    break;
	
  case GET_ID:
    ubloxId=data;
    ubloxCKA += data;
    ubloxCKB += ubloxCKA;
    ubloxProcessDataState = GET_LL;
    break;
	
  case GET_LL:
    ubloxExpectedDataLength = data;
    ubloxCKA += data;
    ubloxCKB += ubloxCKA;
    ubloxProcessDataState = GET_LH;
    break;
	
  case GET_LH:
    ubloxExpectedDataLength += data << 8;
    ubloxDataLength=0;
    ubloxCKA += data;
    ubloxCKB += ubloxCKA;
    if (ubloxExpectedDataLength <= sizeof(ubloxMessage)) {
      ubloxProcessDataState = GET_DATA;
    }
    else {
      // discard overlong message
      ubloxProcessDataState = WAIT_SYNC1;
    }
    break;
	
  case GET_DATA:
    ubloxCKA += data;
    ubloxCKB += ubloxCKA;
    // next will discard data if it exceeds our biggest known msg
    if (ubloxDataLength < sizeof(ubloxMessage)) {
      ubloxMessage.raw[ubloxDataLength++] = data;
    }
    if (ubloxDataLength >= ubloxExpectedDataLength) {
      ubloxProcessDataState = GET_CKA;
    }
    break;
	
  case GET_CKA:
    if (ubloxCKA != data) {
      ubloxProcessDataState = WAIT_SYNC1;
    } 
	else {
      ubloxProcessDataState = GET_CKB;
    }
    break;
	
  case GET_CKB:
    if (ubloxCKB == data) {
      parsed = 1;
      ubloxParseData();
    }
    ubloxProcessDataState = WAIT_SYNC1;
    break;
	
  }
  return parsed;
}

void sendGps()
{
  noInterrupts();
  
if (gpsData.state == GPS_NOFIX)
{
  Serial.print("status=nofix_"); 
}
else
{
  Serial.print("status=fix_");
  Serial.print("latitude="); Serial.print(currentPosition.latitude); Serial.print("_");
  Serial.print("longitude="); Serial.print(currentPosition.longitude); Serial.print("_");
  Serial.print("altitude="); Serial.print(currentPosition.altitude); Serial.print("_");
  Serial.print("sat="); Serial.print(gpsData.sats); Serial.print("_");
}
  
  
  interrupts();
}



////////////////////////////////////////////////////////////////////////////////////////// PWM functions :

void updatePWM()
{
  noInterrupts();
  
  pwm1=pulseIn(AIL_PIN, HIGH, PWM_TIMEOUT);
  pwm2=pulseIn(ELEV_PIN, HIGH, PWM_TIMEOUT);
  pwm3=pulseIn(THROTTLE_PIN, HIGH, PWM_TIMEOUT);
  pwm4=pulseIn(RUDDER_PIN, HIGH, PWM_TIMEOUT);
  pwm5=pulseIn(AUX1_PIN, HIGH, PWM_TIMEOUT);
  pwm6=pulseIn(AUX2_PIN, HIGH, PWM_TIMEOUT);
  pwm7=pulseIn(AUX3_PIN, HIGH, PWM_TIMEOUT);
  pwm8=pulseIn(AUX4_PIN, HIGH, PWM_TIMEOUT);

  if (pwm1 != 0) pwm1 = (pwm1 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm1 = -1;
  
  if (pwm2 != 0) pwm2 = (pwm2 - PWM_MIN ) / ( PWM_MAX - PWM_MIN );
  else pwm2 = -1;
  
  if (pwm3 != 0) pwm3 = (pwm3 - PWM_MIN ) / ( PWM_MAX - PWM_MIN );
  else pwm3 = -1; 
  
  if (pwm4 != 0) pwm4 = (pwm4 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm4 = -1;
  
  if (pwm5 != 0) pwm5 = (pwm5 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm5 = -1;
  
  if (pwm6 != 0) pwm6 = (pwm6 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm6 = -1;
  
  if (pwm7 != 0) pwm7 = (pwm7 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm8 = -1;
  
  if (pwm8 != 0) pwm8 = (pwm8 - PWM_MIN ) / ( PWM_MAX - PWM_MIN ); 
  else pwm8 = -1;


  interrupts();
}

void initializePWM()
{
  noInterrupts();
  
   pinMode(RUDDER_PIN, INPUT);
   pinMode(THROTTLE_PIN, INPUT);
   pinMode(AIL_PIN, INPUT);
   pinMode(ELEV_PIN, INPUT);
   pinMode(AUX1_PIN, INPUT);
   pinMode(AUX2_PIN, INPUT);
   pinMode(AUX3_PIN, INPUT);
   pinMode(AUX4_PIN, INPUT);
  
  pwm1 = 0.0;
  pwm2 = 0.0;
  pwm3 = 0.0;
  pwm4 = 0.0;
  pwm5 = 0.0;
  pwm6 = 0.0;
  pwm7 = 0.0;
  pwm8 = 0.0;
  
  interrupts();
}

void sendPWM()
{
  noInterrupts();
  
  Serial.print("pwm1="); Serial.print(pwm1); Serial.print("_");
  Serial.print("pwm2="); Serial.print(pwm2); Serial.print("_");
  Serial.print("pwm3="); Serial.print(pwm3); Serial.print("_");
  Serial.print("pwm4="); Serial.print(pwm4); Serial.print("_");
  Serial.print("pwm5="); Serial.print(pwm5); Serial.print("_");
  Serial.print("pwm6="); Serial.print(pwm6); Serial.print("_");
  Serial.print("pwm7="); Serial.print(pwm7); Serial.print("_");
  Serial.print("pwm8="); Serial.print(pwm8); Serial.print("_");
  
  interrupts();
}

////////////////////////////////////////////////////////////////////////////////////////// Ultrasonic functions :

void initializeUltrasonic()
{
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   
   distance = 0;
   duration = 0;
}

void updateUltrasonic()
{
  noInterrupts();
  
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
  
   digitalWrite(trigPin, LOW);
   duration = pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT);
  
   // Calculate the distance (in cm) based on the speed of sound.
   distance = duration / 58.2;
  
   if (distance >= MAXIMUM_RANGE || distance <= MINIMUM_RANGE)
   {
     distance = -1.0;
   }
   
   interrupts();
}

void sendUltrasonic()
{
  noInterrupts();
  
  Serial.print("ultradist="); Serial.print(distance); Serial.print("_");
  
  
  interrupts();
}

////////////////////////////////////////////////////////////////////////////////////////// Voltage and current functions :

void updateVoltCurrent()
{
  noInterrupts();
  
  voltage = VOLT_COEFF * 5.0 * 1/1024 * analogRead(VOLTAGE_PIN);
  current = AMP_COEFF * 5.0 * 1/1024 * analogRead(CURRENT_PIN);
  
  interrupts();
}

void sendVoltCurrent()
{
  noInterrupts();
  
  Serial.print("voltage="); Serial.print(voltage); Serial.print("_");
  Serial.print("current="); Serial.print(current); Serial.print("_");
  
  interrupts();
}

////////////////////////////////////////////////////////////////////////////////////////// Timing management functions :
void timingIRQ()
{
  noInterrupts();
  
  if (PWMcounter >= 1000 / PWM_FREQ)
  {
    updatePWM();
    sendPWM();
    PWMcounter = 1;
  }
  else PWMcounter ++;
  
   if (ULTRASONICcounter >= 1000 / ULTRASONIC_FREQ)
  {
    updateUltrasonic();
    sendUltrasonic();
    ULTRASONICcounter = 1;
  }
  else ULTRASONICcounter ++;
  
    if (GPScounter >= 1000 / GPS_FREQ)
  {
    updateGps();
    sendGps();
    GPScounter = 1;
  }
  else GPScounter ++;
  
    if (VOLTAGECURRENTcounter >= 1000 / VOLTAGE_CURRENT_FREQ)
  {
    updateVoltCurrent();
    sendVoltCurrent();
    VOLTAGECURRENTcounter = 1;
  }
  else VOLTAGECURRENTcounter ++;

interrupts();
}


////////////////////////////////////////////////////////////////////////////////////////// Arduino Setup and Loop :

void setup()  {
Serial.begin(115200);
// GPS init :
initializeGps();

// PWM init :
initializePWM();

//Ultrasonic init:
initializeUltrasonic();

//Timing management init :
t.every(1, timingIRQ);

}

void loop()  {
 t.update();
}

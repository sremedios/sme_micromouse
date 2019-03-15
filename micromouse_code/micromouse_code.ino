// Sharp IR GP2Y0A41SK0F Distance Test
// http://tinkcore.com/sharp-ir-gp2y0a41-skf/
/*
#define sensor_0 A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_1 A1 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_2 A2 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_3 A3 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
*/

// D9-12 is motors
#define motor_L_0 9
#define motor_L_1 10
#define motor_R_0 11
#define motor_R_1 12


#include <SoftwareSerial.h>

//#define rxPin 3  // pin 3 connects to smcSerial TX  (not used in this example)
//#define txPin 4  // pin 4 connects to smcSerial RX
SoftwareSerial smcSerial = SoftwareSerial(motor_L_0, motor_L_1);
 
// required to allow motors to move
// must be called when controller restarts and after any error
void exitSafeStart()
{
  smcSerial.write(0x83);
}
 
// speed should be a number from -3200 to 3200
void setMotorSpeed(int speed)
{
  if (speed < 0)
  {
    smcSerial.write(0x86);  // motor reverse command
    speed = -speed;  // make speed positive
  }
  else
  {
    smcSerial.write(0x85);  // motor forward command
  }
  smcSerial.write(speed & 0x1F);
  smcSerial.write(speed >> 5);
}
 

void setup() {
  /*
  Serial.begin(9600); // start the serial port
  */

  // initialize software serial object with baud rate of 19.2 kbps
  smcSerial.begin(19200);
 
  // the Simple Motor Controller must be running for at least 1 ms
  // before we try to send serial data, so we delay here for 5 ms
  delay(5);
 
  // if the Simple Motor Controller has automatic baud detection
  // enabled, we first need to send it the byte 0xAA (170 in decimal)
  // so that it can learn the baud rate
  smcSerial.write(0xAA);  // send baud-indicator byte
 
  // next we need to send the Exit Safe Start command, which
  // clears the safe-start violation and lets the motor run
  exitSafeStart();  // clear the safe-start violation and let the motor run

  
}

void loop() {

  setMotorSpeed(1600);  // half-speed forward
  delay(1000);
  setMotorSpeed(-1600);  // half-speed reverse
  delay(1000);

  /*
  // 5v
  float volts_0 = analogRead(sensor_0)*0.0048828125;  // value from sensor * (5/1024)
  int distance_0 = 13*pow(volts_0, -1); // worked out from datasheet graph
  delay(10); // slow down serial port 

  float volts_1 = analogRead(sensor_1)*0.0048828125;  // value from sensor * (5/1024)
  int distance_1 = 13*pow(volts_1, -1); // worked out from datasheet graph
  delay(10); // slow down serial port 

  float volts_2 = analogRead(sensor_2)*0.0048828125;  // value from sensor * (5/1024)
  int distance_2 = 13*pow(volts_2, -1); // worked out from datasheet graph
  delay(10); // slow down serial port 

  float volts_3 = analogRead(sensor_3)*0.0048828125;  // value from sensor * (5/1024)
  int distance_3 = 13*pow(volts_3, -1); // worked out from datasheet graph
  delay(10); // slow down serial port 
  
  if (distance_0 <= 30){

    Serial.print("sensor_0\t");
    Serial.print(distance_0);
    Serial.print("\t sensor_1\t");
    Serial.print(distance_1);
    Serial.print("\t sensor_2\t");
    Serial.print(distance_2);
    Serial.print("\t sensor_3\t");
    Serial.print(distance_3);
    Serial.print("\n");
  }
  */
}

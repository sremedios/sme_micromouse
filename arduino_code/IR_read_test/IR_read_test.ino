// Sharp IR GP2Y0A41SK0F Distance Test
// http://tinkcore.com/sharp-ir-gp2y0a41-skf/

#define sensor_0 A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_1 A1 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_2 A2 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensor_3 A3 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {
  
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
}

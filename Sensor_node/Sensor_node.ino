#include <SoftwareSerial.h>
#include <basicMPU6050.h>

// Configure the SoftwareSerial pins
SoftwareSerial sim800l(3, 2); // RX, TX
#define THINGSPEAK_API_KEY ""
basicMPU6050<> imu;

void setup() {
  delay(15000);
  imu.setup();
  //imu.setBias();
  pinMode(12,OUTPUT);

  Serial.begin(9600);
  sim800l.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  // Wait for the SIM800L module to initialize
  delay(2000);


  // Set the APN (Access Point Name) for your cellular network provider
  sendATCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  sendATCommand("AT+SAPBR=3,1,\"APN\",\"internet.fido.ca\"");
  sendATCommand("AT+SAPBR=1,1");
  delay(1000);

  // Open an HTTP connection
  sendATCommand("AT+HTTPINIT");
  delay(1000);
  sendATCommand("AT+HTTPPARA=\"CID\",1");
  delay(1000);

  // Close the HTTP connection and terminate the HTTP session
  //  sendATCommand("AT+HTTPTERM");
  //
  //  delay(2000); // Wait for 5 seconds before sending the next request
  //  sendATCommand("AT+SAPBR=0,1");
  //  delay(2000); // Wait for 5 seconds before sending the next request
  //  sendATCommand("AT+CGATT=0");
}
float th = 0.13;
void loop() {
  // Update gyro calibration
  digitalWrite(12,HIGH);
  delay(200);
  digitalWrite(12,LOW);
  delay(2000);
  imu.updateBias();

  //-- Scaled and calibrated output:
  // Accel
  Serial.print( imu.ax() );
  Serial.print( " " );
  Serial.print( imu.ay() );
  Serial.print( " " );
  Serial.print( imu.az() );
  Serial.print( "    " );

  Serial.println(analogRead(A0));
  delay(1000);
  float humidity = analogRead(A0);
  humidity = map(humidity, 200, 1011, 1, 10);
  humidity=10-humidity;
  Serial.println(humidity);
  float magnitude = imu.ax() * imu.ax() + imu.ay() * imu.ay();// + imu.az() * imu.az();
  float value = sqrt(magnitude);
  float x = (abs(value - th)) * 100;
  //  if (value < 0)
  //    value = -1 * value;
  //  value *= 100;
  //  int s = map(value, 0, 100, 0, 10);
  int s=0;
  int y = x;
  Serial.print(" mag without loop: ");
  Serial.println(y);
  //Serial.println(s);
  if (y > 1)
  {
    Serial.println("Earthquake aya! Bhagooooo");
    s = y;
  }
  else
    s = 0;
  delay(100);
  // Set the ThingSpeak server URL and the GET request parameters
  String url = "AT+HTTPPARA=\"URL\",\"api.thingspeak.com/update?api_key=";
  url += String(THINGSPEAK_API_KEY);
  url += "&field1=";
  url += String(humidity);
  //url += "\"";
  url += "&field2=";
  url += String(s);
  url += "\"";

  sendATCommand(url);

  // Send the GET request
  delay(2000);
  sendATCommand("AT+HTTPACTION=0");
  delay(2000);
  // Read the response
  sendATCommand("AT+HTTPREAD");
  delay(10000);

}

void sendATCommand(String command) {
  sim800l.println(command);
  delay(1000); // Wait for the response
  while (sim800l.available()) {
    Serial.write(sim800l.read());
  }
}

#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h" 

int buzzer = 7 ;
int redLed = 11 ;
int greenLed = 10 ;
int smokeSensor = A0;
int flame_sensor = A5 ;
int flame_detected ;
int sensorThreshHold = 50;


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio&apos;s status

// Specify IP address or hostname
char server[] = "firealarmsensor.herokuapp.com";

WiFiClient client;

void setup() {

  // Initialize serial and wait for port to open:

  Serial.begin(9600);
  pinMode(buzzer, OUTPUT) ;
  pinMode(greenLed, OUTPUT) ;
  pinMode(redLed, OUTPUT) ;
  pinMode(flame_sensor, INPUT) ;
  pinMode(smokeSensor, INPUT) ;


  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don&apos;t continue

    while (true);

  }

  // attempt to connect to WiFi network:

  if ( status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WPA SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network:

    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:

    delay(5000);
  }

  Serial.println("You&apos;re connected to the network");
}

void loop() {
  int analogSensor = analogRead(smokeSensor);
  int flame_detected = analogRead(flame_sensor) ;

  Serial.println(flame_detected);
  Serial.println(analogSensor);

  if (analogSensor > 1000)
  {
   if ( status != WL_CONNECTED) {
 
      status = WiFi.begin(ssid, pass);
  
    }
    Serial.print("Somke!");
    Serial.println(analogSensor);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 5000, 3000);
    String smokeInt = String(analogSensor);
    
    sendData("Riaydh", "2Invent Company 4480 Al Imam Saud Ibn Faysal Rd, As Sahafah, Riaydh 13321 6833", smokeInt, "+966546367561", "There is a gas leak!!!", "test@gmail.com");

  }
  else if (flame_detected > 600)
  {
  
    Serial.println("Flame detected...! take action immediately.");
    tone(buzzer, 5000, 3000);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
     if ( status != WL_CONNECTED) {

    status = WiFi.begin(ssid, pass);

    }
    String temp = String(flame_detected);
    sendData("Riaydh", "2Invent Company 4480 Al Imam Saud Ibn Faysal Rd, As Sahafah, Riaydh 13321 6833", temp, "+966546367561", "There is a Fire!!!", "test@gmail.com");
    sendData("Riaydh", "2Invent Company 4480 Al Imam Saud Ibn Faysal Rd, As Sahafah, Riaydh 13321 6833", temp, "+966546367508", "There is a is a Fire!!!", "test@gmail.com");
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(greenLed, HIGH);
    noTone(buzzer);
    digitalWrite(redLed, LOW);
  }
  delay(5000);
}


void sendData(String city, String address, String temp, String phone_number, String message, String email) {
  String jsonData = "{\"temperature\" : \"" + temp + "\" , \"Address\" : \"" + address + "\" , \"city\" : \"" + city + "\" , \"phone_number\" : \"" + phone_number + "\" , \"email\" : \"" + email + "\" , \"message_type\" : \"" + message + "\"}";

  if (client.connect(server, 80)) {
    client.println("POST /api/alarms/ HTTP/1.1");
    client.println("Host: firealarmsensor.herokuapp.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());
    client.println();
    client.print(jsonData);
  }

  if (client.connected()) {
    client.stop();
  }
  Serial.println(jsonData);
}

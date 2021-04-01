

#include <HTTPClient.h>

#include <WiFi.h>

#include "DHTesp.h"
#define DHTpin 15    //D15 of ESP32 DevKit
#define ledPin 13
#define buzzer 12
#define relay 14
#define blueLight 2

DHTesp dht;

char in;

void setup() {

  Serial.begin(115200);
  Serial.println("\n... Starting ESP32 ...");
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");

  dht.setup(DHTpin, DHTesp::DHT11);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(blueLight, OUTPUT);


  WiFi.begin("phone_EXT", "sysborg1");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  Serial.println("connected");
}


void loop() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String Shumidity = String(humidity);
  String Stemperature = String(temperature);

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;

    http.begin("http://192.168.1.100:5000/posttemp");  //Specify destination for HTTP request

    //   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
    //   int httpResponseCode = http.POST("request body in text formate");   //Send the actual POST request

    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    int httpResponseCode = http.POST("{ \"temperature\" :  " +  Stemperature   + " , \"humidity\":" + Shumidity + " }"); //Send the actual POST request

    if (httpResponseCode > 0) {

      String response = http.getString();                       //Get the response to the request

      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer

    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }



    delay(1000);




    Serial.print(dht.getStatusString());
    Serial.print("\t");
    Serial.print(humidity, 1);
    Serial.print("H\t\t");
    Serial.print(temperature, 1);
    Serial.print("C\t\t");
    Serial.print(dht.toFahrenheit(temperature), 1);
    Serial.print("\t\t");
    Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
    Serial.print("\t\t");
    Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

    http.end();  //Free resources

  } else {
    Serial.println("Error in WiFi connection");
  }

  delay(1000);  //Send a request every 10 seconds

}

//////////////////////////////////
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
//////////////////////////////////
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//////////////////////////////////

//////////////////////////////////
#include "ce_builder.cpp"

/////////////////////////////////
#define SERVER_ENDPOINT "..." // ex : "http://192.168.1.11:3000/sensor_data_receiver"
#ifndef WIFI_SSID
#define WIFI_SSID "..." // wifi name
#define WIFI_PASS  "..." // wifi pass
#endif


/////////////////////////////////////////////
#define DHTTYPE DHT11
#define DHT_SENSOR_PIN D3 // connect DHT Data pin to PIN D3
////////////////////////////////////////////

// sensor
DHT_Unified dht(DHT_SENSOR_PIN, DHTTYPE);

/////////////////////////////////////
void setup() {

  Serial.begin(9600);

  /////////////////////////////////
  // init sensor
  /////////////////////////////////
  Serial.println("init DHT sensor");
  dht.begin();

  /////////////////////////////////
  // wifi
  ////////////////////////////////
  //*
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("\n");
    Serial.println("Try to connect to WIFI");
    Serial.println("MAC : " + WiFi.macAddress());
  }
  Serial.println("Connected to Wifi");
  Serial.print("Connected! Sensor's IP address: ");
  Serial.println(WiFi.localIP());
  /**/
}

void loop() {

  // Delay between measurements.
  delay(1000);

  ///////////////////////////////////////////////////////
  // Sensor Logic
  ///////////////////////////////////////////////////////  
  sensors_event_t event;
  // get temperature
  dht.temperature().getEvent(&event);
  float temperature_value = event.temperature;
  if (isnan(temperature_value)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(temperature_value);
    Serial.println(F("°C"));
  }
  // get humidity
  dht.humidity().getEvent(&event);
  float relative_humidity = event.relative_humidity;
  if (isnan(relative_humidity)) {
    Serial.println(F("Error reading Humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(relative_humidity);
    Serial.println(F("%"));
  }

  ///////////////////////////////////////////////////////
  // WiFi connection
  ///////////////////////////////////////////////////////
  //*
  if ((WiFi.status() == WL_CONNECTED)) {
    
    ////////////////////////////////////////////////////////
    // Prepare Cloudevent object
    //////////////////////////////////////////////////////
    CEBuilder ce;
    // set cloudevent context
    ce.setSpecVersion("v1");
    ce.setId("12345");
    ce.setSource("/my.greenhouse.1");
    ce.setType("my.greenhouse.1.sensor.dht");

    // data container
    string data[] = { "temperature:" , "humidity:" };
    // temperature data
    char str__temperature[] = "000.00";
    dtostrf(temperature_value , -5 , 2 , str__temperature);
    data[0] += str__temperature;
    // humidity data
    char str__humidity[] = "000.00";
    dtostrf(relative_humidity , -5 , 2 , str__humidity);  
    data[1] += str__humidity;
    
    // set temperature/humidity datat to cloudevent object
    ce.setData(data, 2);

    // Get Structured format of the cloudevent
    string ce_body = ce.structuredFormat();
    
    // convert (std::string) to (char *) so that can be used by http.POST()
    char *ce_structured_object = new char[ce_body.length() + 1];
    strcpy(ce_structured_object, ce_body.c_str());
    //
    Serial.printf("%s\n" , ce_structured_object);

    ///////////////////////////////////////////////////
    // HTTP : Connect and Send CloudEvent Object
    ///////////////////////////////////////////////////
    WiFiClient client;    
    HTTPClient http;
    // configure target server and url
    http.begin(client, SERVER_ENDPOINT);

    // cloudevent required Header
    http.addHeader("Content-Type", "application/cloudevents+json; charset=utf-8");

    // start connection and send HTTP header and body
    int httpCode = http.POST(ce_structured_object);
    delete [] ce_structured_object;

    // response
    Serial.printf("[HTTP] POST response code : %d\n", httpCode);
    if(httpCode != 200){
      Serial.printf("[HTTP] POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    }    
    Serial.println("");
    http.end();
  }
  /**/

}

/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "Token del api";            // Put here your Ubidots TOKEN
const char *WIFI_SSID = "INFINITUM4304_2.4";                // Put here your Wi-Fi SSID
const char *WIFI_PASS = "vxMr7ddEx3";                // Put here your Wi-Fi password
const char *PUBLISH_DEVICE_LABEL = "esp32-wroom";     // Put here your Device label to which data  will be published
const char *PUBLISH_VARIABLE_LABEL = "random";   // Put here your Variable label to which data  will be published
const char *SUBSCRIBE_DEVICE_LABEL = "esp32-wroom";   // Replace with the device label to subscribe to
const char *SUBSCRIBE_VARIABLE_LABEL = "onoff"; // Replace with the variable label to subscribe to

const int PUBLISH_FREQUENCY = 5000; // Update rate in millisecondsx

unsigned long timer;

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/
void callback(char *topic, byte *payload, unsigned int length)
{/*
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();*/
  float value=atof((char*)payload);
  Serial.println(value);
  if(value == 1){
    Serial.println("Encender relevador");
  }else if(value == 0){
    Serial.println("Apagar relevador");
  }else{
    Serial.println("Dato inválido");
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL, SUBSCRIBE_VARIABLE_LABEL); // Insert the device and variable's Labels, respectively

}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL, SUBSCRIBE_VARIABLE_LABEL); // Insert the device and variable's Labels, respectively
  }
  if (abs(millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value = random(1,50);
    char contexto[15];
    sprintf(contexto, "\"estado\":%s", "\"enviado\"");
    
    ubidots.add(PUBLISH_VARIABLE_LABEL, value, contexto); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);

  }
    //delay(5000);
  ubidots.loop();
}

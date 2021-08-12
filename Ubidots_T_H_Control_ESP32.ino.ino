///LCD I2C
#include <Wire.h> //Librería para pantalla LCD_I2C
#include <LiquidCrystal_I2C.h>//Librería para pantalla LCD_I2C

//LCD I2C

#include "UbidotsEsp32Mqtt.h" //Librería para hacer la comunicación MQTT a UBIDOTS
#include "DHT.h" //Librería para el sensor de temperatura DHT 11
/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-jtQ4p0lQzzPNr4hsHyUj3lDG06wWRn";  //Token relacionado con el dispositivo Ubidots
const char *WIFI_SSID = "INFINITUM4304_2.4";      // Red a la cual nos vamos a conectar
const char *WIFI_PASS = "vxMr7ddEx3";      // contraseña de la red a la que nos conectamos
const char *DEVICE_LABEL = "esp32-wroom";   // Nombre del dispositivo al que nos estamos conectando
const char *VARIABLE_LABEL = "statusrel"; // Variable de la cual extraeremos la infomración para encender o apagar el ventilador


Ubidots ubidots(UBIDOTS_TOKEN);//Se valida el token de ubidots para conectarnos al dispositivo
float value;
const int relevador = 15; //pin del relevador que controla el ventilador

/*DHT*/
uint8_t DHTPIN = 5; //Pin de datos por el cual se lee el sensor de temperatura
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float humedad = 0;
float temperatura = 0;
/*DHT*/

const int PUBLISH_FREQUENCY = 5000; // Update rate in millisecondsx

unsigned long timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
/****************************************
 * Auxiliar Functions
 ****************************************/
void callback(char *topic, byte *payload, unsigned int length)//función que extrae la información de la variable para verificar si el valor ha cambiado o no
{
  /*Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");*/
  /*for (int i = 0; i < length; i++)
  {
    //Serial.print((char)payload[i]);
  }*/
  value=atof((char*)payload);
  Serial.println((char*)payload);
  Serial.println(value);
  if(value==0)
  {
    digitalWrite(relevador, HIGH);//si el valor es cero apagar el RELE
    delay(20);
  }else{
    digitalWrite(relevador, LOW);//si el valor es diferente enciende el RELE
    delay(20);
  }
  
   //Serial.print(value);
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{   
  Serial.begin(115200);
  //LCD 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  BIENVENIDO   ");
  lcd.setCursor(0,1);
  lcd.print("       :)      ");
  //delay(100);
  //LCD
  
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);//se declara callback como método de consulta hacia la variable
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL);
  
  pinMode(relevador, OUTPUT);
  digitalWrite(relevador, HIGH);
 
  //DHT 
  pinMode(DHTPIN, INPUT);
  dht.begin();
  //DHT
}

void loop()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); // aquí va la variable y el dispositivo que requerimos para checar constantemente su valor
      //digitalWrite(relevador, HIGH);
      //digitalWrite(relevador, LOW);
     if(t>=35)
    {
      digitalWrite(relevador, LOW);//si el valor es cero apagar el RELE
      delay(20);
    }else{
      digitalWrite(relevador, HIGH);//si el valor es diferente enciende el RELE
      delay(20);
    }
  }
  
  if (abs(millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    
    
    if (isnan(h) || isnan(t)) {//si no se ha encontrado el sensor de temperatura se muestra en pantalla
      Serial.println(F("Failed to read from DHT sensor!"));
      lcd.clear();
      
      lcd.setCursor(0,0);
      lcd.print("   Sensor no    ");
      lcd.setCursor(0,1);
      lcd.print("   encontrado   ");
      return;
    }
    /*if(temperatura >= 31){
      ubidots.add(VARIABLE_LABEL, 1);
    }else{
      ubidots.add(VARIABLE_LABEL, 0);
    }*/
    if(humedad == h && temperatura == t){//si el valor de la temperatura es el mismo entonces que no haga prosiga
      return;
    }
    //lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("humedad:         ");
    lcd.setCursor(9,0);
    lcd.print(h);
    lcd.setCursor(15,0);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("   temp:     ");
    lcd.setCursor(9,1);
    lcd.print(t);
    lcd.setCursor(15,1);
    lcd.print("C");
    
    Serial.print(F("Humedad: "));
    Serial.print(h);
    Serial.print(F("% Temperatura: "));
    Serial.print(t);
    Serial.println(F("°C "));
    humedad = h;
    temperatura = t;
    ubidots.add("temperatura", t); // Insertamos sus etiquetas de variable y el valor a enviar
    ubidots.add("humedad", h); // Insertamos sus etiquetas de variable y el valor a enviar
    ubidots.publish(DEVICE_LABEL);// publicamos los valores de las variables
    
  }
    //delay(5000);
  ubidots.loop();
    
}

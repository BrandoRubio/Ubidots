#include "UbidotsEsp32Mqtt.h"

/****************************************
Define Constants
****************************************/

#define WIFISSID "INFINITUM4304_2.4" // Put your WifiSSID here
#define PASSWORD "vxMr7ddEx3"// Put your wifi password here
#define TOKEN  "BBFF-jtQ4p0lQzzPNr4hsHyUj3lDG06wWRn"  //Token relacionado con el dispositivo Ubidots

#define VARIABLE_LABEL_SUB_1 "relay"    // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_2 "relay2"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_3 "relay3"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_4 "relay4"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_5 "relay5"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_6 "relay6"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_7 "relay7"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_8 "relay8"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_9 "relay9"   // Assing the variable label to subscribe
#define VARIABLE_LABEL_SUB_10 "relay10" // Assing the variable label to subscribe

#define DEVICE_LABEL "esp32-wroom" // Assig the device label to subscribe

#define R1 15
#define R2 2
#define R3 4
#define R4 18
#define R5 19
#define R6 21
#define R7 22
#define R8 23
#define R9 32
#define R10 33

const uint8_t NUMBER_OF_VARIABLES = 10;                                                                                                            // Number of variable to subscribe to
char *variable_labels[NUMBER_OF_VARIABLES] = {"relay", "relay2", "relay3", "relay4", "relay5", "relay6", "relay7", "relay8", "relay9", "relay10"}; // labels of the variable to subscribe to

float CONTROL1;  // Name of the variable to be used within the code.
float CONTROL2;  // Name of the variable to be used within the code.
float CONTROL3;  // Name of the variable to be used within the code.
float CONTROL4;  // Name of the variable to be used within the code.
float CONTROL5;  // Name of the variable to be used within the code.
float CONTROL6;  // Name of the variable to be used within the code.
float CONTROL7;  // Name of the variable to be used within the code.
float CONTROL8;  // Name of the variable to be used within the code.
float CONTROL9;  // Name of the variable to be used within the code.
float CONTROL10; // Name of the variable to be used within the code.

float value;      // To store incoming value.
uint8_t variable; // To keep track of the state machine and be able to use the switch case.

const int ERROR_VALUE = 65535; // Set here an error value

/****************************************
Initializate constructors for objects
****************************************/
Ubidots ubidots(TOKEN);

/****************************************
Auxiliar Functions
****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
    char *variable_label = (char *)malloc(sizeof(char) * 30);
    get_variable_label_topic(topic, variable_label);
    Serial.println(variable_label);
    value = btof(payload, length);
    set_state(variable_label);
    execute_cases();
    free(variable_label);
}

// Verifica qué variable ha tenido algún cambio
void get_variable_label_topic(char *topic, char *variable_label)
{
    Serial.print("topic:");
    Serial.println(topic);
    sprintf(variable_label, "");
    for (int i = 0; i < NUMBER_OF_VARIABLES; i++)
    {
        char *result_lv = strstr(topic, variable_labels[i]);
        if (result_lv != NULL)
        {
            uint8_t len = strlen(result_lv);
            char result[100];
            uint8_t i = 0;
            for (i = 0; i < len - 3; i++)
            {
                result[i] = result_lv[i];
            }
            result[i] = '\0';
            Serial.print("Label is: ");
            Serial.println(result);
            sprintf(variable_label, "%s", result);
            break;
        }
    }
}

// Convierte los valores del array a float
float btof(byte *payload, unsigned int length)
{
    char *demo_ = (char *)malloc(sizeof(char) * 10);
    for (int i = 0; i < length; i++)
    {
        demo_[i] = payload[i];
    }
    return atof(demo_);
}

// asigna el número correspondiente al avariable a utilizar en switch case
void set_state(char *variable_label)
{
    variable = 0;
    for (uint8_t i = 0; i < NUMBER_OF_VARIABLES; i++)
    {
        if (strcmp(variable_label, variable_labels[i]) == 0)
        {
            break;
        }
        variable++;
    }
    if (variable >= NUMBER_OF_VARIABLES)
        variable = ERROR_VALUE; // Not valid
}

// Function with switch case to determine which variable changed and assigned the value accordingly to the code variable
void execute_cases()
{
    switch (variable)
    {
    case 0:
        CONTROL1 = value;
        digitalWrite(R1, value);
        Serial.print("CONTROL1: ");
        Serial.println(CONTROL1);
        Serial.println();
        break;
    case 1:
        CONTROL2 = value;
        // digitalWrite(R2, value);
        Serial.print("CONTROL2: ");
        Serial.println(CONTROL2);
        Serial.println();
        break;
    case 2:
        CONTROL3 = value;
        // digitalWrite(R3, value);
        Serial.print("CONTROL3: ");
        Serial.println(CONTROL3);
        Serial.println();
        if(CONTROL3 == 0){
          //Código para apagar relevador
        }else{
          //Código para encender relevador
        }
        break;
    case 3:
        CONTROL4 = value;
        // digitalWrite(R4, value);
        Serial.print("CONTROL3: ");
        Serial.println(CONTROL3);
        Serial.println();
        break;
    case 4:
        CONTROL5 = value;
        // digitalWrite(R5, value);
        Serial.print("CONTROL5: ");
        Serial.println(CONTROL5);
        Serial.println();
        break;
    case 5:
        CONTROL6 = value;
        // digitalWrite(R6, value);
        Serial.print("CONTROL6: ");
        Serial.println(CONTROL6);
        Serial.println();
        break;
    case 6:
        CONTROL7 = value;
        // digitalWrite(R7, value);
        Serial.print("CONTROL7: ");
        Serial.println(CONTROL7);
        Serial.println();
        break;
    case 7:
        CONTROL8 = value;
        // digitalWrite(R8, value);
        Serial.print("CONTROL8: ");
        Serial.println(CONTROL8);
        Serial.println();
        break;
    case 8:
        CONTROL9 = value;
        // digitalWrite(R9, value);
        Serial.print("CONTROL9: ");
        Serial.println(CONTROL9);
        Serial.println();
        break;
    case 9:
        CONTROL10 = value;
        // digitalWrite(R10, value);
        Serial.print("CONTROL10: ");
        Serial.println(CONTROL10);
        Serial.println();
        break;
    case ERROR_VALUE:
        Serial.println("error");
        Serial.println();
        break;
    default:
        Serial.println("default");
        Serial.println();
    }
}

/****************************************
Main Functions
****************************************/

void setup()
{
    Serial.begin(115200);
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);
    pinMode(R5, OUTPUT);
    pinMode(R6, OUTPUT);
    pinMode(R7, OUTPUT);
    pinMode(R8, OUTPUT);
    pinMode(R9, OUTPUT);
    pinMode(R10, OUTPUT);

    // ubidots.setDebug(true);  // uncomment this to make debug messages available
    ubidots.connectToWifi(WIFISSID, PASSWORD);
    ubidots.setCallback(callback);
    ubidots.setup();
    ubidots.reconnect();
    for (uint8_t i = 0; i < NUMBER_OF_VARIABLES; i++)
    {
        ubidots.subscribeLastValue(DEVICE_LABEL, variable_labels[i]); // Insert the device and Variable's Labels
        delay(100);
    }
}

void loop()
{
    if (!ubidots.connected())
    {
        ubidots.reconnect();
        for (uint8_t i = 0; i < NUMBER_OF_VARIABLES; i++)
        {
            ubidots.subscribeLastValue(DEVICE_LABEL, variable_labels[i]); // Insert the device and Variable's Labels
            delay(100);
        }
    }

    ubidots.loop();
}

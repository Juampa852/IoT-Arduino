#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h> //HTTP Requests
//#include <SoftwareSerial.h>

//SoftwareSerial softSerial(3, 2); // RX, TX
//const char* ssid = "TURBONETT_24CA8E";
//const char* password = "c566b7c995";

const char* ssid = "Fam._Monroy";
const char* password = "2dc808998a";

//const char* ssid = "MI";
//const char* password = "nachodudes";

//const char* host = "18.206.88.138";
const String host= "18.206.88.138";
const uint16_t port=8000;
const String endpoint="/configuracion/arduino/color_led/";


void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN,HIGH); //LED OFF
  //Serial.setDebugOutput(true);

  /*if(Serial.available()){
    Serial.println(Serial.read());
  }*/
  //Habilita conexión a WEP
  WiFi.enableInsecureWEP(); 

  Serial.printf("Connecting to %s ", ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  int status;
  while (WiFi.status() != WL_CONNECTED)
  {
    status=WiFi.status();
    String msg="";
    switch(status){
      case 0:
         msg="Cambiando entre statuses";
         //WiFi.printDiag(Serial);
         break;
      case 1:
         msg="No se puede llegar al SSID";
         //WiFi.printDiag(Serial);
         //Serial.println();
         break;
      case 3:
         msg="¡¡¡¡CONECTADO!!!!";
         break;
      case 4:
         msg="Contraseña incorrecta";
         break;
      case 6:
         msg="Desconectado (no configurado en modo estacion)";
         break;
      default:
        msg="Status desconocido";
        break;
    }
    Serial.println(msg);
    Serial.println();
    Serial.println();
    //Serial.print(WiFi.status());
    delay(1000);
    //Serial.print(".");
  }
  Serial.print(WiFi.localIP());
  Serial.println(" connected");
  digitalWrite(LED_BUILTIN,LOW);  // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01);
  
}


void loop()
{
    String temp="";
    HTTPClient http;    //Declare object of class HTTPClient
    
    http.begin(host,port,endpoint);      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    
    int httpCode = http.GET();   //Send the request
    String payload = http.getString();                  //Get the response payload
    Serial.println("Resultado de conectar a " +host+endpoint);
    Serial.print("Código de respuesta: ");
    Serial.println(httpCode);   //Print HTTP return code
    
    Serial.print("Respuesta: '");
    Serial.print(payload);    //Print request response payload
    Serial.println("'");
    Serial.println();
    delay(500);
    while(Serial.available()){
      temp=temp+(char)Serial.read();
    }
    Serial.println();
    Serial.println(temp);
  
   
   http.end();  //Close connection
   
  delay(5000);
}

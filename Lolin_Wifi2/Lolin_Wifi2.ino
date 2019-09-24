#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h> //HTTP Requests


SoftwareSerial softSerial(, tx); // RX, TX
const char* ssid = "TURBONETT_24CA8E";
const char* password = "c566b7c995";

//const char* ssid = "MI";
//const char* password = "nachodudes";

//const char* host = "18.206.88.138";
const String host= "18.206.88.138";
const uint16_t port=8000;
const String endpoint="/configuracion/led/";


void setup()
{
  Serial.begin(9600);
  Serial.println();
  //Serial.setDebugOutput(true);

  //Habilita conexión a WEP
  WiFi.enableInsecureWEP(); 

  Serial.printf("Connecting to %s ", ssid);
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
  Serial.println(" connected");
}


void loop()
{
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
   
   http.end();  //Close connection
  //////////////////////
  /*WiFiClient client;
  
  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print(String("GET "+endpoint) + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    Serial.println("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }*/
  delay(5000);
}

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h> //HTTP Requests
//#include <SoftwareSerial.h>

//SoftwareSerial softSerial(3, 2); // RX, TX
//const char* ssid = "TURBONETT_24CA8E";
//const char* password = "c566b7c995";

const char* ssid = "MI";
const char* password = "nachodudes";

//const char* host = "18.206.88.138";
const String host= "18.206.88.138";
const uint16_t port=8000;

const String get_color = "/configuracion/arduino/color_led/";
const String get_configuraciones = "/configuracion/configuraciones/";
const String post_ingresos = "/horarios/ingresos/todo/";
const String post_huella = "/usuarios/especial/ingreso_huella/";
const String get_huella = "/usuarios/especial/id_bd_huella/";
const String get_puerta = "/configuracion/control_puerta";

const String endpoint="/configuracion/arduino/color_led/";
//Variables
int r ; 
int g ;
int b ; 
boolean huellas ;
boolean led ;
boolean led_rgb ;
boolean ventilador;
boolean puerta;
boolean listo = false;
  
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

  //Serial.printf("Connecting to %s ", ssid);
  //Serial.println();
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
    //Serial.println(msg);
    //Serial.println();
    //Serial.println();
    //Serial.print(WiFi.status());
    delay(1000);
    //Serial.print(".");
  }
  //Serial.print(WiFi.localIP());
  //Serial.println(" connected");
  digitalWrite(LED_BUILTIN,LOW);  // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01);
  
}


void loop()
{
  
  int id = 0;
  HTTPClient http;    //Declare object of class HTTPClient
  fetch();
  if (Serial.available()) {
    char opcion = (char)Serial.read();
    switch(opcion){
      case '1': 
        while (!Serial.available()) {
        }
          id = Serial.read();
          insertarIngreso(id);
        
        break;
      case '2':
        if (Serial.available()) {
          id = Serial.read();
          insertarHuella(id);
        }
        break;
      case '3':
        obtenerColor();
        break;
      case '4':
        obtenerConfiguracion();
        break;
      case '5':
        if (Serial.available()) {
          id = Serial.read();
          obtenerHuella(id);
        }
        break;
      case '6':
        todo();
        break;
      default: id= 0;
    }
  }
  delay(500);
}

void todo(){
  Serial.println(huellas);
  Serial.println(led);
  Serial.println(led_rgb);
  Serial.println(ventilador);
  Serial.println(puerta);
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
}

void fetch(){
  HTTPClient http;
  http.begin(host,port,get_color);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    
  int httpCode = http.GET();   //Send the request
  String payload = http.getString();                  //Get the response payload
  DynamicJsonBuffer  jsonBuffer(200);
  JsonObject& root = jsonBuffer.parseObject(payload);
  r = root["r"];
  g = root["g"];
  b = root["b"];
  http.end();  //Close connection

  //HTTPClient http;
  http.begin(host,port,get_configuraciones);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    
  httpCode = http.GET();   //Send the request
  payload = http.getString();                  //Get the response payload
  DynamicJsonBuffer  jsonBuffer2(200);
  JsonObject& root2 = jsonBuffer2.parseObject(payload);
  huellas = root2["bandera_sensor_huellas"];
  led = root2["bandera_led"];
  led_rgb = root2["bandera_led_rgb"];
  ventilador = root2["bandera_ventilador"];
  puerta = root2["bandera_puerta"];
  http.end();  //Close connection
}

void insertarIngreso(int id) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["huella"] = id;
  HTTPClient http;
  http.begin(host,port,post_ingresos);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header

  String jsonDec="";
  json.printTo(jsonDec);
    
  int httpCode = http.POST(jsonDec);   //Send the request
  Serial.println(httpCode);

  http.end();  //Close connection
}

void insertarHuella(int id){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["id_sensor"] = id;
  HTTPClient http;
  http.begin(host,port,post_huella);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
  
  String jsonDec="";
  json.printTo(jsonDec);
  
  int httpCode = http.POST(jsonDec);   //Send the request
  Serial.println(httpCode);

  http.end();  //Close connection
}

void obtenerColor(){
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
}

void obtenerConfiguracion() {
  Serial.println(huellas);
  Serial.println(led);
  Serial.println(led_rgb);
  Serial.println(ventilador);
  Serial.println(puerta);
  
}

void obtenerHuella(int id) {
  HTTPClient http;
  http.begin(host,port,get_huella+id);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    
  int httpCode = http.GET();   //Send the request
  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonBuffer  jsonBuffer(200);
    JsonObject& root = jsonBuffer.parseObject(payload);
    String id_resp = root["id"];
    Serial.println(id_resp);
  } else {
    String id_resp = "0";
    Serial.println(id_resp);
  }
  http.end();  //Close connection
}

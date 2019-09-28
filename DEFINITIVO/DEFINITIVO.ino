#include <Servo.h>
//#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//SoftwareSerial mySerial(2, 3);  // RX, TX
SoftwareSerial wifi(8, 9);  // RX, TX
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//uint8_t id;
const int pinVentilador = 13;
const int pinLuces = 3;
Servo servo;
const int pinServo=11;
const int anguloInicial=100;
int anguloServo=anguloInicial;

//banderas
int bandera_luz = 0;
int bandera_rgb = 0;
int ventilador = 0;
int registro_sensor = 0;
int puerta = 0;

//datos
int r = 0;
int g = 0;
int b = 0;
int id_sensor = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifi.begin(9600);
  //while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("Programa Sensor de huellas");

  // set the data rate for the sensor serial port
  //finger.begin(57600);
  //delay(5);
/*
  while (!finger.verifyPassword()) {
    Serial.println("Did not find fingerprint sensor :(");
  }
  Serial.println("Found fingerprint sensor!");
*/
  pinMode(pinVentilador,OUTPUT);
  for (int i =10 ; i<12 ; i++){
    pinMode(i, OUTPUT);
  }
 servo.attach(pinServo);
 servo.write(anguloServo);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo.write(anguloServo);
  Serial.println("pruebas");
  wifi.print('6');
  while (!wifi.available()){}
  registro_sensor = wifi.readStringUntil('\n').toInt();
  bandera_luz = wifi.readStringUntil('\n').toInt();
  bandera_rgb = wifi.readStringUntil('\n').toInt();
  ventilador = wifi.readStringUntil('\n').toInt();
  puerta = wifi.readStringUntil('\n').toInt();
  //while(!wifi.available()){
    
  r = wifi.readStringUntil('\n').toInt();
  g = wifi.readStringUntil('\n').toInt();
  b = wifi.readStringUntil('\n').toInt();
    
  Serial.println("sensor " + (String)registro_sensor);
  Serial.println("luz " + (String)bandera_luz);
  Serial.println("rgb " + (String)bandera_rgb);
  Serial.println("ventilador " + (String)ventilador);
  Serial.println("puerta " + (String)puerta);


  Serial.println("r:" + (String)r);
  Serial.println("g:" + (String)g);
  Serial.println("b:" + (String)b);
/*
  if(registro_sensor == 1) {
    Serial.println("Guardar huella");
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();
    if (id == 0) {// ID #0 not allowed, try again!
         return;
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);
    
    while (!  getFingerprintEnroll() );
    
  } else {
    Serial.println("Ingreso");
    int p = -1;
    Serial.println("Waiting for valid finger...");      
    while(p == -1){       
      p=getFingerprintIDez(); 
    }
    Serial.println("-------------");
  }
 */
  configuraciones();
  delay(500);

}

void configuraciones() {
  if (puerta == 0) {
    anguloServo=anguloInicial;
    servo.write(anguloServo);
  }else{
    anguloServo=20;
    servo.write(anguloServo);
  }
  if (ventilador==0) {
    digitalWrite(pinVentilador,true);
  } else {
    digitalWrite(pinVentilador,false);
  }

  if (bandera_luz == 1){
    digitalWrite(pinLuces, true);
  } else {
    digitalWrite(pinLuces, false);
  }
  
  if (bandera_rgb == 1) {
    analogWrite(7 , r) ;   // Red    - Rojo
    analogWrite(6, g) ;   // Green - Verde
    analogWrite(5, b) ;   // Blue - Azul
  } else {
    analogWrite(7 , 0) ;   // Red    - Rojo
    analogWrite(6, 0) ;   // Green - Verde
    analogWrite(5, 0) ;   // Blue - Azul
  }
  
  
}
/*
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


///PARA REGISTRAR UNA HUELLA
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Serial.println("ID de la huella registrada:"+id);
    postHuella(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

//PARA VERIFICAR LAS COINCIDENCIAS DE HUELLAS
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  //return finger.fingerID;
  // hacer el post
  postIngreso(finger.fingerID); 
  //Cambia el ángulo del servo para abrir la puerta
  anguloServo=20;
  return 1;
}

void postHuella(int id){
  wifi.print('2');
  wifi.print(id);
  while (!wifi.available()){}
  String resp = wifi.readStringUntil('\n');
}
void postIngreso(int id){
  wifi.print('1');
  wifi.print(id);
  while (!wifi.available()){}
  String resp = wifi.readStringUntil('\n');
}
*/

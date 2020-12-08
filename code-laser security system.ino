/*
Team members:
1)Jay Patel
2)Kaushal Patel
3)Tanuj Joshi
4)Jaivik Patel 
*/


//import servo lib
#include <Servo.h>
//import software serial lib
#include <SoftwareSerial.h>

//import fingerprint lib from adafruit
#include <Adafruit_Fingerprint.h>
//create servo object
Servo htx;

//define software serial pins
SoftwareSerial mySerial(2, 3);

//create fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//predefined variables for LEDs, Sensor and buzzer
int sensorValue = 0;
const int buzzerPin = 9; 
const int led = 12;
const int led1 = 13;

//define fingerprint IDs as stored!
int FINGER_ID = 1;
int Finder_id2 = 2;
int servoPin = 11;

//initial setup
void setup()
{
  htx.attach(servoPin);
  pinMode(led,OUTPUT);
  pinMode(buzzerPin,OUTPUT);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  //make sure that the fingerprint sensor is connected properly
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    //loop if there was an error detecting the sensor
    while (1) { delay(1); }
  }
}

//forever loop
void loop()                     
{
  
  boolean flag = true;
  
  //read the photoresistor and conver it to ADC value
  sensorValue = analogRead(A0);
  
 //if value is greater than 800
 
  if(sensorValue >=800) {
    while(flag){
    //blink both leds
     blink(led);
     blink(led1);
     //get fingerprint ID
     int id = getFingerprintID();
    //start buzzer
     tone(buzzerPin,2000);
     //move servo 90 degrees
     htx.write(90);
     
     //if the Ids matches any of the stored ID then stop the alarm
     if(id == FINGER_ID || id == Finder_id2){
       flag = false;
     } 
    
     delay(50);
    }
    //stop buzzer and leds
    noTone(buzzerPin);
    digitalWrite(led,LOW);
    digitalWrite(led1,LOW);
    //move servo back to zero
    htx.write(0);
  }
  digitalWrite(led,LOW);
  delay(50);            //don't ned to run this at full speed.
}

//function to blink LED
void blink(int pin){
  digitalWrite(pin,HIGH);
  delay(170);
  digitalWrite(pin,LOW);
  delay(170);
  
}

//function to get fingerprint Id
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p+10;
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
  p = finger.fingerSearch();
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
  return finger.fingerID;
}

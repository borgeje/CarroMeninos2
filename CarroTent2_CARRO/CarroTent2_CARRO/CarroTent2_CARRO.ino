/*
  Sketch    CARRO DOS MENINOS!!
  Este FUNCIONOU!!

*/

// NRF24L01
#include <SPI.h>
#include "RF24.h"
RF24 radio(9, 10);                // define the object to control NRF24L01
const byte addresses[6] = "Free1";// define communication address which should correspond to remote control
// servo
#include <Servo.h>
Servo dirServo;                  // define servo to control turning of smart car
const int dirServoPin = 2;        // define pin for signal line of the last servo
byte dirServoOffset = 0;          // define a variable for deviation(degree) of the servo
byte HIGHSPEED = 1;
// motor
const int dirAPin = 7,            // define pin used to control rotational direction of motor A
          pwmAPin = 6,            // define pin for PWM used to control rotational speed of motor A
          dirBPin = 4,            // define pin used to control rotational direction of motor B
          pwmBPin = 5,            // define pin for PWM used to control rotational speed of motor B
          snsAPin = A0,           // define pin for detecting current of motor A
          snsBPin = A1,           // define pin for detecting current of motor B
          buzzerPin = 6,          // define pin for buzzer
          Velocidade = 3,         // define pin for SPEED
          ledRPin = A3,           // define R pin of RGBLED
          ledGPin = A4,           // define G pin of RGBLED
          ledBPin = A5;           // define B pin of RGBLED
const byte motorSpeed = 255;      // define motor speed(0-255)
#define FORWARD LOW
#define BACKWARD HIGH
// wireless communication
int dataRead[8];                  // define array used to save the read data
const unsigned long wirelessOvertime = 250; // define communication overtime(ms)
unsigned long lastReceivedSignal; // define a variable to save the time(ms) of last received the signal




void setup() {
  // serial port
  Serial.begin(115200);
  // NRF24L01
  radio.begin();                      // initialize RF24
  radio.setChannel(120); 
  radio.setPALevel(RF24_PA_MAX);      // set power amplifier (PA) level
  radio.setDataRate(RF24_1MBPS);      // set data rate through the air
  radio.setRetries(0, 15);            // set the number and delay of retries
  radio.openWritingPipe(addresses);   // open a pipe for writing
  radio.openReadingPipe(1, addresses);// open a pipe for reading
  radio.startListening();             // start monitoringtart listening on the pipes opened
  // servo
  dirServo.attach(dirServoPin); // attaches the servo on dirServoPin to the servo object
  // motor
      digitalWrite(ledRPin, HIGH);
      digitalWrite(ledGPin, HIGH);
      digitalWrite(ledBPin, HIGH);
      digitalWrite(dirAPin, HIGH);
      digitalWrite(dirBPin, HIGH);
      digitalWrite(Velocidade, HIGH);
      digitalWrite(buzzerPin, HIGH);
      
  pinMode(dirAPin, OUTPUT);     // set dirAPin to output mode
  pinMode(pwmAPin, OUTPUT);     // set pwmAPin to output mode
  pinMode(dirBPin, OUTPUT);     // set dirBPin to output mode
  pinMode(pwmBPin, OUTPUT);     // set pwmBPin to output mode
  pinMode(Velocidade, OUTPUT);
  // pin
  pinMode(buzzerPin, OUTPUT);   // set buzzerPin to output mode
  pinMode(ledRPin, OUTPUT);     // set ledRPin to output mode
  pinMode(ledGPin, OUTPUT);     // set ledGPin to output mode
  pinMode(ledBPin, OUTPUT);     // set ledBPin to output mode
  digitalWrite(ledRPin, HIGH);  // turn off R LED
  digitalWrite(ledGPin, HIGH);  // turn off G LED
  digitalWrite(ledBPin, HIGH);  // turn off B LED

      digitalWrite(ledRPin, HIGH);
      digitalWrite(ledGPin, HIGH);
      digitalWrite(ledBPin, HIGH);
      digitalWrite(dirAPin, HIGH);
      digitalWrite(dirBPin, HIGH);
      digitalWrite(Velocidade, HIGH);
      digitalWrite(buzzerPin, HIGH);
}








void loop()
{
  // read radio data
  if ( radio.available()) {             // if receive the data
    while (radio.available()) {         // read all the data
      radio.read(dataRead, sizeof(dataRead));   // read data
    }
    delay(100);
/*    digitalWrite(dirAPin, dataRead[5]);       // dataRead[5] = FRENTE
    Serial.print("DIRECAO A PIN: ");
    Serial.print(dataRead[5]);
    Serial.print("  DIRECAO B PIN:    ");      // dataRead[5] = TRAS
    Serial.print(dataRead[6]);
    Serial.print("   Velocidade:  ");
    Serial.println(dataRead[7]);
    digitalWrite(dirBPin, dataRead[6]);
    digitalWrite(Velocidade, dataRead[7]);

    if(dataRead[0]>500) 
      digitalWrite(buzzerPin, LOW); else digitalWrite(buzzerPin, HIGH); */
 
   // Reading front and back of JOYSTICK 

/*   
    if(dataRead[2]>900) {
        digitalWrite(Velocidade, LOW);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(dirAPin, LOW);
        digitalWrite(dirBPin, HIGH);
        Serial.println("AHEAD   FULL speed!!");
    } else
       {
        if(dataRead[2]>600) {
          digitalWrite(Velocidade, HIGH);
          digitalWrite(buzzerPin, HIGH);
          digitalWrite(dirAPin, LOW);
          digitalWrite(dirBPin, HIGH);
          Serial.println("AHEAD   ");
          } else
            {
              if(dataRead[2]<70) {
                digitalWrite(Velocidade, LOW);
                digitalWrite(buzzerPin, LOW);
                digitalWrite(dirBPin, LOW);
                digitalWrite(dirAPin, HIGH);
                Serial.println("BACK   FULL speed!!");
            } else {
              if(dataRead[2]<450) {
                    digitalWrite(Velocidade, HIGH);
                    digitalWrite(buzzerPin, HIGH);
                    digitalWrite(dirAPin, HIGH);
                    digitalWrite(dirBPin, LOW);
                    Serial.println("BACK  ");
              } else
                 {
                    digitalWrite(Velocidade, HIGH);
                    digitalWrite(buzzerPin, HIGH);
                    digitalWrite(dirAPin, HIGH);
                    digitalWrite(dirBPin, HIGH);
                    Serial.println(">>> STOPPED  ");
                 }
            }
            }
       }
*/

    if(dataRead[2]>900) {
        HIGHSPEED = LOW;
        digitalWrite(dirAPin, LOW);
        digitalWrite(dirBPin, HIGH);
        Serial.print("AHEAD   FULL speed!!");
    } else
       {
        if(dataRead[2]>600) {
          HIGHSPEED = HIGH;
          digitalWrite(dirAPin, LOW);
          digitalWrite(dirBPin, HIGH);
          Serial.print("AHEAD   ");
          } else
            {
              if(dataRead[2]<70) {
                HIGHSPEED = LOW;
                digitalWrite(dirBPin, LOW);
                digitalWrite(dirAPin, HIGH);
                Serial.print("BACK   FULL speed!!");
            } else {
              if(dataRead[2]<450) {
                    HIGHSPEED = HIGH;
                    digitalWrite(dirAPin, HIGH);
                    digitalWrite(dirBPin, LOW);
                    Serial.print("BACK  ");
              } else
                 {
                    HIGHSPEED = HIGH;
                    digitalWrite(Velocidade, HIGH);
                    digitalWrite(buzzerPin, HIGH);
                    digitalWrite(dirAPin, HIGH);
                    digitalWrite(dirBPin, HIGH);
                    Serial.print(">>> STOPPED  ");
                 }
            }
            }
       }   



    if((dataRead[3]>600)&&(HIGHSPEED==LOW)) {
        digitalWrite(Velocidade, HIGH);
        digitalWrite(buzzerPin, LOW);
        Serial.print("DIREITA  = ");
        Serial.println(dataRead[3]);
    } else if((dataRead[3]<450)&&(HIGHSPEED==LOW)) {
          digitalWrite(Velocidade, LOW);
          digitalWrite(buzzerPin, HIGH);
          Serial.print("ESQUERDA  ");
          Serial.println(dataRead[3]);
          } 
          else if((dataRead[3]>600)&&(HIGHSPEED==HIGH)) 
             {
                digitalWrite(Velocidade, HIGH);
                digitalWrite(buzzerPin, HIGH);
                Serial.print("Direita normal (nao vale) = ");
                Serial.println(dataRead[3]);
              }
             else if((dataRead[3]<450)&&(HIGHSPEED==HIGH)) 
                 {
                    digitalWrite(Velocidade, HIGH);
                    digitalWrite(buzzerPin, HIGH);
                    Serial.print("Esquerda normal (nao vale) = ");
                     Serial.println(dataRead[3]);
                  }
             
            else {
                    digitalWrite(Velocidade, HIGHSPEED);
                    digitalWrite(buzzerPin, HIGHSPEED);
                    Serial.print("n/a = ");
                    Serial.println(dataRead[3]);
              }
            



    
 /* Code from https://github.com/Freenove/Freenove_Three-wheeled_Smart_Car_Kit_for_Arduino/blob/master/Sketches/Remote_control/Remote_control_Remote/Remote_control_Remote.ino
  *  
*/
    // save the time of last received the wireless signal
    lastReceivedSignal = millis();
  }
  else {
    // if lose wireless signal for a long time
    if (millis() - lastReceivedSignal >= wirelessOvertime) {
      // stop the car
      Serial.println("No data, then stopping the car");

      // turn off the RGB LED
      digitalWrite(ledRPin, HIGH);
      digitalWrite(ledGPin, HIGH);
      digitalWrite(ledBPin, HIGH);
      digitalWrite(dirAPin, HIGH);
      digitalWrite(dirBPin, HIGH);
      digitalWrite(Velocidade, HIGH);
      digitalWrite(buzzerPin, HIGH);
    }
  }

}






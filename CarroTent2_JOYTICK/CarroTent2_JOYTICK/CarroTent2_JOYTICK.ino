
/*
CARRO DOS MENINOS JOYSTYCK - este funcionou
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;
          // 0 = CARRO
          // 1 = Controle Remoto

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);     // this is as its in the NANO IO SHIELD
/**********************************************************/

const byte addresses[6] = "Free1";

// Used to control whether this node is sending or receiving
bool role = 1;
          // 0 = CARRO
          // 1 = Controle Remoto

const int pot1Pin = A0,           // define POT1
          pot2Pin = A1,           // define POT2
          joystickXPin = A2,      // define pin for direction X of joystick
          joystickYPin = A3,      // define pin for direction Y of joystick
          joystickZPin = 7,       // define pin for direction Z of joystick
          s1Pin = 4,              // define pin for S1
          s2Pin = 3,              // define pin for S2
          s3Pin = 2,              // define pin for S3
          led1Pin = 6,            // define pin for LED1 which is close to POT1 and used to indicate the state of POT1
          led2Pin = 5,            // define pin for LED2 which is close to POT2 and used to indicate the state of POT2
          led3Pin = 8;            // define pin for LED3 which is close to NRF24L01 and used to indicate the state of NRF24L01
// wireless communication
int dataWrite[8];                 // define array used to save the write data



          
void setup() {
  Serial.begin(115200);
  Serial.println(F("Carro dos Minino - Comecando! "));
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
/*/  radio.begin();
//  radio.setChannel(120);
//  radio.setPALevel(RF24_PA_MAX);
 Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  radio.startListening();  // Start the radio listening for data */

  radio.begin();                      // initialize RF24
  radio.setChannel(120);              // Set channel 120
  radio.setPALevel(RF24_PA_MAX);      // set power amplifier (PA) level
  radio.setDataRate(RF24_1MBPS);      // set data rate through the air
  radio.setRetries(0, 15);            // set the number and delay of retries
  radio.openWritingPipe(addresses);   // open a pipe for writing
  radio.openReadingPipe(1, addresses);// open a pipe for reading
  radio.stopListening();              // stop listening for incoming messages
  // pin
  pinMode(joystickZPin, INPUT);       // set led1Pin to input mode
  pinMode(s1Pin, INPUT);              // set s1Pin to input mode
  pinMode(s2Pin, INPUT);              // set s2Pin to input mode
  pinMode(s2Pin, INPUT);              // set s3Pin to input mode
  pinMode(led1Pin, OUTPUT);           // set led1Pin to output mode
  pinMode(led2Pin, OUTPUT);           // set led2Pin to output mode
  pinMode(led3Pin, OUTPUT);           // set led3Pin to output mode

}

void loop() {

  // put the values of rocker, switch and potentiometer into the array
  dataWrite[0] = analogRead(pot1Pin);
  dataWrite[1] = analogRead(pot2Pin);
  dataWrite[2] = analogRead(joystickXPin);
  dataWrite[3] = analogRead(joystickYPin);
  dataWrite[4] = digitalRead(joystickZPin);
  dataWrite[5] = digitalRead(s1Pin);
  dataWrite[6] = digitalRead(s2Pin);
  dataWrite[7] = digitalRead(s3Pin);

    
  // write radio data
  if (radio.write(dataWrite, sizeof(dataWrite)))
  {
    digitalWrite(led3Pin, HIGH);
    delay(20);
    digitalWrite(led3Pin, LOW);
  }

  // make LED emit different brightness of light according to analog of potentiometer
  analogWrite(led1Pin, map(dataWrite[0], 0, 1023, 0, 255));
  analogWrite(led2Pin, map(dataWrite[1], 0, 1023, 0, 255));





  
 
/****************** CONTROLE REMOTO Role ***************************/  
/*
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    Serial.println(F("Now sending"));

//    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( 1, 1 )){
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
 //       Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
//        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role **************************

  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }




/****************** Change Roles via Serial Commands **************************

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    Serial.println(c);
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }

*/
} // Loop


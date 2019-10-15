 // An example demonstrating the multiceiver capability of the NRF24L01+
// in a star network with one PRX hub and up to six PTX nodes

//This sketch is a modification from a video on the ForceTronics YouTube Channel,

//which code was leveraged from http://maniacbug.github.io/RF24/starping_8pde-example.html

//This sketch is free to the public to use and modify at your own risk

#define WHICH_NODE 4 
#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <math.h>
int  sensors[] = {A0,A1};
#define numSensors (sizeof(sensors)/sizeof(char *)) 

#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/

const int pinCE = 7; //This pin is used to set the nRF24 to standby (0) or active mode (1)
unsigned long last_time = 0;
int period = 33;
const int pinCSN = 8; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out
const int frequencies[] = {800, 1000};
int currentIndex[] = {0, 0};
//int flip[]={-1,1};
short sins[2];
RF24 radio(pinCE, pinCSN); // Create your nRF24 object or wireless SPI connection

   // must be a number from 1 - 6 identifying the PTX node

const uint64_t wAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};

const uint64_t PTXpipe = wAddress[ WHICH_NODE - 1 ];   // Pulls the address from the above array for this node's pipe

byte counter = 1; //used to count the packets sent

bool done = false; //used to know when to stop sending packets


void setup()

{

//  Serial.begin(115200);   //start serial to communicate process




  radio.begin();            //Start the nRF24 module

  radio.setPALevel(RF24_PA_HIGH);  // "short range setting" - increase if you want more range AND have a good power supply
  radio.setChannel(124);          // the higher channels tend to be more "open"
  radio.setDataRate(RF24_2MBPS);
  radio.openWritingPipe(PTXpipe); //open reading or receive pipe

  radio.stopListening(); //go into transmit mode

}

void loop()

{



  if (millis() > last_time + period) {

  last_time = millis();
    

  int currentSensor=numSensors;
 while(currentSensor--){

 
  sins[currentSensor] =(short) analogRead(sensors[currentSensor]);
 }

    
//sweep  
//    int i = 2;
    //    while (i--)
//    {
//      //Serial.println(count);
//      sins[i] = (float)currentIndex[i]/(float)frequencies[i];
//      currentIndex[i] = (currentIndex[i] + 1) % frequencies[i];
//    }


    if ( !radio.write(&sins, sizeof(sins))) {

      Serial.println("delivery failed");

    }
  
  }


}

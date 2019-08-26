// An example demonstrating the multiceiver capability of the NRF24L01+
// in a star network with one PRX hub and up to six PTX nodes

//This sketch is a modification from a video on the ForceTronics YouTube Channel,
//which code was leveraged from http://maniacbug.github.io/RF24/starping_8pde-example.html
//This sketch is free to the public to use and modify at your own risk

#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+

#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
unsigned long time_now = 0;
int period = 10;
const int pinCE = 7; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const int pinCSN = 8; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out
RF24 radio(pinCE, pinCSN); // Declare object from nRF24 library (Create your wireless SPI)

//Create up to 6 pipe addresses P0 - P5;  the "LL" is for LongLong type
const uint64_t rAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };

   double sins[2];
   byte packets[9];
void setup()   
{
  Serial.begin(115200);  //start serial to communication

  radio.begin();  //Start the nRF24 module

  radio.setPALevel(RF24_PA_HIGH);  // "short range setting" - increase if you want more range AND have a good power supply
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(124);          // the higher channels tend to be more "open"

  // Open up to six pipes for PRX to receive data
  radio.openReadingPipe(0,rAddress[0]);
  radio.openReadingPipe(1,rAddress[1]);
  radio.openReadingPipe(2,rAddress[2]);
  radio.openReadingPipe(3,rAddress[3]);
  radio.openReadingPipe(4,rAddress[4]);
  radio.openReadingPipe(5,rAddress[5]);
  
  radio.startListening();                 // Start listening for messages
}

void loop()  
{   
time_now = millis();

  while (millis() < time_now + period) {


    byte pipeNum = 0; //variable to hold which reading pipe sent data
    byte gotByte = 0; //used to store payload from transmit module
    
    while(radio.available(&pipeNum)){ 

     radio.read(&sins,sizeof(sins)); 
  

      byte pipe[1];
      pipe[0]=pipeNum;


     memcpy(packets, sins, sizeof(sins));
     memcpy(packets+sizeof(sins), pipe, sizeof(pipe));

      
      Serial.write((byte *)packets,sizeof(packets));

    
    }

  }   
}

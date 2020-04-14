//////////////////////////////////////////////////////////////////////////////////////////
//
//    Firmware for ADS1292 with ESP8266 (Node mcu v1.0 with ESp-12E)
//    Author: Deepak Khatri
//    Date: 13 April 2020
//    https://deepaklorkhatri007.github.io/
//
//    Copyright (c) 2020 Deepak Khatri
//   
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT). 
//   
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT 
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
//
//    Inspiration for the code:
//    1. https://bois083.wordpress.com/lucid-dreaming-device/version-5-0/ads1292-firmware/
//    2. https://github.com/Protocentral/ADS1292rShield_Breakout/tree/master/software
//
//    Working on a new simpler Arduino Library
//    https://github.com/deepaklorkhatri007/ADS1292
//
///////////////////////////////////////////////////////////////////////////////////////// 


#include "ads1292r.h"
#include <SPI.h>
ads1292r ADS1292;

// for storing the channel data
unsigned long channelData[2];

// get the data from the chip
// and store it in channelData
void ADS1292_UpdateChannelData()
{
   uint8 i, j, tmp;
   
   digitalWrite(ADS1292_CS_PIN, LOW);
   
   // read 3 byte status register (1100 + LOFF_STAT[4:0] + GPIO[1:0] + 13 '0's)
   tmp = SPI.transfer(0xFF);
   tmp = SPI.transfer(0xFF);
   tmp = SPI.transfer(0xFF);

   // read the channel data
   for(i = 0; i < 2; i++)
   {  
      channelData[i] = 0;
      
      // read 24 bits of channel data in 3 byte chunks
      for(j = 0; j < 3; j++)
      {
         tmp = SPI.transfer(0xFF);  // get one byte
         channelData[i] = ((channelData[i]) << 8) | tmp; //shift the data by a byte and add the new byte
      }
      
      // convert 3 byte 2's complement to 4 byte 2's complement
      if((channelData[i] >> 23) == 1)
      {
         channelData[i] |= 0xFF000000;
      }
      else
      {
         channelData[i] &= 0x00FFFFFF;
      }
   }
   
   digitalWrite(ADS1292_CS_PIN, HIGH);
}

void setup() {
  delay(2000);
  // initalize the  data ready and chip select pins:
  pinMode(ADS1292_DRDY_PIN, INPUT);  //6
  pinMode(ADS1292_CS_PIN, OUTPUT);    //7
  pinMode(ADS1292_START_PIN, OUTPUT);  //5
  pinMode(ADS1292_PWDN_PIN, OUTPUT);  //4

  Serial.begin(115200);  // Baudrate for serial communica

  ADS1292.ads1292_Init();  //initalize ADS1292 slave

  Serial.println("Initiliziation is done");
}

bool transmitOn = true;

void loop() {
  
  if(transmitOn){
         if((digitalRead(ADS1292_DRDY_PIN)) == LOW) // new data available
         {
              ADS1292_UpdateChannelData();
//              Serial.println(channelData[0]);   // send channel 1 data///
              Serial.println(channelData[1]);   // send channel 2 data//
         }
      }
      
}

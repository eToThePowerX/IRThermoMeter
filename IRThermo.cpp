#include "LowPower.h"

float tempC=0.00, tempF;
float arr[100];
int androidCmd; // Command message from Android
int count = 0;
int toggle = 0;
int countTimer=0;

void setup() {
  
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  
  Serial.begin(9600);
  Wire.begin(); 

  company();
  for(;;)
  {
    if(digitalRead(4)==HIGH)
    {
    }
    else
    {
      toggle=1;
      break;
    }
    
    if(digitalRead(5)==HIGH)
    {
    }
    else
    {
      toggle=0;
      break;
    }
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print("Press to Continue");
  display.display();

  countTimer = 0;
  timer1();
}

void loop() {
  while(digitalRead(4)==HIGH);
  countTimer = 0;
  delay(2000);
  
  /*Temperature Calculation*/
  Wire.beginTransmission(0X5A); // start transmission to device
  Wire.write(0X07); // sends register address to read from
  Wire.endTransmission(false); // end transmission
  Wire.requestFrom(0X5A, (size_t)3); // send data n-bytes read
  uint16_t ret;
  ret = Wire.read(); // receive DATA
  ret |= Wire.read() << 8;  // receive DATA   
  uint8_t pec = Wire.read();
  tempC = ret;
  tempC *= .02;
  tempC -= 273.15;
  tempC += 2.5;
  tempF = (tempC * (9.0/5.0)) + 32;

  /*Send to Android*/

    if(toggle==1)
    {
      Serial.print(tempC);Serial.print("°C"); Serial.println("/n");
    }
    else
    {
      Serial.print(tempF);Serial.print("°F"); Serial.println("/n");
    }

  
  if(Serial.read()=='p')
  {
    /*Display*/
    display.clearDisplay();
    oledDisplayHeader();
    bluetoothSignOn();

    check_adc();
    
    if(toggle==1)
    {
      oledDisplay(2,20,30,tempC,'C');
    }
    else
    {
      oledDisplay(2,20,30,tempF,'F');
    }
    display.display();
  }
  else
  {
    /*Display*/
    display.clearDisplay();
    oledDisplayHeader();
    bluetoothSignOff();
    
    check_adc();
        
    if(toggle==1)
    {
      oledDisplay(2,20,30,tempC,'C');
    }
    else
    {
      oledDisplay(2,20,30,tempF,'F');
    }

    display.display();
    
    arr[count++]=tempF;
    if(count==100)
    {
      capacityFull();
      while(digitalRead(5)==HIGH);
      delay(2000);
      for(int k=0;k<count;k++)
      {
        if(toggle==1)
        {
          Serial.print(tempC);Serial.print("°C"); Serial.println("/n");
        }
        else
        { 
          Serial.print(tempF);Serial.print("°F"); Serial.println("/n");
        }
        if(Serial.read()=='p')
        {
          
        }
        else
        {
          
        }
        arr[k] = 0.00;
        delay(500);
      }
      count=0;
      capacityEmpty();
    }
  }
}

void capacityFull(){
  clearHeader();
  bufferHeader();
}
void clearHeader(){
 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(24, 11);
 display.print("MEMORY IS FULL");
 display.display();
 delay(100);
}
void bufferHeader(){
  //display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  display.println(F("Please connect to the      device...."));
  display.display();      // Show initial text
  delay(100);
}
void capacityEmpty(){
  display.clearDisplay();
  bufferedHeader();
  clearedHeader();
  display.display();
}

void clearedHeader(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print("Press to Continue");
  display.display();
}
void oledDisplayHeader(){
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(35, 10);
    if(tempF<=96.0)
    {
        display.print("    LOW");
    }
    else if(tempF>96.0 && tempF<=99.0)
    {
        display.print("  No Fever");
    }
    else if(tempF>99.0 && tempF<=100.0)
    {
        display.print(" Mild Fever");
    }
    else if(tempF>100.0 && tempF<=102.0)
    {
        display.print("Moderate Fever");
    }
    else
    {
        display.print(" High Fever");
    }
}
void company(){
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(2, 20);
  display.print("BAPATLA");
  display.display();
  delay(2000);
}
void SelectMode()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(35, 16);
  display.print("Select Mode");
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 36);
  display.print("degC  degF");
  display.display();
  delay(1000);
}



void check_adc()
{
  int voltage = analogRead(A0);
  if(voltage>0 && voltage<205)
  battery_20();
  if(voltage>205 && voltage<409)
  battery_40();
  if(voltage>409 && voltage<614)
  battery_60();
  if(voltage>614 && voltage<818)
  battery_80();
  if(voltage>818 && voltage<1023)
  battery_100();
}

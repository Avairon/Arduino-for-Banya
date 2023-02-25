#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <microDS18B20.h>
#include <math.h>

LiquidCrystal_I2C LCD(0x3F,16,2); // присваиваем имя LCD для дисплея

int low_hy = 3;
int high_hy = 4; //порты
int test_temp = 55; //задаём проверочную температуру 
int rel = 7;

int flagg = 0;

MicroDS18B20<10> parTemp;
MicroDS18B20<11> roomTemp;

void water(int rele, int highH)
{
  while(digitalRead(highH) == 0) 
  {
    digitalWrite(rele, HIGH);
  }
  digitalWrite(rele, LOW);
}

void AFKwater(int lowH, int waterRel)
{
  while(digitalRead(lowH) == 0) 
  {
    digitalWrite(waterRel, HIGH);
  }
  digitalWrite(rel, LOW);
}

void lcdLevel(int lowL, int highL)
{
  LCD.setCursor(0, 0);
  LCD.print("par ");
  LCD.setCursor(4, 0);
  
  parTemp.requestTemp();
  if (parTemp.readTemp()){
    if(round(parTemp.getTemp()) >= 100)
    {
      LCD.print(round(parTemp.getTemp()));
      LCD.setCursor(7,0);
      LCD.print("'C  ");
    }
    else
    {
      LCD.print(round(parTemp.getTemp()));
      LCD.setCursor(6,0);
      LCD.print("'C   ");
    }
  }

  LCD.setCursor(0, 1);
  LCD.print("kom ");
  LCD.setCursor(4, 1);
  
  roomTemp.requestTemp();
  if (roomTemp.readTemp()){
    if(round(roomTemp.getTemp()) >= 100)
    {
      LCD.print(round(roomTemp.getTemp()));
      LCD.setCursor(7,1);
      LCD.print("'C  ");
    }
    else
    {
      LCD.print(round(roomTemp.getTemp()));
      LCD.setCursor(6,1);
      LCD.print("'C   ");
    }
  }
  
  
  LCD.setCursor(11, 1);
  LCD.print("Bak");
  
  if(digitalRead(lowL) == 1) 
  {
    LCD.setCursor(14, 1);
    LCD.print("|");
  }
  else
  {
    LCD.setCursor(14, 1);
    LCD.print("_");
  }
  
  if(digitalRead(highL) == 1) 
  {
    LCD.setCursor(15, 1);
    LCD.print("|");
  }
  else
  {
    LCD.setCursor(15, 1);
    LCD.print("_");
  }
  
}

void setup() 
{
  LCD.init();
  LCD.backlight();  
  LCD.clear();
  
  Serial.begin(115200);
  
  pinMode(low_hy, INPUT_PULLUP);
  pinMode(high_hy, INPUT_PULLUP);

  pinMode(rel, OUTPUT);
}
  
void loop()
{
  lcdLevel(low_hy, high_hy);
  
  parTemp.requestTemp();
  if (parTemp.readTemp())
  {
    if(round(parTemp.getTemp()) >= test_temp)
    {
      if(flagg == 0 && digitalRead(high_hy) == 0)
      {
        water(rel, high_hy);
        flagg = 1;
      } 
    }
  }

  if (parTemp.readTemp())
  {
    if(round(parTemp.getTemp()) <= 50) flagg = 0;
  }
  AFKwater(low_hy, rel);
  
  delay(1000);
}

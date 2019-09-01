
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
dht DHT;
//#define dht_apin A3
//#define SensorPin 2
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(10,11);
 unsigned long int avgValue;
float b;
  int buf[10],temp;

LiquidCrystal_I2C lcd( 0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//int LDR_Pin = A0;
//int MQ3_Pin = A1;

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  s.begin(115200);
Serial.begin(115200);  
lcd.begin(16, 2);


}

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop() 
{
 //ldr

int LDR_val= analogRead(A0);
 root["l"] = LDR_val;
Serial.print("LDR ");
Serial.println(LDR_val);

//mq3

int MQ3_val= analogRead(A1);
Serial.print("MQ3 ");
Serial.println(MQ3_val);
root["m"] = MQ3_val; 

//dht11

  int chk = DHT.read11(A3);
  int tempt = DHT.temperature;
  int hum = DHT.humidity;
  root["t"]=tempt;
  root["h"] = hum;
 //delay(500);
  
  //tempt = tempt-25;
 // hum=hum-10;
  Serial.print("HUmidity ");
  Serial.println(hum);
  Serial.print("Temperature ");
  Serial.println(tempt);

  //ph
  
  for(int i=0; i<10 ; i++)
  {
    buf[i]=analogRead(A2);
    delay(10);
  }
  for(int i=0; i<9 ; i++)
  {
    for(int j=i+1 ; j<10 ; j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp; 
      }
    }
  }
  avgValue=0;
  for(int i=2; i<8; i++)
  {
    avgValue+=buf[i];
  }
  float phValue=(float)avgValue*5.0/1024/6;
  phValue=3.5*phValue;
  Serial.print("PH ");
  Serial.println(phValue);
  root["p"]= phValue;
  
  if(s.available()>0)
{
 root.printTo(s);
}
 
 int i=0;
 int w =0;
 int f=0;
 delay(1000);
   
   
  if((tempt>4 && tempt<60) || hum > 60 || LDR_val<50 ){
      w++;
      
    }
    //phValue=phValue-4;
    
     
     if((MQ3_val>300 && MQ3_val<400) || phValue<6.1) {
        
       i++;
      }
    
     if (i>0 ){
      lcd.clear();
      lcd.setCursor(8,0);
      lcd.print("HARMFUL");
      lcd.setCursor(0,1);
      lcd.print("CHANCE-");
      lcd.setCursor(7,1);
      lcd.print("Diarrhea");
        f++; 
      } else if (w>0 && f<1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WARNING");
        lcd.setCursor(0,1);
        lcd.print("USER choice");
      }else if(i<1 && w<1){
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Go eat");
      }
}

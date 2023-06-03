#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
const int rs = 9 , en = 10 , d4 = 11 , d5 = 12 , d6 = 13 , d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define RX 2
#define TX 3
String AP = "Spam";       // AP NAME
String PASS = "7218824546"; // AP PASSWORD
String API = "NE3QJJM62ETZPH0V";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int sensorPin = A5;
int a = 18;
int sensorValue;
int turbidity;
SoftwareSerial esp8266(RX,TX);
void setup() 
{
  pinMode(a ,OUTPUT);
  lcd.begin(16 , 2);
  digitalWrite(a , HIGH);
  pinMode(a, OUTPUT);
  Serial.begin(9600);
  //delay(1000);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
 // delay(1000);
}

void loop()
{
 String getData ="GET /update?api_key="+ API +"&field1=" + getValue();
 String pass = getValue();
 printdata(pass);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0,"+String(getData.length()+4),4,">");
 esp8266.println(getData);
 //delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 }


String getValue()
{
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  turbidity = map(sensorValue, 0, 750, 100, 0);
 
  return String(turbidity);
}


void sendCommand(String command, int maxTime, char readReplay[]) 
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
 
    countTimeCommand++;
  }
 
  if(found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
 //F1A
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
 
  found = false;
 }

 void printdata(String t)
 {
   turbidity = t.toInt();
   if(turbidity < 100)
  {
    lcd.setCursor(12, 0);
    lcd.print(" ");
  }
  //delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Turbidity: ");
  Serial.print("Turbidity:");
  Serial.print("   ");
  lcd.setCursor(10, 0);
  lcd.print(turbidity);
  Serial.println(turbidity);
  //delay(200);
  if(turbidity >25)
  {
    lcd.setCursor(0, 1);
    lcd.print("harmful");
   Serial.print("harmful");
  }
  else if (turbidity >= 16)
  {
    lcd.setCursor(0, 1);
    lcd.print("Normal");
    Serial.print("Normal");
     lcd.setCursor(6,1);
    lcd.print(" ");
  }
  else if (turbidity <= 15 )
  {
  lcd.setCursor(12,0);
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print("Good");
  lcd.setCursor(4,1);
  lcd.print("      ");
  Serial.print("Good ");
  }
 }

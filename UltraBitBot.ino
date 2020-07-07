#include <Servo.h>
Servo myservo;

//Left Infrared Sensor = 2
//Right Infrared Sensor = 13
//Right Motor Input 1 = 4
//Right Motor Input 2 = 5
//Left Motor Input 1 = 6
//Left Motor Input 2 = 7
//US Trigger Pin = 8
//US Echo Pin = 9
//Enable A = 3
//Servo pulse pin = 12
//LDR Output = A0

int count, lth;
boolean activeLED, activeT;

void setup() 
{
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(13,INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,INPUT);
  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);
  myservo.attach(12);
  count = 0;//This is the number of leds on
  lth = 500;//This is the threshold limit of ldr
  activeLED = false;
  activeT = true;
}

void loop() 
{
  analogWrite(3,255);
  analogWrite(11,255);
  //Here we read the LDR
  int led = analogRead(A0);
  if(led>lth and !(activeLED))
  {
    activeLED = true;
  }
  else if(led<lth and activeLED)
  {
    count = count + 1;
    activeLED = false;
  }
  //Here we read the Ultrasonic sensor
  digitalWrite(8,LOW);
  delayMicroseconds(2);
  digitalWrite(8,HIGH);
  delayMicroseconds(10);
  digitalWrite(8,LOW);
  int duration = pulseIn(9,HIGH);
  double distance = duration/58.82;
  //If the obstacle is at distance more than 5 cm then the bot runs normally
  if(distance>10.0)
  {
    if(digitalRead(2)==HIGH and digitalRead(13)==HIGH)
    {
      //If the bot is in a simple region without the flags
      if(activeT == false)
      {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
      }
      //If the bot is in the flag region
      else
      {
        activeT = false;        
      }
    }
    //Turn right
    if(digitalRead(2)==HIGH and digitalRead(13)==LOW)
    {
      if(activeT==false)
      {
        digitalWrite(4,LOW);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
      }
      else
      {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,HIGH);
      }
    }
    //Turn left
    if(digitalRead(2)==LOW and digitalRead(13)==HIGH)
    {
      if(activeT==true)
      {
        digitalWrite(4,LOW);
        digitalWrite(5,HIGH);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
      }
      else
      {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
        digitalWrite(7,HIGH);
      }
    }
    //Here we determine whether the region is the flag region or not
    if(digitalRead(2)==LOW and digitalRead(13)==LOW)
    {
      if(!(activeT))
      {
        activeT = true;
        count = count - 1;
        //Here we determine and knock down the correct flag
        if(count==0)
        {
          delay(100);
          for(int i=0;i<252;i=i+4)
          {
            myservo.write(i);
            delay(10);
          }
          delay(100);
          for(int i=252;i>0;i=i-4)
          {
            myservo.write(i);
            delay(10);
          }
          delay(100);
        }
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
      }
      else
      {
        digitalWrite(4,HIGH);
        digitalWrite(5,LOW);
        digitalWrite(6,HIGH);
        digitalWrite(7,LOW);
      }
    }
    delay(10);
  }
  //If the obstacle is less than 5 cm away, then stop and wait until the obstacle is more than 5 cm away(applicable in case of first barrier)
  else
  {
    delay(500);
  }
}

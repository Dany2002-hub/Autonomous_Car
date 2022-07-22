#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "HDZwONJA-3TIzIs_nSa9mdYfjDw_AZAN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "N";
char pass[] = "ioup0833";

BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  if (y>220){forward();}
  else if (y<35){backward();}
  else if (x>220){right();}
  else if (x<35){left();}
  else {Stop();}
    if (WiFi.status() == WL_CONNECTED){digitalWrite(9,LOW);}
}

BLYNK_WRITE(V0) {
  int v = param[0].asInt();
  analogWrite(D0, v/4);//D0 is analog pin for setting speed
}

void setup(){
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  pinMode(D0,OUTPUT);
  pinMode(D2,OUTPUT);
  analogWrite(D2,0);
  Blynk.begin(auth, ssid, pass);
}



void loop(){
  if (WiFi.status() != WL_CONNECTED){digitalWrite(9,HIGH);analogWrite(D2,0);}
  Blynk.run(); 
}


void forward(){
analogWrite(D2,50);
}

void backward(){
analogWrite(D2,300);
}

void left(){
analogWrite(D2,160);
}

void right(){
analogWrite(D2,105);
}

void Stop(){
analogWrite(D2,0);
}

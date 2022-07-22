///////////////////////////////////////////////////////// Code to be uploaded in Arduino Uno with fixed Motor Shield
#include <AFMotor.h> 
AF_DCMotor RMF(1, MOTOR12_64KHZ);   // RightMotorForward 
AF_DCMotor LMF(2, MOTOR12_64KHZ);   // LeftMotorForward  
AF_DCMotor LMB(3, MOTOR12_64KHZ);   // LeftMotorBackward
AF_DCMotor RMB(4, MOTOR12_64KHZ);   // RightMotorBackward

// For Ultrasonic Sensor
#define echoPin A1 
#define trigPin A0

#include <Servo.h> 
Servo servo;

//For rf reciver
#include <RCSwitch.h>
RCSwitch driver = RCSwitch();

/////////////////////////////////////////////////////////// Initial Setup.
void setup() {
  servo.attach(9);
  delay(5000);
  servo.write(90);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(1000);

  pinMode(10, OUTPUT);pinMode(A4, OUTPUT);
  driver.enableReceive(0);  // Receiver on interrupt 0,i.e., pin #2
  Serial.begin(9600);
  
}
/////////////////////////////////////////////////////////////////////// Defining Functions to be used in loop

short odcm(){      // Obstacle Distance(in cm) -Function
  digitalWrite(trigPin, LOW); // Clears the trigPin condition
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); 
  long duration = pulseIn(echoPin, HIGH); //Reads the echoPin, returns the sound wave travel time in micro sec
  short distance = duration * 0.033 / 2; // Calculating the obstacle distance
  //Serial.println(distance);
  return distance;
}

int cdd(){  // Check Driving Direction with servo -Function
  short di;
  int d0=0;
  di=odcm();
  if (di<= 30){
    Stop();
    servo.write(0);
    delay(700);
    short d=odcm();
    if (d>50) {
      return (3);
      
    }
    else if (d<=30) {
      d0=d0+10;

      servo.write(180);
      delay(900);
      short d1;
      d1=odcm();
      if (d1>50) {
        return(2);}
      else if (d1<=30){
        d0=d0+10;
        if (d0==20){
          return(1);
        }
      }
      else {
        if (d>=d1) {
          return(3);
        }
        else {
          return(2);
        }
      }
    }
  }
  else {
    return(4);
  }
 // function will return 1 for back, 2 for left, 3 for right, 4 for forward
}

////////////////////////DC Motor running functions
void Stop(){        ///// To apply brack -Function
  RMF.run(RELEASE);
  LMF.run(RELEASE);
  RMB.run(RELEASE);
  LMB.run(RELEASE);
  digitalWrite(10, HIGH);
}

void moveForward(){   //// To move forward -Function
  RMF.run(FORWARD);
  LMF.run(FORWARD);
  RMB.run(FORWARD);
  LMB.run(FORWARD);
  digitalWrite(10, LOW);
}

void moveBack(){      //// To move backward -Function
  RMF.run(BACKWARD);
  LMF.run(BACKWARD);
  RMB.run(BACKWARD);
  LMB.run(BACKWARD);
  digitalWrite(10, HIGH);
}

void turningRight(){     //// To start turning right -Function
  RMF.run(BACKWARD);
  LMF.run(FORWARD);
  RMB.run(RELEASE);
  LMB.run(RELEASE);
}

void turningLeft(){     //// To start turning left -Function
  RMF.run(FORWARD);
  LMF.run(BACKWARD);
  RMB.run(RELEASE);
  LMB.run(RELEASE);
}
/////////////////////////////////// Driving modes
int v;
void autoMode(){         // Car running in autonomously -Function
  v=255;
  RMF.setSpeed(v);
  LMF.setSpeed(v);  
  RMB.setSpeed(v);
  LMB.setSpeed(v);
  int ck = cdd();   //// checking path
 // if (ck!=4){Stop(); delay(200);}
  if (ck==4) {
    servo.write(90);
    moveForward();
    delay(20);
  }
  else if (ck==3){
    servo.write(90);
    delay(400);
    turningRight();
    delay(1850);
  }
  else if (ck==2){
    servo.write(90);
    delay(400);
    turningLeft();
    delay(1850);
  }  
  else if (ck==1){
    servo.write(90);
    moveBack();
    delay(1500);
    turningRight();
    delay(1850);
  } 
 // Serial.println(ck);
}

void AndroidControlMode() {     // Car running in android control mode -Function
  int a2= analogRead(A2);
 Serial.println(a2);
  int vv= analogRead(A5);
  v=(vv/2.7);
  if (v>255){v=255;}
  RMF.setSpeed(v);
  LMF.setSpeed(v);  
  RMB.setSpeed(v);
  LMB.setSpeed(v);
  servo.write(90);
  if (a2>65 && a2<=205){
    int safe=odcm();
    if (safe > 30){
      moveForward();  
    }  
    else {Stop();
    delay(20);}
  }
  else if (a2>205 && a2<=425){
    turningRight();
  }
  else if (a2>425 && a2<=622){
    turningLeft();
  }
  else if (a2>622){
    moveBack();
  }
  else{
    Stop();
  }
}


//////////////////////////////////////////////////////////// Initialising Loop...
void loop() {
  int m=digitalRead(13);
  int a3=analogRead(A3);
  Serial.print("a3capacitor voltage");Serial.println(a3);  
  if (driver.available()) {      digitalWrite(A4, HIGH);     
      int x= driver.getReceivedValue(); 
      Serial.println(x); 
      if (x==48){ v=0;}
      else if (x==49){v=180;}
      else if (x==50){v=190;}
      else if (x==51){v=210;}
      else if (x==52){v=225;}
      else if (x==53){v=235;}
      else if (x==54){v=245;}
      else if (x==55){v=255;}
      RMF.setSpeed(v);
      LMF.setSpeed(v);  
      RMB.setSpeed(v);
      LMB.setSpeed(v);
      if (x==114){int safe=odcm();
         if (safe > 30){moveForward();  }  
         else {Stop();delay(20);};}
      else if (x==108){turningLeft();}
      else if (x==102){turningRight();}
      else if (x==98){moveBack();}
      else if (x==115){Stop();}
      driver.resetAvailable();delay(10);digitalWrite(A4,LOW);}
      else if (m==1 && a3<200){autoMode();}
      else if (a3<200){AndroidControlMode();}
  
}

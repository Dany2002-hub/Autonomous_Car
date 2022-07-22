//  Example for different sending methods
  
 // https://github.com/sui77/rc-switch/

#include <RCSwitch.h>

RCSwitch rf_driver = RCSwitch();
 
#include <Wire.h> 
int ADXL345 = 0x53;
float X_out, Y_out, Z_out;  
float roll,pitch,rollF,pitchF=0;
int LR, FB;

 
void setup(){
  rf_driver.enableTransmit(12);
  
  pinMode(13, OUTPUT);digitalWrite(13,HIGH);
  pinMode(2, OUTPUT);digitalWrite(2,LOW);
  pinMode(3, OUTPUT);digitalWrite(3,HIGH);
  Wire.begin();
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access to POWER_CTL Register
  // Enable measurement
  Wire.write(8); // Bit D3 High for measuring enable
  Wire.endTransmission();delay(10);
  //Off-set Calibration
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);Wire.write(1);
  Wire.endTransmission();delay(10);
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);Wire.write(-2);
  Wire.endTransmission();delay(10);
  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);Wire.write(-9);
  Wire.endTransmission();delay(10);
}

void readOrientation(){
        // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers, each axis value in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); X_out = X_out / 256; // X-axis value
   //For a range of +-2g, we need to divide the raw values by 256
  Y_out = ( Wire.read() | Wire.read() << 8); Y_out = Y_out / 256; // Y-axis value  
  Z_out = ( Wire.read() | Wire.read() << 8); Z_out = Z_out / 256; // Z-axis value  
  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
  // Low-pass filter to remove sudden jerks
  rollF = 0.7 * rollF + 0.3 * roll;
  pitchF = 0.7 * pitchF + 0.3 * pitch;
  // Fixing reading error
  LR = rollF+8.3;
  FB = pitchF+3; 
} 
void speedControl (){//Reading and sending speed control signal -function
  int v =analogRead(A3);
  if (v<128){rf_driver.send(48,8);}
  else if (v>=128 && v<256){rf_driver.send(49,8);}
  else if (v>=256 && v<384){rf_driver.send(50,8);}
  else if (v>=384 && v<512){rf_driver.send(51,8);}
  else if (v>=512 && v<640){rf_driver.send(52,8);}
  else if (v>=640 && v<768){rf_driver.send(53,8);}
  else if (v>=768 && v<896){rf_driver.send(54,8);}
  else {rf_driver.send(55,8);}
}

void loop(){
  
  speedControl();
  readOrientation();  //reading then sending direction control signal  
  if ((LR<=-25) && (FB>-20 && FB<20)){rf_driver.send(102,8);} 
  else if ((LR>=20) && (FB>-20 && FB<20)){rf_driver.send(108,8);}
  else if ((FB<=-20) && (LR>-20 && LR<20)){rf_driver.send(114,8);}
  else if ((FB>=20) && (LR>-20 && LR<20)){rf_driver.send(98,8);}
  else {rf_driver.send(115,8);}
}

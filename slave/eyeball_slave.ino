// eyeball_slave.ino
// - Use I2C to control multiple devices.

#include <Wire.h>
#include <Servo.h> 

// slave 的眼球
#define PIN_P 2   // 眼皮
#define PIN_B 3   // 眼球

Servo servoP, servoB;
int posP, posB;

const int SLAVE_ADDRESS = 1;
char incomingByte = 0;

void setup() 
{  
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  Wire.onReceive(receiveEvent); // register event
  
  Serial.begin(9600);

  servoP.attach(PIN_P);
  servoB.attach(PIN_B);
  
  posP = posB = 1500;

  servoP.writeMicroseconds(posP);  
  servoB.writeMicroseconds(posB);
  delay(15);      
}

void loop() 
{
  // 根據傳進來的 pattern # 動作. 
  // 如果 master 一直沒傳新的值進來, 就重覆上次的動作.
  switch (incomingByte)
  {
    case 1:
      pattern1();
      break;
    case 2:
      pattern2();
      break;
    case 3:
      pattern3();
      break;
    default:
      break;
  }

  delay(1000);
}

void receiveEvent(int howMany)
{
  // master 傳來的命令格式為一數值, 代表 pattern-#.
  while (Wire.available()) 
  {
    incomingByte = Wire.read();
  }
}

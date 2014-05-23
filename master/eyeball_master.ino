// eyeball_master.ino
// - Use I2C to control multiple devices.

#include <Wire.h>
#include <Servo.h> 

// master 的眼球
#define PIN_P 2   // 眼皮
#define PIN_B 3   // 眼球

Servo servoP, servoB;
int posP, posB;

//const int SLAVE_ADDRESS = 1;

// for serial port handling 
char inData[80];
byte index = 0;
byte count = 0;

#define NO_PREFIX       0
#define S_PREFIX        1
#define P_PREFIX        1

#define TOKEN_S         "s"
#define TOKEN_P         "p"

byte pfxS = NO_PREFIX;
byte pfxP = NO_PREFIX; 
byte sAll = 0; // all slaves mode
byte sNum = 0; // slave-#
byte pNum = 0; // pattern-#

void setup() 
{  
  Wire.begin(); 
  
  Serial.begin(9600);
  //Serial.println("Type something to send:");

  servoP.attach(PIN_P);
  servoB.attach(PIN_B);
  
  posP = posB = 1500;

  servoP.writeMicroseconds(posP);  
  servoB.writeMicroseconds(posB);
  delay(15);    
}

void loop() 
{
  while (Serial.available() > 0)
  {
    char aChar = Serial.read();
    if (aChar == '\n')
    {
      // End of record detected. Time to parse
      char *p = inData; //assign the string to *p
      int counter = 0; //initialise the counter
      String str;
            
      str = strtok(p, ",");
      while (str != NULL)
      {
        Serial.println(str);
        ParseCommand(str);
        counter++;
        str = strtok(NULL, ",");
      }

      index = 0;
      inData[index] = NULL;      
      
      Serial.write("done\n");
    }
    else
    {
      inData[index] = aChar;
      index++;
      inData[index] = '\0'; // Keep the string NULL terminated
    }
  }
}

void ParseCommand(String str)
{
   // the input commands formats:
   //   * s
   //      - all: all slaves
   //      - (num): slave-#(num)   
   //   * p
   //      - (num): pattern-#(num)
   //
   // the command example:
   //   s,all,p,1 -> all slaves, run pattern-1
   //   s,5,p,3   -> slave-5, run pattern-3

   if (str == TOKEN_S && pfxS == NO_PREFIX) // 開始 s 命令
   {
      pfxS = S_PREFIX; 
   }
   else if (str == "all" && pfxS == S_PREFIX) // s 命令, all
   {                               
      sAll = 1;
   }
   else if (pfxP == NO_PREFIX && pfxS == S_PREFIX) // 仍在解析 s 命令, 尚未走到 p 命令
   {
      sNum = str.toInt();
      pfxS = NO_PREFIX; // s 命令結束
   }
   else if (str == TOKEN_P && pfxP == NO_PREFIX && pfxS == NO_PREFIX) // 開始 p 命令
   {
      pfxP = P_PREFIX;
   }
   else if (pfxP == P_PREFIX && pfxS == NO_PREFIX) // 解析 p 命令的參數
   {
      pNum = str.toInt();
      pfxP = NO_PREFIX; // p 命令結束
   }

   // 全部命令解析完成, 來操控 slave 吧
   if (sAll)
   {
      // 針對 all salves
      
      //Wire.beginTransmission(SLAVE_ADDRESS);
      //Wire.write(pNum);
      //Wire.endTransmission();
   }
   else
   {
      // 特定 salves
      Wire.beginTransmission(sNum);
   	  Wire.write(pNum);
      Wire.endTransmission();
   }

  // 不光是 slave 要做事, master 自己也要做啊.
  switch (pNum)
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
}


//pixy.blocks[i].signature; //The signature number of the detected object (1-7)
//pixy.blocks[i].x; //The x location of the center of the detected object (0 to 319)
//pixy.blocks[i].y; //The y location of the center of the detected object (0 to 199)
//pixy.blocks[i].width; //The width of the detected object (1 to 320)
//pixy.blocks[i].height; //The height of the detected object (1 to 200)
//pixy.blocks[i].print(); //A member function that prints the detected object information to the serial port

String outtxt = "";
char LF = 10;
String SN = "";
String C = ",";
String blank = SN + "1,000,000" + LF;
String Status = "1";    //status 1= object not found,  2=found
int leng = 0;
unsigned long mils = 0;
unsigned long old = 0;
uint16_t blocks;

#include <SPI.h>
#include <Pixy.h>


// This is the main Pixy object
Pixy pixy;

void setup()
{
  Serial.begin(115200); //(19200);
  pixy.init();
}

void loop() {
  blocks = pixy.getBlocks(6);
  int curBlock = 9;
  int tallest = 0;
  int widest = 0;
  if (blocks >= 1) {
    float curDist = 1000; // distance from center
    for (int l = 0; l < blocks; l++) {
      if ((pixy.blocks[l].height) > tallest) {
        tallest = pixy.blocks[l].height;
      }
      if ((pixy.blocks[l].width) > widest) {
        widest = pixy.blocks[l].width;
      }
    }
    
    for (int l = 0; l < blocks; l++) {
      float aspectRatio = float(pixy.blocks[l].height) / float(pixy.blocks[l].width);
      //      Serial.print(aspectRatio + C);
      //      Serial.print(pixy.blocks[l].x + C);

      //      Serial.print(l);
      //      Serial.print(pixy.blocks[l].height);
      //      Serial.println();
      if ((aspectRatio >= 2.0) && (aspectRatio <= 3.5) && ((pixy.blocks[l].height) > int(tallest) / 2)) {
        float distX  = abs (float(pixy.blocks[l].x) - 159.5);
        if (distX < curDist) {
          curDist = distX;
          curBlock = l;
        }
      }
    }

  }
  if (widest > 32) curBlock = 9;
  if (pixy.blocks[curBlock].width > 27) curBlock = 9;
  
  
  if (curBlock < 8) {
    Status = "2";

    old = millis();
    outtxt = Status + C + T(pixy.blocks[curBlock].x) + C + T(pixy.blocks[curBlock].height) + LF;
    // Serial.println(outtxt);

  }

  else {
    if ((millis() - old) > 45) {
      outtxt = blank;
      Status = "1";
    }
    
  }
//  if (blocks > 0) {
//    Serial.print(blocks + C);
//    Serial.print(Status + C);
//    Serial.print(curBlock + C);
//    Serial.print(millis() + C);
//    Serial.print(pixy.blocks[curBlock].x + C);
//
//    for (int l = 0; l < blocks; l++) {
//      Serial.print(l + C);
//      Serial.print(pixy.blocks[l].signature + C);
//      Serial.print(pixy.blocks[l].x + C);
//      Serial.print(pixy.blocks[l].y + C);
//      Serial.print(pixy.blocks[l].width + C);
//      Serial.print(pixy.blocks[l].height + C);
//    }
//    Serial.println();
//  }

  if (Serial.read() == 82) Serial.print(outtxt);

}

String T(int val) {
  String SN = "";
  String Tmp = SN + "00" + val;
  int len = Tmp.length();
  String Final = Tmp.substring(len - 3, len);
  return Final;

}

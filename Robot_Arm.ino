#include <Servo.h>

Servo shoulder;
Servo elbow;

float originX = -5.5;
float originY = 7.75;
byte sIn[3];

void setup()
{
  pinMode(4,OUTPUT);
  
  Serial.begin(9600);
  
  shoulder.attach(3);
  elbow.attach(5);
  goToPolar(3,90);
}

void loop()
{ 
  if(Serial.available() >= 3)
  {
    Serial.readBytes(sIn,3);

    goToBoardPos(((float)sIn[1])/10,((float)sIn[2])/10);
    delay(2000);
    digitalWrite(4,HIGH);
    delay(500);
    
    if(sIn[0] == 115)
      goToSquare();
    else if(sIn[0] == 116)
      goToTriangle();
    
    delay(2000);
    
    digitalWrite(4,LOW);
    Serial.write("G");
  }
  goToPolar(3,90);
}

//==== COORD structure ====
//rect: [X, Y]
//pol : [R, A]

void recToPol(float *coord)
{
  float temp[] = {coord[0], coord[1]};
  coord[0] = round(sqrt((temp[0]*temp[0])+(temp[1]*temp[1])));
  if(temp[0] != 0)
    coord[1] = round(4068*atan2(temp[1],temp[0])/71);
  else
    coord[1] = 90;
}

void polToRec(float *coord)
{
  float temp[] = {coord[0], coord[1]};
  coord[0] = temp[0]*cos(temp[1]);
  coord[0] = temp[0]*sin(temp[1]);
}

void goToPolar(float r, float a)
{
  
  float phi = (acos(r/18)*4068)/71;
  float eAng = constrain(180-(2*phi),0,180);
  float sAng = constrain(145-a+phi,0,180);

  Serial.print("E: ");
  Serial.print(eAng);
  Serial.print("   S: ");
  Serial.println(sAng);
  
  elbow.write(round(eAng));
  shoulder.write(round(sAng));
}

void goToBoardPos(float boardX, float boardY)
{
  float armCoords[] = {originX + boardX, originY + boardY};
  recToPol(armCoords);
  goToPolar(armCoords[0], armCoords[1]);
}

void goToTriangle()
{
  goToPolar(9,180);
}

void goToSquare()
{
  goToPolar(9,0);
}

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
/**************************/
// Define an array for holding sensor values.
#define NUM_SENSORS 6
ZumoReflectanceSensorArray reflectanceSensors;
ZumoBuzzer buzzer;
unsigned int sensorValues[NUM_SENSORS];
Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
int lastError = 0;
const int MAX_SPEED = 400;
/*************************/


void setup()
{
  /******calibrate array sensor*****/
  buzzer.play(">g32>>c32");
  reflectanceSensors.init();
  button.waitForButton();
  int i;
  
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  
  motors.setSpeeds(0,0);
  digitalWrite(13, LOW);         // turn off LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(reflectanceSensors.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  /**********end of sensor calibration********************/
  button.waitForButton();
  digitalWrite(13, HIGH);
  
  delay(5000);
  digitalWrite(13, LOW);
}

void loop()
{
  /*****display sensor value***************************/
  // read calibrated sensor values and obtain a measure of the line position.
  // Note: the values returned will be incorrect if the sensors have not been properly
  // calibrated during the calibration phase.
  
  unsigned int position = reflectanceSensors.readLine(sensorValues);

  // To get raw sensor values instead, call:  
  //reflectanceSensors.read(sensorValues);

  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.print("    ");
  Serial.println(position);
  
  delay(250);
  /*****************end of display sensor value*****************/
  
  
  /*******motor control, have NOT tested!!!!************************************/
  /* we have 4 scenario here:
   1) all sensors sensing white...
    position =0 or less than 50,
    both motor turn backward then rotate 180 degree
    
   2)sensor 1 and 2 is sensing white...
   position >3000
   rotate right about 90 degree
   
   3) sensor 5 and 6 is sensing white...
   poistion <2000
   rotate left about 90 degree
   
   4) all sensors sensing black...
  positon = 2500
  both motor move forward at same speed
  
  
  need to think of a better formula
  
  */
  
  //4)
   motors.setSpeeds(200,200);
    
 //1)
 if(position<50){
     
  for (int speed = 0; speed >= -400; speed--)
  {
    motors.setSpeeds(speed,speed);
    delay(2);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setSpeeds(speed,speed);
    delay(2);
  }
  motors.setSpeeds(200,0);
  delay(2000);
  }
  //2)
  if(position >=3000){
    motors.setSpeeds(200,0);
    delay(1000);
  }
  
  //3)
  if(position<=2000){
    motors.setSpeeds(0,200);
    delay(1000);
  }
  
  
 
    
  
  
  
}

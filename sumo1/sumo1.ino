#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

// calibaration phase 1 

#define LED_PIN 13
#define NUM_SENSORS 6

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
ZumoBuzzer buzzer;

//proto-types
//boolean moveForward();

unsigned int sensorValues[NUM_SENSORS];

void setup(){
  reflectanceSensors.init();
  pinMode(LED_PIN,OUTPUT);
  motors.flipLeftMotor(true);
  
  
  digitalWrite(LED_PIN,HIGH);
  unsigned long startTime = millis();
  while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  digitalWrite(LED_PIN,LOW);
  Serial.begin(9600);
  
  //motors.flipRightMotor(true);
  
}

void loop(){
  //go forward 
  //digitalWrite(LED_PIN,HIGH);
  digitalWrite(LED_PIN,LOW);
  for(byte i = 0;i<NUM_SENSORS;i++){
    if(sensorValues[i] >= 900){
      digitalWrite(LED_PIN,HIGH);
      motors.setSpeeds(400,400);
    }
    else{
      unsigned long time = millis();
      while(millis() - time < 100){
        //motors.setSpeeds(400,400);
        //delay(5);
        //motors.setSpeeds(-200,-200);
        //delay(5);
        motors.setSpeeds(200,-200);//on spot turn 
      }
    }
  }
}

//sumo move forward
//sumo move backward
//sumo turn right

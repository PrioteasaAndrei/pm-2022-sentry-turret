#include <Servo.h>

#define BASE 2
#define BUZZER_PIN 10 // trebuie pwm
#define echoPin 6 
#define trigPin 5 // pwm
#define MAX_RANGE 150
#define MOTION_DETECTOR 12
Servo down;
Servo up;

const int sensitivity = 40;
const int left = 20;
const int right = 50;

int curDist,prevDist;
// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int delayTime = 100;
int pasIncrementBase = 5;

int lastMotionState;    // the previous state of motion sensor
int currentMotionState; // the current state of motion sensor

unsigned int lastMotionDetected;

void setup() {
  // put your setup code here, to run once:
//  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(BASE,OUTPUT);
  digitalWrite(BASE,HIGH);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  up.attach(9);
  down.attach(3);

  up.write(0);
  down.write(20);
  lastMotionDetected = 0;
  
}

void stopMotors(){
  digitalWrite(BASE,LOW);
}

int measureDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  if(distance > MAX_RANGE)
    return MAX_RANGE;
  
  return distance;
}
void moveServoUp(){
  for(int i=0;i<=120; i+=2){
    up.write(i);
//    delay(50);
  }
  delay(200);
  for(int i=120 ; i>=0; i-=2){
    up.write(i);
//    delay(50);
  }
}


// fiddle with delay time
void motionDetector(int curDist,int prevDist){

   if(abs(curDist - prevDist) > sensitivity){

     if(millis() - lastMotionDetected > 50){
       tone(BUZZER_PIN,1000);
       moveServoUp();
       delay(200);
       noTone(BUZZER_PIN);
 
     }
    
     

     lastMotionDetected = millis();
   }
}

void loop() {


  for(int i=left;i<=right; i+=pasIncrementBase){
     down.write(i);
     delay(delayTime);
     curDist = measureDistance();
     delay(100);
     prevDist = measureDistance();

     motionDetector(curDist,prevDist);

     delay(delayTime);
  }

 
     delay(delayTime);

    for(int i=right;i>=left; i-=pasIncrementBase){
     down.write(i);
     delay(delayTime);
     curDist = measureDistance();
     delay(100);
     prevDist = measureDistance();
     motionDetector(curDist,prevDist);
     delay(delayTime);
  }

   


}

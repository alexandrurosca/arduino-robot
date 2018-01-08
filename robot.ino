 #include <Servo.h>
 #include "pitches.h"

// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11

///LightSensor
float resolutionADC = .0049 ;
// rezolutia implicita (pentru referinta 5V) = 0.049 [V] / unitate
float resolutionSensor = .01 ;
// rezolutie senzor = 0.01V/°C



int notes;
int duration;


// note durations: 4 = quarter note, 8 = eighth note, etc.:
// Master of puppets
int intro1[] = {NOTE_E3, 0, NOTE_D4, NOTE_CS4, NOTE_C4};
int intro1NoteDurations[] = {4, 2, 4, 4, 1 };
int intro1Count = 5;


int intro2[] = {NOTE_E3, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_E3, NOTE_DS4, NOTE_E3, NOTE_E3, NOTE_D4, NOTE_CS4, NOTE_C4};
int intro2NoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 2};
int intro2Count = 11;

int intro3[] = {NOTE_E3, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_E3, NOTE_AS3, NOTE_E3, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_GS3, NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_E3};
int intro3NoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,};
int intro3Count = 16;

int preverse1[] = {NOTE_E3, NOTE_F3, NOTE_B3, NOTE_E3, NOTE_F3, NOTE_C4, NOTE_E3, NOTE_F3, NOTE_CS4, NOTE_E3, NOTE_F3, NOTE_C4, NOTE_E3, NOTE_F3, NOTE_B3, NOTE_B3};
int preverse1NoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, };
int preverse1Count = 16;

int preverse2[] = {NOTE_E3, NOTE_F3, NOTE_B3, NOTE_E3, NOTE_F3, NOTE_C4, NOTE_E3, NOTE_F3, NOTE_CS4, NOTE_E3, NOTE_F3, NOTE_C4, NOTE_E3, NOTE_F3, NOTE_B3, 0};
int preverse2NoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, };
int preverse2Count =  16;

int preverse3[] = {NOTE_E3, NOTE_F3, NOTE_B3, NOTE_E3, NOTE_F3, NOTE_C4, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3, NOTE_FS3};
int preverse3NoteDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, };
int preverse3Count = 16;

int endIntro[] = {NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_DS4, NOTE_A5, NOTE_E3, NOTE_DS4, NOTE_A5, NOTE_E3, NOTE_DS4, NOTE_A5, NOTE_E3, NOTE_DS4, NOTE_A5, NOTE_E3, NOTE_DS4, NOTE_A5};
int endIntroNoteDurations[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2 };
int endIntroCount = 23;

int eightVampE[]{ NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, };
int eightVampENoteDurations[] = { 8, 8, 8, 8, 8, 8, 8, 8,  };
int eightVampECount = 8;

int verse1[] { 0, NOTE_G3, NOTE_A3, 0, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_A3};
int verse1NoteDurations[] { 8, 8, 8, 8, 8, 8, 8, 8};
int verse1Count = 8;

int verse2[] { NOTE_A3, 0, NOTE_A3, 0 };
int verse2NoteDurations[] {8, 8, 8, 8,};
int verse2Count = 4;

int eightVampFSharp[]{ NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_FS3, NOTE_FS3, };
int eightVampFSharpNoteDurations[]{ 8, 8, 8, 8, 8, 8, 8, 8, };
int eightVampFSharpCount = 8;
///end master of puppets


Servo srv;
const int trigPin = 9;
const int echoPin = 10;
long durationDistance;
int distance;
int stateMachine;


void setup() {
     // configurarea pinilor motor ca iesire, initial valoare 0
     digitalWrite(mpin00, 0);
     digitalWrite(mpin01, 0);
     digitalWrite(mpin10, 0);
     digitalWrite(mpin11, 0);
     pinMode (mpin00, OUTPUT);
     pinMode (mpin01, OUTPUT);
     pinMode (mpin10, OUTPUT);
     pinMode (mpin11, OUTPUT);

     //Sonar
     pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
     pinMode(echoPin, INPUT); // Sets the echoPin as an Input

     // pin LED
     pinMode(13, OUTPUT);


     //Serial for testing purpose
     Serial.begin(9600);
     //test2();
     //playSong();
     playWithServo(8, 90);
     stateMachine = 0;

}

// Functie de siguranta
// Executa oprire motoare, urmata de delay

// Utilizare servo
// Pozitionare in trei unghiuri
// La final, ramane in mijloc (90 grade

long distanceDetected;
long minDistance = 40;
long distanceRight;
long distanceLeft;
long constantLight = -25;
long read_light;
int speedRightLeft = 200;

void loop() {
  //doAction(1, 200);//dreapta
 // doAction(2, 200);//stanga
 //doAction(3, 200);//fata
 //doAction(4, 200);//back

  
  distanceDetected = readDistance();
  read_light = readLight(10);
  Serial.println("Light: " + read_light);
  if(read_light < constantLight){
      doAction(5,100);
      Serial.println("cleaning");
     // doAction(4,speedRightLeft);
     check();
    }
  
  if(distanceDetected < minDistance){
        check();
  }
  doAction(1, 200);
  //delay(500);


}

void check(){
  playWithServo(8, 180);
        delay(1000);
        distanceLeft = readDistance();
        playWithServo(8, 0);
        delay(1000);
        distanceRight = readDistance();
        playWithServo(8, 90);
        
        
        if(distanceLeft > distanceRight){
          doAction(4,speedRightLeft);
        }else if(distanceRight >= distanceLeft){
          doAction(3,speedRightLeft);
        }else{
          doAction(2, 200);
          doAction(2, 200);
        }
  }

long filter(long distance){
    if(distance > 3000){
        return 19;
    }
}

void doAction(int state, int speed) {
  
  switch (state) {
    case 1: 
          delay(400); 
          moveRight(speed);
      break;
    case 2:
    delay(400);
    moveLeft(speed);
      break;
    case 3:     moveForward(speed);
      break;
    case 4: 
      
      moveBackwards(speed);
      break;
    case 5: playSong();
      delay(1000);
      break;
  }
}


long readDistance() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationDistance = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= durationDistance*0.034/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    return distance;
    //return filter(distance);
}

float readLight(int count) {
  // citeste lighteratura de count ori de pe pinul analogic pin
  int pin = 0;
  float sumlight = 0;
  for(int i = 0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * resolutionADC;
    float lightCelsius = (voltage - 0.5) / resolutionSensor ;
    // scade deplasament, converteste in grade C
    sumlight = sumlight + lightCelsius;
    // suma lighteraturilor
  }
  Serial.println( sumlight / (float) count);
  return sumlight / (float)count;
  // media returnata
}

void test2() {
    moveRight(200);
    moveRight(200);
    moveLeft(200);
    moveLeft(200);
    moveBackwards(200);
    moveBackwards(200);
    moveForward(200);
    moveForward(200);
    readDistance();
    float light = readLight(30);
    Serial.println(light);
}

void playWithServo(int pin, int angle)
{
    srv.attach(pin);
    srv.write(angle);
    delay(500);
}


void  moveForward(int speed) {

    digitalWrite(mpin01, 0);
    digitalWrite(mpin11, 0);

    analogWrite (mpin00, speed);
    analogWrite (mpin10, speed);

    delay (200);
    delayStopped(200);
}


void  moveBackwards(int speed) {
    digitalWrite(mpin00, 0);
    analogWrite(mpin01, speed);
    digitalWrite(mpin10, 0);
    analogWrite(mpin11, speed);


    delay (200);
    delayStopped(200);
}

void moveLeft(int speed) {


//    digitalWrite(mpin00, 0);
//    digitalWrite(mpin11, 0);

    analogWrite(mpin01, speed);
    analogWrite (mpin10, speed);

    delay (200);
    delayStopped(200);
}

void moveRight(int speed) {

    digitalWrite(mpin10, 0);
    digitalWrite(mpin01, 0);

    analogWrite (mpin00, speed);
    analogWrite(mpin11, speed);

    delay (200);
    delayStopped(200);
}


void test() {
     // Cod avertizare
     // Blink lent
     for (int i=0; i<10; i++)
     {
         digitalWrite(13, 1);
         delay(200);
         digitalWrite(13, 0);
         delay(200);
     }
     // Blink rapid. Scoateti cablul USB!!!!
     for (int i=0; i<10; i++)
     {
         digitalWrite(13, 1);
         delay(100);
         digitalWrite(13, 0);
         delay(100);
     }
     digitalWrite(13, 1);



}

void playSong() {
  playInput(intro1, intro1NoteDurations, intro1Count);
  playInput(intro2, intro2NoteDurations, intro2Count);
  playInput(intro3, intro3NoteDurations, intro3Count);
  playInput(intro2, intro2NoteDurations, intro2Count);
  playInput(intro3, intro3NoteDurations, intro3Count);
//  playInput(preverse1, preverse1NoteDurations, preverse1Count);
//  playInput(preverse2, preverse2NoteDurations, preverse2Count);
//  playInput(preverse1, preverse1NoteDurations, preverse1Count);
//  playInput(preverse3, preverse3NoteDurations, preverse3Count);
//  playInput(endIntro, endIntroNoteDurations, endIntroCount);

  //verse 1

  // playInput(eightVampE, eightVampENoteDurations, eightVampECount);
  // playInput(verse1, verse1NoteDurations, verse1Count);
  // playInput(eightVampE, eightVampENoteDurations, eightVampECount);
  // playInput(verse2, verse2NoteDurations, verse2Count);
  // playInput(eightVampE, eightVampENoteDurations, eightVampECount);
  // playInput(verse1, verse1NoteDurations, verse1Count);
  // playInput(eightVampE, eightVampENoteDurations, eightVampECount);
  // playInput(verse2, verse2NoteDurations, verse2Count);
  //
  // playInput(eightVampFSharp, eightVampFSharp, eightVampFSharpCount);
  noTone(8);
}

void playInput(int notes[], int durations[], int count){

  int pulledCount = sizeof(durations)/sizeof(int);

  for (int thisNote = 0; thisNote < count; thisNote++) {
    int noteDuration = 1000 / durations[thisNote];
    tone(13, notes[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(13);
  }
}

// Functie pentru controlul unui motor
// Intrare: pinii m1 si m2, directia si viteze
void delayStopped(int ms)
{

     digitalWrite(mpin00, 0);
     analogWrite(mpin01, 0);
     digitalWrite(mpin10, 0);
     analogWrite(mpin11, 0);

     delay(ms);
}

int speakerPin = 13;



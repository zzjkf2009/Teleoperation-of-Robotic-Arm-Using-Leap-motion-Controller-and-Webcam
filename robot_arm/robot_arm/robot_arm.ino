#include <Servo.h>  
//string hold the income data String readstring; 
//deﬁne motors
Servo m,m1,m2,m3,m4; void setup() { 
  //start Serial connection
  Serial.begin(115200); 
m.attach(9); //x
m1.attach(10); //z
m2.attach(5); //y 
m3.attach(11);//p 
m4.attach(6);}//g 
void loop() {  //serial available and working 
while (Serial.available()) {  
//delay to get all the income 
delayMicroseconds(65); 
//if there is income data 
if (Serial.available() >0) { 
  // store character
  char c =Serial.read(); 
  //add it to the income string 
readstring += c;}} 
//if the string has value
if(readstring.length() >0) {
  String P=readstring.substring(0,readstring.indexOf(‘P’));//position “P”
  String Q=readstring.substring(readstring.indexOf(‘P')+1,readstring.indexOf('Q')); //position “Q”
  String F=readstring.substring(readstring.indexOf(‘Q')+1,readstring.indexOf('F')); //position “F”
  String K=readstring.substring(readstring.indexOf(‘F')+1,readstring.indexOf('K')); //position “K”
  String L=readstring.substring(readstring.indexOf(‘K')+1,readstring.indexOf('L')); //position “L”
  char carrayP[P.length() + 1]; //deﬁne a new 
string for “P” data char carrayQ[Q.length() + 1]; //deﬁne a new string for “Q”data 
char carrayF[F.length() + 1]; //deﬁne a new string for “F” data 
char carrayK[K.length() + 1]; //deﬁne a new string for “K” data 
char carrayL[L.length() + 1]; //deﬁne a new string for “L” data
P.toCharArray(carrayP, sizeof(carrayP)); // initialize the string “P”
Q.toCharArray(carrayQ, sizeof(carrayQ)); // initialize the string “Q”
F.toCharArray(carrayF, sizeof(carrayF)); // initialize the string “F”
K.toCharArray(carrayK, sizeof(carrayK)); // initialize the string “K”
L.toCharArray(carrayL, sizeof(carrayL)); // initialize the string “L”
int pv = atoi(carrayP); //convert from string to int to write to the motor
int qv = atoi(carrayQ); //convert from string to int to write to the motor
int fv = atoi(carrayF); //convert from string to int to write to the motor
int kv = atoi(carrayK); //convert from string to int to write to the motor
int lv = atoi(carrayL); //convert from string to int to write to the motor
m.write(pv); //write the int to motor to move
m1.write(qv); //write the int to motor to move
m2.write(fv); //write the int to motor to move
m3.write(kv); //write the int to motor to move 
m4.write(lv); //write the int to motor to move 
} readstring=""; // sign the income string to nothing
}


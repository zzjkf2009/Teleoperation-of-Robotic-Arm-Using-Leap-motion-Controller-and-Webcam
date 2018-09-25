/*  The base rotation is controlled by the x position of the hand.  The end effector is controlled by
the distance between two fingers.  Finally, the shoulder and elbow joint angles are calculated by the
inverse kinematics equation with inputs of the y and z coordinates.*/


import com.leapmotion.leap.*;
 
import processing.net.*;
 
Client arm; // deﬁne a tcp client
 
Controller leap; //deﬁne the leap motion controller
 
boolean work=false; // boolean for case the hand and the ﬁnger is detected 
void setup()
{
 
size(250, 250); //set the app window size
 
leap = new Controller(); // initialize leap motion controller
 
arm = new Client(this, "192.168.0.1", 55555); //start the client connection ip + port   the IP was found through the leap motion device 
 
}
 /* There are four servos in the arm, and three separate main controls.  The base rotation is controlled
 by the x position of the hand.  The end effector is controlled by the distance between two fingers. 
 Finally, the shoulder and elbow joint angles are calculated by the inverse kinematics equation with
 inputs of the y and z coordinates. */
 //base motor formula
 double cba(double a)  //calculate base abgle 
 {
 
float n = 100*3;     /* 3 is normalizer   While the Leap Motion device measures coordinates linearly, the base rotation is circular. 
I wanted to feed the x value through a cosine function so that it would not be too sensitive in midranged values. In order to calculate the base angle, 
I needed to get its range of values that would jibe well with a cosine function, and since the Leap Motion’s vales are in the range of -300 to 300 in Leap Space, 
I divided by 100 times the normalize value*/
 
a = 1.5+2*a/n;   
 
double angle = 90+Math.cos(a)*90;
 
return angle;}


double inverseKt1(float y,float z,int l1,int l2){
  double t1=Math.acos((sqrt(z)+sqrt(y)-sqrt(l1)-sqrt(l2))/(2*l1*l2));
  return
  t1;
    
}

double inverseKt2(float y,float z,int l1,int l2){
  double t1=Math.acos((sqrt(z)+sqrt(y)-sqrt(l1)-sqrt(l2))/(2*l1*l2));
  double t2=Math.asin(((l1+l2*Math.cos(t1))*y-l2*Math.sin(t1)*z)/(sqrt(l1)+sqrt(l2)+2*l1*l2*Math.cos(t1)));
  return 
  t2;
    
}

 
 
void draw()
 
{
 
 //leap motion
 
HandList hands = leap.frame().hands();
 
Hand hand = hands.get(0);
 
FingerList fingers = hand.fingers();
 
Vector hp = hand.palmPosition();
 
Pointable f1=fingers.get(0);
 
Pointable f2=fingers.get(1);

 
float ff1=f1.tipPosition().getX();
 
float ff2=f2.tipPosition().getX();
 
float sub=ff1-ff2; // get the distance between the two ﬁngers
float pitch = hand.direction().pitch()* 100; // get pitch  //set maximum and minimum values for the controller range
 
if(hp.getY()<150) hp.setY(150);
 
if(hp.getY()>445) hp.setY(445);
 
if(hp.getZ()<-180) hp.setZ(-180);
 
if(hp.getZ()>180) hp.setZ(180);
 
float zv=map(hp.getZ(),-180,180,160,8); // map z
 
float yv=map(hp.getY(),150,445,0,179); //map y

int l1=40;
int l2=40;
 
double xv=180-cba(-hand.palmPosition().getX()/1.5); // get x  moveBase
//int moveBase=(int)xv;
double t1=inverseKt1(yv,zv,l1,l2);   // t1 should be theta2, which is the elbow angle 
//int moveShoulder=(int)t1;
double t2=inverseKt2(yv,zv,l1,l2);   // t2 is theta1, which is the sholder angle
//int moveElbow=(int)t2;
//moveShoulder = toDegrees(angles.theta1); ?
//moveElbow = 45+toDegrees(angles.theta2);  ?

 
float pv=map(pitch,-90,100,160,6); // map pitch
//int moveWrist=(int)pv;
 
float gv=map(sub,20,90,145,73); //map ﬁngers distance
//int moveEnd=(int)gv;
 
if(fingers.count()>=2){work=true;} // set the case
 
else{work=false;}
 
 //if in range
 
if(work&&zv<=180&&zv>=0&&yv<=150&&yv>=0&&xv<=180&&xv>=0&&pv<=160&&pv>=6&&gv<=145&&gv>=73){
 
String moveBase=(int)xv+"P";
 
String moveElbow=(int)t1+"Q";
//String moveShoulder=(int)t1+"Q";
 
String moveShoulder=(int)t2+"F";
//String moveElbow=(int)t2+"F";
 
String moveWrist=(int)pv+"K";
 
String moveEnd=(int)gv+"L";
 
arm.write(moveBase+moveShoulder+moveElbow+moveEnd+moveWrist); // write to the MCU
 
}  //leap motion
 
background(0);  // box background color
  fill(255); // text color
  textSize(height/3); // text size
  text(yv, 90, 160); // text value and position on the box
}
 


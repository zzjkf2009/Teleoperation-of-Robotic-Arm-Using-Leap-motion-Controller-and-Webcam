import processing.video.*;
//import javax.swing.*;
Capture cam; 
PImage source;
PImage destination;

void setup() { 
   //println(Capture.list());
  //352,288,30;
  //640 480 30;

  size(640,480);
  cam = new Capture(this, 640, 480, 30);
  cam.start();

 // source = loadImage("output.jpg");
 // destination = createImage(source.width, source.height, RGB);
  //size(source.width, source.height); 
  
  
} 

void captureEvent(Capture cam) {
  cam.read();

}

void draw(){
  image(cam,0,0);
}

void mousePressed() {
    saveFrame("output2.jpg");
}



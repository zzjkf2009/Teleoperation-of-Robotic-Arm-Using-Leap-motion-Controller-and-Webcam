import gab.opencv.*;
import processing.video.*;
import java.awt.*;
OpenCV opencv;
Capture cam; 
PImage source;
PImage destination;

void setup() { 
  cam = new Capture(this, 352, 288, 30);
  cam.start();
  //saveFrame("out.jpg");
  source = loadImage("output.jpg");
  destination = createImage(source.width, source.height, RGB);
  //size(source.width, source.height); 
  size(352, 288);
  background(0);


} 

void draw() { 


  float threshold = 100;

  source.loadPixels();
  destination.loadPixels();

  for (int x = 0; x < source.width; x++) {
    for (int y = 0; y < source.height; y++ ) {
      int loc = x + y*source.width;
      // Test the brightness against the threshold
      if (brightness(source.pixels[loc]) > threshold) {
        destination.pixels[loc]  = color(255);  // White
      } else {
        destination.pixels[loc]  = color(0);    // Black
      }
    }
  }

  // We changed the pixels in destination
  destination.updatePixels();
  // Display the destination

  //image(destination, 0, 0);
  image(cam,0,0);
} 

void captureEvent(Capture cam) {
  cam.read();

}





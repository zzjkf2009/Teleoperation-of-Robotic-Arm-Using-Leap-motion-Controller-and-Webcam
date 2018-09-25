import processing.video.*;
PImage destinationRED;
PImage destinationGREEN;
PImage destinationBLUE;
int RED_x, RED_y, GREEN_x, GREEN_y, BLUE_x, BLUE_y;


// Variable for capture device
Capture video;

// A variable for the color we are searching for.
//color trackColor; 

void setup() {
// println(Capture.list());
  RED_x= RED_y= GREEN_x= GREEN_y= BLUE_x= BLUE_y=0;
  size(640,480);
  video = new Capture(this,width,height,30);
  video.start();
  destinationRED = createImage(video.width, video.height, RGB);
  destinationGREEN = createImage(video.width, video.height, RGB);
  destinationBLUE = createImage(video.width, video.height, RGB);
  // Start off tracking for red
 // trackColor = color(208,16,61);
  smooth();
}

void draw() {
  
  // Capture and display the video
 
    video.read();
    image(video,0,0);
    videoprocessing();
    centerOfMass(destinationRED.pixels, destinationRED.width, destinationRED.height,'r');
    centerOfMass(destinationGREEN.pixels, destinationGREEN.width, destinationGREEN.height,'g');
    centerOfMass(destinationBLUE.pixels, destinationBLUE.width, destinationBLUE.height,'b');
    if ((RED_x != 0)&&(RED_y != 0)){
      ellipse(RED_x,RED_y,5,5);
      textSize(20);
      text("RED", RED_x-5,RED_y-5);
    }
    if ((GREEN_x != 0)&&(GREEN_y != 0)){
      ellipse(GREEN_x,GREEN_y,5,5);
      textSize(20);
      text("GREEN", GREEN_x-5,GREEN_y-5);
    }
    if ((BLUE_x != 0)&&(BLUE_y != 0)){
      ellipse(BLUE_x,BLUE_y,5,5);
      textSize(20);
      text("BLUE", BLUE_x-5,BLUE_y-5);
    }
}

void videoprocessing(){
  
   video.loadPixels();
   destinationRED.loadPixels();
   destinationGREEN.loadPixels();
   destinationBLUE.loadPixels();

  // Before we begin searching, the "world record" for closest color is set to a high number that is easy for the first pixel to beat.
 // float worldRecord = 500; 

  // XY coordinate of closest color
  int closestX = 0;
  int closestY = 0;

  // Begin loop to walk through every pixel
  for (int x = 0; x < video.width; x ++ ) {
    for (int y = 0; y < video.height; y ++ ) {
      int loc = x + y*video.width;
      // What is current color
      color currentColor = video.pixels[loc];
      float r1 = red(currentColor);
      float g1 = green(currentColor);
      float b1 = blue(currentColor);
    //  float r2 = red(trackColor);
    //  float g2 = green(trackColor);
     // float b2 = blue(trackColor);
     /////////////RED/////////////////
    if (r1>(g1+50)&&r1>(b1+50)){
      destinationRED.pixels[loc]  = color(255); //white
    }
    else{
       destinationRED.pixels[loc]  = color(0); //black
    }
    ///////////////////GREEN/////////////////////
    if (g1>(r1+50)&&g1>(b1+50)){
      destinationGREEN.pixels[loc]  = color(255); //white
    }
    else{
       destinationGREEN.pixels[loc]  = color(0); //black
    }
///////////////////BLUE///////////////////////
if (b1>(g1+50)&&b1>(r1+50)){
      destinationBLUE.pixels[loc]  = color(255); //white
    }
    else{
       destinationBLUE.pixels[loc]  = color(0); //black
    }
    }
  }
   
}

void centerOfMass (int[] image, int imageWidth, int imageHeight, char c){          //c for color
  int SumX = 0;
  int SumY = 0;
  int num  = 0; 
  for (int i=0; i<imageWidth; i++)
  {
    for (int j=0; j<imageHeight; j++)
    {
      int loc = i + j*imageWidth;
      if (image[loc] == color(255))    //black
      {
        SumX = SumX + i;
        SumY = SumY + j; 
        num = num+1;
      }
    }
  }
  // println(imageWidth, " ", imageHeight);
  if (num == 0) return;
  SumX = SumX / num;
  SumY = SumY / num;
  switch (c) {
    case 'r': RED_x = SumX;
              RED_y = SumY;
              break;
    case 'g': GREEN_x = SumX;
              GREEN_y = SumY;
              break;
    case 'b': BLUE_x = SumX;
              BLUE_y = SumY;
              break;
    default: break;
  }
  //println("Center of mass is (", SumX, ", ", SumY, ")" );
  // The coordinate (SumX,SumY) is the center of the image mass
}
//////////////////////////////////////////////////////////////////


     /* // Using euclidean distance to compare colors
      float d = dist(r1,g1,b1,r2,g2,b2); // We are using the dist( ) function to compare the current color with the color we are tracking.

      // If current color is more similar to tracked color than
      // closest color, save current location and current difference
      if (d < worldRecord) {
        worldRecord = d;
        closestX = x;
        closestY = y;
      }
    }
  }

  // We only consider the color found if its color distance is less than 10. 
  // This threshold of 10 is arbitrary and you can adjust this number depending on how accurate you require the tracking to be.
  if (worldRecord < 30) { 
    // Draw a circle at the tracked pixel
    fill(trackColor);
    strokeWeight(4.0);
    stroke(0);
    ellipse(closestX,closestY,16,16);
  }
}

void mousePressed() {
  // Save color where the mouse is clicked in trackColor variable
  //int loc = mouseX + mouseY*video.width;
 // trackColor = video.pixels[loc];
 saveFrame("testimage.jpg");
}*/

import processing.video.*;
PImage source;
PImage destination;
int red_threshold=200;
int green_threshold=125;
int blue_threshold=100;

void setup(){
 source = loadImage("out2.jpg");
 //source.resize(source.width/4,source.height/4);
 size(source.width,source.height);
 destination = createImage(source.width, source.height, RGB);
}

void draw(){
  background(source);
  color c = source.get(743,594);
  println(red(c)," ",green(c)," ",blue(c));
  //saveFrame("out2.jpg");
  if (keyPressed){
    if(key=='r'){
      improcessingRED();
      centerOfMass(destination.pixels, destination.width, destination.height);
    }
    if(key=='b'){
      improcessingBLUE();
      centerOfMass(destination.pixels, destination.width, destination.height);
    }
    if(key=='g'){
      improcessingGREEN();
      centerOfMass(destination.pixels, destination.width, destination.height);
    }
  }
  image(destination,0,0);
  
  
}


/////////////////////////////////////////////////////RED/////////////////////////////////////////////
void improcessingRED(){
   source.loadPixels();
  destination.loadPixels();

  for (int x = 0; x < source.width; x++) {
    for (int y = 0; y < source.height; y++ ) {
      int loc = x + y*source.width;
      // Test the brightness against the threshold
      if ((red(source.pixels[loc]) >= (red_threshold)) && (abs(green(source.pixels[loc])-(blue(source.pixels[loc]))) <= 50)) {
        destination.pixels[loc]  = color(255);  // White
      } else {
        destination.pixels[loc]  = color(0);    // Black
      }
    }
  }
  
  // We changed the pixels in destination
  destination.updatePixels();
  // Display the destination
}
///////////////////////////////////////////////////////GREEN////////////////////////////////////////////////
void improcessingGREEN(){
   source.loadPixels();
  destination.loadPixels();
  for (int x = 0; x < source.width; x++) {
    for (int y = 0; y < source.height; y++ ) {
      int loc = x + y*source.width;
      // Test the brightness against the threshold
      if ((green(source.pixels[loc]) >= (green_threshold)) && (abs(blue(source.pixels[loc])-(red(source.pixels[loc]))) <= 50) && (red(source.pixels[loc])<green(source.pixels[loc]))) {
        destination.pixels[loc]  = color(255);  // White
      } else {
        destination.pixels[loc]  = color(0);    // Black
      }
    }
  }
  // We changed the pixels in destination
  destination.updatePixels();
  // Display the destination
}

/////////////////////////////////////////////////////////////////////////////BLUE/////////////////////////////
void improcessingBLUE(){
   source.loadPixels();
  destination.loadPixels();

  for (int x = 0; x < source.width; x++) {
    for (int y = 0; y < source.height; y++ ) {
      int loc = x + y*source.width;
      // Test the brightness against the threshold
      if ((blue(source.pixels[loc]) >= (blue_threshold)) && (abs(green(source.pixels[loc])-(red(source.pixels[loc]))) <= 150)) {
        destination.pixels[loc]  = color(255);  // White
      } else {
        destination.pixels[loc]  = color(0);    // Black
      }
    }
  }
  // We changed the pixels in destination
  destination.updatePixels();
  // Display the destination
}





void centerOfMass (int[] image, int imageWidth, int imageHeight){
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
  SumX = SumX / num;
  SumY = SumY / num;
  println("Center of mass is (", SumX, ", ", SumY, ")" );
  // The coordinate (SumX,SumY) is the center of the image mass
}

/*
  Include the used libraries
*/
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
  Defines
*/
#define DISCPIN 9
#define SLIDEPIN 10
#define BUTTONPIN 2
#define COLOR_S0 4
#define COLOR_S1 5
#define COLOR_S2 6
#define COLOR_S3 7
#define COLOR_OUT 8

/*
  The variables used to move the two servos
*/
Servo discServo, slideServo;
int moveDisc = 0;
int moveSlide = 0;

/*
  Variables for color sensor PW, Calibration Values
*/
int redMin = 22; // Red minimum value
int redMax = 76; // Red maximum value
int greenMin = 26; // Green minimum value
int greenMax = 100; // Green maximum value
int blueMin = 8; // Blue minimum value
int blueMax = 30; // Blue maximum value

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

/*
  Set the LCD to display number and set the outputs
*/
LiquidCrystal_I2C lcd(0x27,16, 2);

const char *redColor = "RED - CHERRY";
const char *greenColor = "GREEN - LIME";
const char *purpleColor = "PURPLE - GRAPE";
const char *yellowColor = "YELLOW - LEMON";
const char *placeCandy = "Place Candy";
int numCandy = 0;

void setup() {
  // Setup Serial Monitor
  Serial.begin(9600);

  // Set the pins for the servos
  discServo.attach(DISCPIN);
  slideServo.attach(SLIDEPIN);

  // Set up the color sensor's pins
  // Set S0 - S3 as outputs
  pinMode(COLOR_S0, OUTPUT);
  pinMode(COLOR_S1, OUTPUT);
  pinMode(COLOR_S2, OUTPUT);
  pinMode(COLOR_S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(COLOR_OUT, INPUT);
  
  // Set Pulse Width scaling to 20%
  digitalWrite(COLOR_S0,HIGH);
  digitalWrite(COLOR_S1,LOW);

  // Set the lcd
  lcd.init();
  lcd.clear();         
  lcd.backlight();

  // Set up the button conditions
  pinMode(BUTTONPIN, INPUT_PULLUP);
}

void loop() {
  // Set the servo variables to initial position
  moveDisc = 0;
  moveSlide = 90;

  moveDiscServo(moveDisc);   // Move disc server to initial position
  moveSlideServo(moveSlide); // Position the slide servo in the initial position

  // Write on the lcd place candy
  writePlaceCandy();

  // Wait to place the candy
  while (digitalRead(BUTTONPIN) == HIGH) {
    // Wait until the button is pressed
  }

  // Clar the place candy output from the lcd
  lcd.clear();
  delay(100);

  // Slowly move disc server to color sensor
  for (int i = 0; i < 90; i++) {
    moveDisc = moveDisc + 1;
    moveDiscServo(moveDisc);
    delay(10);
  }
  delay(4000);

  // Deduce the color of the candy
  findColor();

  moveSlideServo(moveSlide);  // Move the slide under the correct cup
  numCandy = numCandy + 1;    // Increase the number of candies sorted
  writeOutput();              // Write on the LCD the type of candy and number of candies

  // Move sensor to the slide
  for (int i = 0; i < 90; i++) {
    moveDisc = moveDisc + 1;
    moveDiscServo(moveDisc);
    delay(20);
  }
  delay(4000);
}


/*
  Functions to move the servo motors
*/
void moveDiscServo(int degrees) {
  int servoPosition = map(degrees, 0, 180, 0, 180);
  discServo.write(servoPosition);
}

void moveSlideServo(int degrees) {
  int servoPosition = map(degrees, 0, 180, 0, 180);
  slideServo.write(servoPosition);
}

/*
  Function to figure out the color of the candy
*/
void findColor() {
  int maxim = -1;
  int redFlag = 0;
  int greenFlag = 0;
  int blueFlag = 0;

  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  
  Serial.print(" - Blue = ");
  Serial.println(blueValue);

  int red = redValue;
  int green = greenValue;
  int blue = blueValue;

  maxim = red;
  if (maxim < green) {
    maxim = green;
  }

  if(maxim < blue) {
    maxim = blue;
  }

  if (red > 250 && green > 250 && blue > 250) {
    Serial.println("YELLOW");
    moveSlide = 63;
  } else {
    if(maxim == red) {
      Serial.println("RED");
      moveSlide = 90;
    } else {
      if (maxim == green) {
        Serial.println("GREEN");
        moveSlide = 120;
      } else {
        Serial.println("PURPLE");
        moveSlide = 60;
      }
    }
  }
}

// Function to read Red Pulse Widths
int getRedPW() {
  // Set sensor to read Red only
  digitalWrite(COLOR_S2,LOW);
  digitalWrite(COLOR_S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(COLOR_OUT, LOW);
  // Return the value
  return PW;
}
 
// Function to read Green Pulse Widths
int getGreenPW() {
  // Set sensor to read Green only
  digitalWrite(COLOR_S2,HIGH);
  digitalWrite(COLOR_S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(COLOR_OUT, LOW);
  // Return the value
  return PW;
}
 
// Function to read Blue Pulse Widths
int getBluePW() {
  // Set sensor to read Blue only
  digitalWrite(COLOR_S2,LOW);
  digitalWrite(COLOR_S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(COLOR_OUT, LOW);
  // Return the value
  return PW;
}

/*
  Function to write the output on the LCD
*/
void writePlaceCandy() {
  lcd.clear();
  lcd.setCursor(0, 0);

  for (int i = 0; i < strlen(placeCandy); i++) {
    lcd.setCursor(i, 0);
    lcd.print(placeCandy[i]); 
  }
}

void writeOutput() {
  lcd.clear();   // Clear the previous output

  lcd.setCursor(0, 0);
  switch(moveSlide) {
    case 60:
      for (int i = 0; i < strlen(purpleColor); i++) {
          lcd.setCursor(i, 0);
          lcd.print(purpleColor[i]); 
        }
      break;
    case 120:
      for (int i = 0; i < strlen(greenColor); i++) {
          lcd.setCursor(i, 0);
          lcd.print(greenColor[i]); 
      }
      break;
    case 90:
      for (int i = 0; i < strlen(redColor); i++) {
          lcd.setCursor(i, 0);
          lcd.print(redColor[i]); 
      }
      break;
    case 63:
      for (int i = 0; i < strlen(yellowColor); i++) {
          lcd.setCursor(i, 0);
          lcd.print(yellowColor[i]); 
      }
      break;
  }

  // Print number of candy
  char buffer[16]; // Create a character buffer to store the converted string
  itoa(numCandy, buffer, 10); // Convert the integer to a string
  const char* stringNumCandy = buffer;
  for (int i = 0; i < strlen(stringNumCandy); i++) {
    lcd.setCursor(i, 1);
    lcd.print(stringNumCandy[i]); 
  }
}

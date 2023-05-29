#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Calibration Values
// Get these from Calibration Sketch

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

void setup() {

  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop() {
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

  if (red < 100 && green < 100 && blue < 100) {
    Serial.println("BLUE");
  } else {
    if (red > 250 && green > 250 && blue > 250) {
      Serial.println("YELLOW");
    } else {
      if(maxim == red) {
        Serial.println("RED");
      } else {
        if (maxim == green) {
          Serial.println("GREEN");
        } else {
          Serial.println("BLUE");
        }
      }
    }
  }
}


// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}
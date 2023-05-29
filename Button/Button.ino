volatile bool isInterrupted = false;

#define BUTTON 2

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleInterrupt, CHANGE);
}

void loop() {
  if (!isInterrupted) {
    // Run the program as normal
  }
}

void handleInterrupt() {
  if (digitalRead(BUTTON) == LOW) {
    isInterrupted = !isInterrupted;
  }
}

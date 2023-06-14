// Pin number for input
const int inputPin1 = 12;
const int inputPin2 = 13;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set the input pin as INPUT
  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);
  pinMode(7, OUTPUT);
}
void loop() {
  // Read the state of the input pin
  analogWrite(0,255);
  int inputState1 = digitalRead(inputPin1);
  int inputState2 = digitalRead(inputPin2);
  
  
  // Check if the input pin is HIGH
  if (inputState1 == HIGH) {
    // Output a message
    Serial.println("Down");
  }
   else if (inputState2 == HIGH) {
     Serial.println("Up");
   }
    else {
      Serial.println("No magnetic field detected.");
    }
  
  // Delay for a short period
  analogWrite(0,0);
  delay(1500); 
}
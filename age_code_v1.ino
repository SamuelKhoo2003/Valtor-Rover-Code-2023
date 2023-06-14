const int inputPin = 4;  // Pin connected to the pulse input
unsigned long period = 0;  // Variable to store the pulse period
unsigned long age;
unsigned long sum;
void setup() {
  pinMode(4,INPUT);
  Serial.begin(9600); 
}
void loop() {
  age = 0;
  sum = 0;
  // Wait for the pulse to start (assuming it starts at LOW)
  while (digitalRead(inputPin) == HIGH) {}
 
  // Measure the pulse duration
  for( int i = 0; i < 100; i++){
    unsigned long ontime = pulseIn(inputPin, HIGH);
    unsigned long offtime = pulseIn(inputPin,LOW);
    unsigned long pulseDuration = ontime + offtime;
    sum = sum + pulseDuration;
  }

  // Calculate the period in microseconds
  period = sum / 100;
  age = period / 10;
  // Print the period in microseconds
  Serial.print("Age ");
  Serial.print(age);
  Serial.println(" years");
 // Serial.print("Age is ", 100*period, );
  // Delay for a short interval
  delay(1000);
}
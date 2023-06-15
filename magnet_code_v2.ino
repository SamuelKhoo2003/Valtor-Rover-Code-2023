const int forward_hall = A3;
const int reverse_hall = A5; 
const int magnet_pin1 = 12;
const int magnet_pin2 = 13; 

//initially the stats for the both the forward and reverse should be 0
double forward_state = 0; 
double reverse_state = 0; 
double state_difference = 0; 

void setup() {
  pinMode(magnet_pin1, OUTPUT); 
  pinMode(magnet_pin2, OUTPUT); 
  pinMode(forward_hall, INPUT); 
  pinMode(reverse_hall, INPUT); 
  Serial.begin(115200); 
}

void loop() {
  forward_state = analogRead(forward_hall); 
  reverse_state = analogRead(reverse_hall); 
  state_difference = 20 * (forward_state - reverse_state); 

  if(state_difference > 150) {
    Serial.println("south"); 
  }

  else if(state_difference < 30) {
    Serial.println("north"); 
  }

  else {
    Serial.println("N/A"); 
  }

}



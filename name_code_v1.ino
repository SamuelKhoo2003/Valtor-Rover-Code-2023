void setup() {
  Serial.begin(115200);
  pinMode(8, INPUT);
}

void loop() {
  char temp[4] = {0, 0, 0, 0}; //initialise an array to store the characters

  while (digitalRead(8)) {} //while the signal is low do nothing (i.e. when there is no name to read)
  
  delayMicroseconds(1200000 / 600); //when the start bit is detected, apply a delay before reading the character
  for (int i = 0; i < 8; i++) {
    temp[0] |= (digitalRead(3) << i); //each time we read a bit we shift it left by i (because each character is written in reverse order)
    delayMicroseconds(1000000 / 600); //this delay is to allow a suitable transition from bit to bit before attempting to read each bit
  }
  
  delayMicroseconds(2000000 / 600);
  for (int i = 0; i < 8; i++) {
    temp[1] |= (digitalRead(8) << i);
    delayMicroseconds(1000000 / 600);
  }

  delayMicroseconds(2000000 / 600);
  for (int i = 0; i < 8; i++) {
    temp[2] |= (digitalRead(8) << i);
    delayMicroseconds(1000000 / 600);
  }

  delayMicroseconds(2000000 / 600);
  for (int i = 0; i < 8; i++) {
    temp[3] |= (digitalRead(8) << i);
    delayMicroseconds(1000000 / 600);
  }

  delayMicroseconds(1000000 / 600);

  //print the name
  Serial.print(temp[0]);
  Serial.print(temp[1]);
  Serial.print(temp[2]);
  Serial.println(temp[3]);
}
int channel[] = {25, 26, 27};
int dataPin = 35; 
//the output pin – mux input
int outputPin[] = {23, 18,  5, 19, 22, 16, 21, 17};
//   inputMUX[] = { 0,  1,  2,  3,  4,  5,  6,  7};

void setup(){
  Serial.begin(115200);
  pinMode(dataPin, INPUT);
  pinMode(outputPin[0], OUTPUT);
  pinMode(outputPin[1], OUTPUT);
  pinMode(outputPin[2], OUTPUT);
  pinMode(outputPin[3], OUTPUT);
  pinMode(outputPin[4], OUTPUT);
  pinMode(outputPin[5], OUTPUT);
  pinMode(outputPin[6], OUTPUT);
  pinMode(outputPin[7], OUTPUT);
  pinMode(channel[0], OUTPUT);
  pinMode(channel[1], OUTPUT);
  pinMode(channel[2], OUTPUT);

  // init all to 0
  digitalWrite(outputPin[0], LOW);
  digitalWrite(outputPin[1], LOW);
  digitalWrite(outputPin[2], LOW);
  digitalWrite(outputPin[3], LOW);
  digitalWrite(outputPin[4], LOW);
  digitalWrite(outputPin[5], LOW);
  digitalWrite(outputPin[6], LOW);
  digitalWrite(outputPin[7], LOW);
  delay(100);

  // draw pattern 001
  //              011
  //              111
  for (int l=0; l<8; l++){
    digitalWrite(outputPin[l], HIGH);
    for (int i=1 ; i>-1; i--){
      for (int j=1 ; j>-1; j--){
        for (int k=1 ; k>-1; k--){
          digitalWrite(channel[2], k);
          digitalWrite(channel[1], j);
          digitalWrite(channel[0], i);
          delay(50);
          Serial.print(i);
          Serial.print(j);
          Serial.print(k);
          Serial.print(" : ");
          Serial.print(digitalRead(dataPin));
          delay(50);
          Serial.print("  ");
        }
      }
    }
    Serial.println("");
  }
}

void loop(){
}
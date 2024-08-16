#define LED_R 14
#define LED_G 12

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  analogWrite(LED_R, 100);
  analogWrite(LED_G, 100);
}

void loop() {
  
}
#define BUTTON_PIN_1 25
#define BUTTON_PIN_2 26
#define BUTTON_PIN_3 23

void setup(){
  Serial.begin(115200);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
}

void loop(){
    Serial.print("TRG 1 ");
    Serial.println(digitalRead(BUTTON_PIN_1));
    Serial.print("TRG 2 ");
    Serial.println(digitalRead(BUTTON_PIN_2));
    Serial.print("TRG 3 ");
    Serial.println(digitalRead(BUTTON_PIN_3));
    delay(100);
}

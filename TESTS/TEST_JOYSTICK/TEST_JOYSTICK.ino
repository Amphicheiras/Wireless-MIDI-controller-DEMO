#define VRX_PIN  17
#define VRY_PIN  5

int valueX, valueY = 0;

void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.print(analogRead(VRX_PIN));
  Serial.println(analogRead(VRY_PIN));
}
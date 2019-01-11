#define DATA_OUT 6 //LED pin

void setup() {
  pinMode(DATA_OUT, OUTPUT);
}

void loop() {
  digitalWrite(DATA_OUT, HIGH);
  delay(200);
  digitalWrite(DATA_OUT, LOW);
  delay(200);
}

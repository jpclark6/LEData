#define LIGHT_SENSOR A2

void setup() {
  Serial.begin(2000000);
}

void loop() {
  long light_reading = 0;
  int readings = 0;
  while(readings < 1000) {
    unsigned long microTime = micros();
    readings += 1;
    light_reading = analogRead(LIGHT_SENSOR);
    Serial.print(readings);
    Serial.print('\t');
    Serial.println(light_reading);
    while(micros() - microTime < 1000){}
  }
  delay(10000);
}

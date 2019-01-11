unsigned int clk = 1000;  //in us
#define DATA_OUT 9        //LED pin

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  pinMode(DATA_OUT, OUTPUT);
}

void loop() {
// UNCOMMENT TO USE WITH SERIAL FROM COMPUTER FOR CUSTOM TEXT
//  Serial.println("Enter clock speed in microseconds");
//  while(Serial.available() == 0) {
//    delay(10);
//  }
//  if (Serial.available() > 0) {
//    String tempClk = Serial.readString();
//    clk = tempClk.toInt();
//  }
//  String textData = "";
//  Serial.println("Type text to send...");
//  while(Serial.available() == 0) {
//    delay(10);
//  }
//  if (Serial.available() > 0) {
//    textData = Serial.readString();
//    sendData(textData);
//  }
////////////////////////////////////////////////////////////////
// COMMENT TO USE WITH SERIAL FROM COMPUTER
  String textData = "Looping test at 200  us. Appx 5 KHz clock, appx. 3 kbit/s data transfer. This much  text will take appx. 0.24 seconds to transfer.";
  clk = 200;
  sendData(textData);

  delay(12000);
}

void sendData(String text) {
  Serial.print("Clock speed set at ");
  Serial.print(clk);
  Serial.println(" us");
  Serial.print("Sending text: ");
  Serial.println(text);
  unsigned long startTime = millis();
  unsigned int stringLength = text.length();

  //initialize slave clock
  for(int i = 0; i < 4; i++) {
    digitalWrite(DATA_OUT, HIGH);
    delayMicroseconds(clk);
    digitalWrite(DATA_OUT, LOW);
    delayMicroseconds(clk);
  }
  
  for (int i = 0; i < stringLength; i++) {
    char nextChar = text.charAt(i);
    sendChar(nextChar);
  }

  digitalWrite(DATA_OUT, LOW); //turn off LED if ending bit was high
  Serial.print("Send completed in ");
  long totTime = millis() - startTime;
  float totTimeDec = float(totTime) / 1000;
  Serial.print(totTimeDec);
  Serial.println(" seconds");
  Serial.println();
}

void sendChar(char Byte) {
  digitalWrite(DATA_OUT, LOW);
  delayMicroseconds(clk);
  digitalWrite(DATA_OUT, HIGH);
  delayMicroseconds(clk);

  for (int i = 0; i < 7; i++) {
    int x = bitRead(Byte, i);
    digitalWrite(DATA_OUT, x);
    delayMicroseconds(clk);
  }
}

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

//////////////////////////
#define FASTADC 1
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
////////////////////////////

int minLight = 200;
int debounce = 0; //debounce in us, change to higher value if having problems

#define LIGHT_SENSOR A0

///Screen Setup ///////////////////////
// You can use any (4 or) 5 pins 
#define sclk 2
#define mosi 3
#define dc   4
#define cs   5
#define rst  6

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);

////////////////////////////////////////

int calc_delay = 3; //experiment around with to see if you can improve timing with delay


void setup() {
  #if FASTADC
   // set prescale to 16
   sbi(ADCSRA,ADPS2) ;
   cbi(ADCSRA,ADPS1) ;
   cbi(ADCSRA,ADPS0) ;
  #endif
  
  Serial.begin(9600);
  Serial.println("Setup to receive text");
  tft.begin();
  tft.fillScreen(BLACK);
}

void loop() {
  String textData = "";
  Serial.println("Waiting... ");

  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(WHITE);  
  tft.setTextSize(2);
  tft.println("Waiting...");

  unsigned long clkStart[4];

  for(int i = 0; i < 4; i++) {
    while (analogRead(LIGHT_SENSOR) < minLight) { //wait until data starts
      delayMicroseconds(4);
    }
    clkStart[i] = micros();

    delayMicroseconds(debounce);
    while (analogRead(LIGHT_SENSOR) > minLight) { //wait until light falls
      delayMicroseconds(3);
    }
  }

  unsigned long clk = ((clkStart[1] - clkStart[0]) + (clkStart[2] - clkStart[1]) + (clkStart[3] - clkStart[2])) / 6 - calc_delay;
  delayMicroseconds(clk);
  
  textData = receiveData(clk);
  
  Serial.println("Text file:");
  Serial.println(textData);
  Serial.println();
  Serial.println();

  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(WHITE);  
  tft.setTextSize(1);
  tft.println(textData);
  tft.setCursor(0, 100);
  tft.setTextColor(BLUE);  
  tft.setTextSize(1);
  tft.println("Calc. clock speed: ");
  tft.print(clk);
  tft.println(" us");

  delay(5000);

}

String receiveData(long clk) {
  char x;
  String text = "";
  boolean dataAvailable = true;

  while (dataAvailable == true) {
    delayMicroseconds(clk / 2 - calc_delay); //move past start to avoid false readings
    unsigned long timeCheck = micros();
    long I = 0;
    while((analogRead(LIGHT_SENSOR) < minLight) && (I < 64000)) {
      I++;
      delayMicroseconds(3);
    }
    if(I == 64000) {
      dataAvailable = false;
    }
    if(dataAvailable == true) {
      unsigned long startClk = micros();
      unsigned long bitTimes[8];
      for(int i = 0; i < 7; i++) {
        bitTimes[i] = 1.5 * clk + startClk + i * clk;
      }
      for(int i = 0; i < 7; i++) {
        while(micros() < bitTimes[i]) {}
        if(analogRead(LIGHT_SENSOR) > minLight) {
          bitSet(x, i);
        } else {
          bitClear(x, i);
        }
      }
      if(x == 0) {
        dataAvailable = false;
      } else {
        text += x;
      }
      delayMicroseconds(clk / 2);
    }
  }
  Serial.println();
  return text;
}

void fillpixelbypixel(uint16_t color) {
  for (uint8_t x=0; x < tft.width(); x++) {
    for (uint8_t y=0; y < tft.height(); y++) {
      tft.drawPixel(x, y, color);
    }
  }
}

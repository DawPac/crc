#include <LiquidCrystal.h>

#define RX_DATA 3
#define RX_CLK 2
#define CRC_CLR 10
#define CRC_CLK 13
#define CRC_EN 12
#define CRC_OUT 11
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_RS 8
#define LCD_EN 9
#define DELAY 50

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
byte rx_byte = 0;
int rx_len = 0;
int bit_position = 0;
char message[16];
bool rx_done = 0;

void crc_clk(){
  digitalWrite(CRC_CLK, HIGH);
  delay(DELAY);
  digitalWrite(CRC_CLK, LOW);
  delay(DELAY);
}
  
void setup() {
  pinMode(RX_DATA, INPUT);
  pinMode(RX_CLK, INPUT);
  pinMode(CRC_CLR, OUTPUT);
  pinMode(CRC_CLK, OUTPUT);
  pinMode(CRC_EN, OUTPUT);
  pinMode(CRC_OUT, INPUT);
  digitalWrite(CRC_CLR, LOW);
  digitalWrite(CRC_CLR, HIGH);
  digitalWrite(CRC_EN, HIGH);
  lcd.begin(16,2);
  strcpy(message, "");
  attachInterrupt(digitalPinToInterrupt(RX_CLK), on_clk, RISING);
}

void on_clk() {
  
  bool rx_bit = digitalRead(RX_DATA);
  digitalWrite(CRC_CLK, HIGH);
    if (bit_position == 8) {
      rx_byte = 0;
      bit_position = 0;
    }
    if (rx_bit) {
      rx_byte |= (0x80 >> bit_position);
    }
    bit_position += 1;
    if (bit_position == 8 && rx_len != 0) {
      strncat(message, &rx_byte, 1);
      if(strlen(message) == rx_len+2) {
        rx_done = 1;
      }
    }
    else if (bit_position == 8) {
      rx_len = rx_byte;
    }
    digitalWrite(CRC_CLK, LOW);
  }

void crc_check() {
  delay(1000);
  digitalWrite(CRC_EN, LOW);
  int crc_check = 0;
  for(int i = 0; i < 16; i++){
    bool read_bit = digitalRead(CRC_OUT);
    if (read_bit) {
      crc_check++;
    }
    crc_clk();
  }
  lcd.setCursor(0,1);
  if(crc_check == 0){
    lcd.print("CORRECT");
  }
  else{
    lcd.print("ERROR");
  }
}

void loop() {
  if(strlen(message) < rx_len && rx_len != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 8; i += 1) {
      if (i < bit_position) {
        lcd.print(rx_byte & (0x80 >> i)? "1" : "0");
      } else {
        lcd.print(" ");
      }serial
    }
  }
  if(strlen(message) == rx_len){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
  }
  delay(200);
  if(rx_done == 1){
    crc_check();
    while(1){}
  }
}

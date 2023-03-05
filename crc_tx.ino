#include <LiquidCrystal.h>

#define TX_DATA 3
#define TX_CLK 2
#define CRC_CLR 10
#define CRC_CLK 13
#define CRC_EN 11
#define CRC_OUT 12
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_RS 8
#define LCD_EN 9

#define DELAY 50

const char *message = "Hello, World!";

void tx_clk(){
  digitalWrite(TX_CLK, HIGH);
  digitalWrite(CRC_CLK, HIGH);
  delay(DELAY);
  digitalWrite(TX_CLK, LOW);
  digitalWrite(CRC_CLK, LOW);
  delay(DELAY);
}

void crc_clk(){
  digitalWrite(CRC_CLK, HIGH);
  delay(DELAY);
  digitalWrite(CRC_CLK, LOW);
  delay(DELAY);
}

void setup() {
  LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print(message);
  
  pinMode(TX_DATA, OUTPUT);
  pinMode(TX_CLK, OUTPUT);
  pinMode(CRC_CLK, OUTPUT);
  pinMode(CRC_CLR, OUTPUT);
  pinMode(CRC_EN, OUTPUT);
  pinMode(CRC_OUT, INPUT);
  digitalWrite(CRC_CLR, LOW);
  digitalWrite(CRC_CLR, HIGH);
  digitalWrite(CRC_EN, HIGH);
  delay(2000);

  char message_len = (char)strlen(message);
  for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
    bool tx_bit = message_len & (0x80 >> bit_idx);
    digitalWrite(TX_DATA, tx_bit);
    tx_clk();
  }
  
  for(int byte_idx = 0; byte_idx < strlen(message); byte_idx++){
    char tx_byte= message[byte_idx];

    lcd.setCursor(0,1);
    lcd.print("        ");
    
    for(int bit_idx = 0; bit_idx < 8; bit_idx++){
      bool tx_bit = tx_byte & (0x80 >> bit_idx);
      digitalWrite(TX_DATA, tx_bit);
      lcd.setCursor(bit_idx,1);
      lcd.print(tx_bit ? "1" : "0");
      lcd.setCursor(byte_idx,0);
      lcd.cursor();
      tx_clk();
      lcd.noCursor();
    }
  }
  digitalWrite(TX_DATA, LOW);
  for(int i = 0; i < 16; i++){
    crc_clk();
  }
  digitalWrite(CRC_EN, LOW);
  lcd.setCursor(0,1);
  lcd.print("        ");
  lcd.setCursor(0,1);
  for(int i = 0; i < 16; i++){
    bool read_bit = digitalRead(CRC_OUT);
    digitalWrite(TX_DATA,read_bit ? HIGH : LOW);
    lcd.print(read_bit ? "1" : "0");
    tx_clk();
  }
  digitalWrite(TX_DATA, LOW);
}

void loop() {
    
}

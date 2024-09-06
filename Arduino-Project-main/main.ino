#include "HX711.h"
#include "LiquidCrystal_I2C.h"

const int DT_PIN = 7;
const int SCK_PIN = 8;

HX711 scale;

const int I2C_ADDRESS = 0x27; 
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;

LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

const long ZERO_READING = 398; // şişe boş
const long FULL_SCALE_READING = 641;  // şişe full doluyken
const float KNOWN_WEIGHT = 0.5; // max su ml

float scale_factor;
float reference_voltage = 5.0;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");

  scale_factor = (FULL_SCALE_READING - ZERO_READING) / KNOWN_WEIGHT;
}

void loop() {
  if (scale.is_ready()) {
    long raw_reading = (scale.read())/1000;
    //Serial.println(raw_reading);

    float voltage = (raw_reading / 1023.0) * reference_voltage;
    //Serial.println(voltage);
    
    float weight = (raw_reading - ZERO_READING) / scale_factor;
    Serial.println(weight);

    if (weight < 0) {
      weight = 0;
    }

    if (weight >= 0.1 && weight <= 0.2) {
      lcd.setCursor(0, 1);
      lcd.print("Almost!");
    } else if (weight > 0.2 && weight <= 0.3) {
      lcd.setCursor(0, 1);
      lcd.print("Halfway!");
    } else if (weight > 0.3 && weight <= 0.5) {
      lcd.setCursor(0, 1);
      lcd.print("Drink up!");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("              ");

    }

    lcd.setCursor(8, 0);
    lcd.print(weight, 2);
    lcd.setCursor(13, 0);
    lcd.print("kg");

    delay(500);
  }
}

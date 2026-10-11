
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define GAS_PIN A1
#define SOIL_PIN A2
#define FLAME_PIN A3

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int gasValue = analogRead(GAS_PIN);
  int soilValue = analogRead(SOIL_PIN);
  int flameValue = analogRead(FLAME_PIN);

  // Convert soil reading to approximate percentage
  int soilPercent = map(soilValue, 1023, 0, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // Display temperature and humidity
  lcd.clear();
  lcd.setCursor(0, 0);

  if (isnan(temp) || isnan(hum))
  {
    lcd.print("DHT Error");
  }
  else
  {
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print("C H:");
    lcd.print(hum, 0);
    lcd.print("%");
  }

  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print(soilPercent);
  lcd.print("% Gas:");
  lcd.print(gasValue);

  // Serial Monitor output
  Serial.println("----- Sensor Data -----");

  if (!isnan(temp) && !isnan(hum))
  {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
  }
  else
  {
    Serial.println("DHT11 reading error");
  }

  Serial.print("Gas Sensor: ");
  Serial.println(gasValue);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  // Many flame sensor modules give LOW on detection
  Serial.print("Flame Sensor Analog: ");
  Serial.println(flameValue);

  delay(2500);

  // Show gas and flame readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);

  lcd.setCursor(0, 1);

  if (flameValue < 500)
  {
    lcd.print("Flame Detected!");
    Serial.println("WARNING: Possible flame!");
  }
  else
  {
    lcd.print("No Flame");
  }

  delay(1500);
}

#define BLYNK_TEMPLATE_ID "TMPL6oleU87uX"
#define BLYNK_TEMPLATE_NAME "Weather Monitoring System"
#define BLYNK_AUTH_TOKEN "T2FaCCn0P4k3XzvuwyPAxYEvX9IYdT1E"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

// --- WiFi Credentials ---
char ssid[] = "ALHN-258C";
char pass[] = "TkgXC43grc";

// --- Pin Definitions ---
#define DHTPIN D4
#define DHTTYPE DHT11
#define RAIN_PIN D5
#define LDR_PIN A0

// --- Sensor & Display Objects ---
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if needed

void setup() {
  Serial.begin(9600);
  dht.begin();
  bmp.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(RAIN_PIN, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Weather Monitoring Started");
}

void loop() {
  Blynk.run();
  readAndDisplayData();
  delay(2000); // Consider replacing with BlynkTimer for better performance
}

void readAndDisplayData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0;

  // --- Light Detection ---
  int lightValue = analogRead(LDR_PIN);
  int lightLED = (lightValue > 700) ? 0 : 255; // 255 = ON (dark), 0 = OFF (bright)
  String lightStatus = (lightValue > 700) ? "OFF" : "ON";

  // --- Rainfall Percentage ---
  int rainDigital = digitalRead(RAIN_PIN);
  int rainPercent = rainDigital == 0 ? 100 : 0;

  // --- Debug Serial Output ---
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C, ");
  Serial.print("Humidity: "); Serial.print(hum); Serial.print(" %, ");
  Serial.print("Pressure: "); Serial.print(pressure); Serial.print(" hPa, ");
  Serial.print("Light: "); Serial.print(lightStatus); Serial.print(", ");
  Serial.print("Rain: "); Serial.print(rainPercent); Serial.println("%");

  // --- LCD Display ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temp); lcd.print("C ");
  lcd.print("H:"); lcd.print(hum); lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("L:"); lcd.print(lightStatus);
  lcd.print(" R:"); lcd.print(rainPercent); lcd.print("%");

  // --- Send to Blynk ---
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, pressure);
  Blynk.virtualWrite(V3, rainPercent);
  Blynk.virtualWrite(V4, lightLED); // Integer value for LED widget
}

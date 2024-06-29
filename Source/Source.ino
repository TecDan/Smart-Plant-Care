#include <DHT.h>
#define Type DHT11 
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// lower moisture reading means higher moisture.
// wet soil reads 440-450, air = 500-510, water is 280-290
const int pump = 3; // Pin connected to the pump relay
const int button = 4; // button pin
const int dry = 480; // Threshold moisture level; below this, watering is not needed
const int wait = 5000; // Wait time in milliseconds

int sensePin = 2; // Pin connected to the DHT sensor
DHT HT(sensePin, Type); // Initialize DHT sensor
float humid; // Variable to store humidity
float temp; // Variable to store humidity

void setup() {
  
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.print("System On");
  pinMode(pump, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps
  HT.begin();
  digitalWrite(pump, HIGH); // turn pump off
  delay(500);
}

void loop() {
  lcd.setCursor(0,0); //Set up the cursor to column 0, line 1
  int moisture = analogRead(0); //connect sensor to Analog 0
  humid = HT.readHumidity();
  temp = HT.readTemperature();
  int buttonState = digitalRead(button);

  delay(wait);
  char buffer[17]; // buffer to hold the formatted string
  sprintf(buffer, "Moisture:%d", moisture);

    // Manual watering control with button press
  if (buttonState == HIGH) { // Button pressed
    lcd.clear();
    lcd.print("Manual Watering");
    Serial.println("Manual Watering");
    digitalWrite(pump, LOW); // Turn the pump on
    delay(wait);
    digitalWrite(pump, HIGH); // Turn the pump off
    lcd.clear();
    lcd.print("Watering Done");
    delay(wait);
    lcd.clear();
    return; // Skip the rest of the loop
  }
    // Turn water on/off depending on moisture level
  if(moisture >= dry){
    lcd.clear();
    Serial.println(moisture);
    lcd.print("Now Watering");
    Serial.println("Now Watering");
    digitalWrite(pump, LOW); // turn pump on
    delay(wait);

    lcd.clear();
    digitalWrite(pump, HIGH); // turn pump off
    Serial.println("Done Watering");
    lcd.print("Done Watering");
    delay(wait);
  } else {
    lcd.clear();
    Serial.println(moisture); 
    lcd.print(buffer);
    lcd.setCursor(0, 1);
    lcd.print("Don't need water");
    Serial.println("Don't need water");
    delay(wait);
    lcd.clear();
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humid);
  }

}

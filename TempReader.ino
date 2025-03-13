#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
const int sensorPin = A0; // Grove Temperature Sensor connected to A0
const float BETA = 3950; // Beta value for the NTC thermistor

void setup() {
    lcd.begin(16, 2); // Initialize the LCD
    lcd.setRGB(100, 255, 0);
}

void loop() {
    int analogValue = analogRead(sensorPin);
    float resistance = (1023.0 / analogValue) - 1.0;
    resistance = 10000.0 / resistance; // Convert to resistance
    float temperatureC = 1.0 / (log(resistance / 10000.0) / BETA + 1.0 / 298.15) - 273.15;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.print(" C");
    
    // Displaying message to user if the temperature is fine or not
    lcd.setCursor(0, 1);
    if(temperatureC > 0 && temperatureC < 20 ){
      lcd.setRGB(0, 255, 255);
      lcd.print("Its Cold Out!")
    }
    else if (temperatureC < 0){
      lcd.setRGB(0, 0, 255);
      lcd.print("Its Freezing!")
    }
    else if (temperatureC > 20 && temperatureC < 30) {
      lcd.setRGB(0, 255, 0);
        lcd.print("Its Nice Outside");
    } 
    else if(temperatureC > 30 && temperatureC < 40){
      lcd.setRGB(255, 150, 0);
      lcd.print("Wear Sunscreen!");
    }
    else {
      lcd.setRGB(255, 0, 0);
        lcd.print("Stay indoors!");
    }
    
    delay(2000); 
}

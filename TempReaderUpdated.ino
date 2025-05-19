#include <Wire.h>
#include "rgb_lcd.h"
#include <Bridge.h>
#include <HttpClient.h>
#include <Process.h>

rgb_lcd lcd;

const int tempPin = A0;  // Temperature Sensor
const int lightPin = A1; // Light Sensor
const int soundPin = A2; // Sound Sensor
const float BETA = 3950;

const String PUSHINGBOX_DEVICE_ID = "vA0C36762177F073"; // Replace this

bool alertSent = false;

void setup() {
    Bridge.begin();
    lcd.begin(16, 2);
    lcd.setRGB(100, 255, 0);
    Serial.begin(9600);
    delay(2000);
}

void loop() {
    // --- Temperature Reading ---
    int analogValue = analogRead(tempPin);
    float resistance = (1023.0 / analogValue) - 1.0;
    resistance = 10000.0 / resistance;
    float temperatureC = 1.0 / (log(resistance / 10000.0) / BETA + 1.0 / 298.15) - 273.15;

    // --- Light Reading ---
    int lightValue = analogRead(lightPin);
    String sunlight = (lightValue > 600) ? "Bright" : (lightValue > 300) ? "Dim" : "Dark";

    // --- Sound Reading ---
    int soundValue = analogRead(soundPin);
    String windStatus = (soundValue > 500) ? "Windy" : "Calm";

    // --- LCD Display: Temp ---
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC, 1);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    if (temperatureC < 0) {
        lcd.setRGB(0, 0, 255);
        lcd.print("Its Freezing!");
    } else if (temperatureC < 20) {
        lcd.setRGB(0, 255, 255);
        lcd.print("Its Cold Out!");
    } else if (temperatureC < 30) {
        lcd.setRGB(0, 255, 0);
        lcd.print("Its Nice Outside");
    } else if (temperatureC < 40) {
        lcd.setRGB(255, 150, 0);
        lcd.print("Wear Sunscreen!");
    } else {
        lcd.setRGB(255, 0, 0);
        lcd.print("Stay indoors!");
    }

    delay(3000);

    // --- LCD Display: Light + Wind ---
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sun: " + sunlight);
    lcd.setCursor(0, 1);
    lcd.print("Wind: " + windStatus);

    delay(3000);

    // --- Send API Alert (if extreme temps) ---
    if ((temperatureC < 0 || temperatureC > 30) && !alertSent) {
        sendPushingBoxAlert();
        alertSent = true; // avoid spamming
    }

    // Reset alert once conditions are back to normal
    if (temperatureC >= 0 && temperatureC <= 30) {
        alertSent = false;
    }
}

// --- Function to Send Alert to PushingBox ---
void sendPushingBoxAlert() {
    Serial.println("Sending alert to PushingBox...");
    HttpClient client;
    String url = "http://api.pushingbox.com/pushingbox?devid=" + PUSHINGBOX_DEVICE_ID;
    client.get(url);

    while (client.available()) {
        char c = client.read();
        Serial.print(c);  // Debug response
    }

    Serial.println("Alert sent!");
}
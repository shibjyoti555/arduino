#include <FreqCount.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Frequency variables
long frequency = 0;
unsigned long sampleTime = 1000;  // 1 second sample time for frequency measurement
unsigned long prevTime = 0;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  
  // Initialize the frequency counter
  FreqCount.begin(1000); // Start the counter with a sample time of 1 second (1000ms)
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the display buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.print("Frequency Detector");
  display.display();
  delay(1000);  // Display splash screen for 1 second
  display.clearDisplay();
}

void loop() {
  if (FreqCount.available()) {
    // Get frequency
    frequency = FreqCount.read();

    // Display frequency on Serial Monitor
    Serial.print("Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");

    // Display frequency on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Base Frequency:");
    display.setTextSize(2);  // Set larger font for frequency
    display.setCursor(0, 20);
    display.print(frequency);
    display.print(" Hz");
    display.display();
    
    // Add a small delay before the next measurement
    delay(1);
  }
}
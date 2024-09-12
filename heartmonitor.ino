#define USE_ARDUINO_INTERRUPTS true
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int PULSE_SENSOR_PIN = A1;
const int LED_PIN = 13;
const int THRESHOLD = 100;  // Increased threshold for better detection
// const float ALPHA = 0.1;
const int NO_OBJECT_THRESHOLD = 50;  // Threshold to detect when no object is present

PulseSensorPlayground pulseSensor;
// float filteredBPM = 0;
bool objectPresent = false;

void setup() {
  Serial.begin(9600);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Heartrate Monitor");
  display.setTextSize(1);
  display.println("~shibjyoti");
  display.display();
  delay(2000);
  display.setTextSize(2);
  display.clearDisplay();
  
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created successfully!");
  }
}

void loop() {
  int rawSignal = analogRead(PULSE_SENSOR_PIN);
  
  if (rawSignal < NO_OBJECT_THRESHOLD) {
    if (objectPresent) {
      objectPresent = false;
      Serial.println("No object detected on the sensor");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("No object detected");
      display.println("Place your finger");
      display.println("on the sensor");
      display.display();
    }
  } else {
    objectPresent = true;
    
    int currentBPM = pulseSensor.getBeatsPerMinute();
    
    if (pulseSensor.sawStartOfBeat()) {
      Serial.println("\n♥ A HeartBeat Happened!");
      Serial.print("BPM: ");
      Serial.println(currentBPM);
      
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("HeartBeat Detected!");
      // display.print("Filtered BPM: ");
      // display.println(filteredBPM);
      display.print("BPM: ");
      display.println(currentBPM);
      display.display();
    }
  }
  
  delay(20);
}
#include <DHT.h>
#include <DHT_U.h>

//original code from https://www.makerguides.com/dht11-dht22-arduino-tutorial/
//polkadot plants prefer temperature between 70 & 80 and humidity above 50%)



// Include the libraries:
#include <Adafruit_Sensor.h>
// Set DHT pin:
#define DHTPIN 7
// Set DHT type
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// Initialize DHT sensor
DHT dht = DHT(DHTPIN, DHTTYPE);
int val = 0; //value for storing moisture value
int soilPin = A0;//Declare a variable for the soil moisture sensor
int soilPower = 6;//Variable for Soil moisture Power
const int numReadings = 60;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

const int redPin = 2; // red led pin on ~D2
const int greenPin = 3; // green led pin on ~D3
const int bluePin = 4; // blue led pin on ~D4

unsigned long previousMillis = 0;
unsigned long myTime;
float humidity;
float t;
float f;
const long interval = 10000;   // milliseconds
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);

  // Setup sensor:
  dht.begin();
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // Wait a few seconds between measurements:
  delay(2000);

  unsigned long currentMillis = millis();

  //use millis so only call the soil moisture every 10 seconds (interval)
  if (currentMillis - previousMillis >= interval) {
    int ledState = LOW;
    if (readSoil() <= 175) {
      ledState = HIGH;
      //digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      //  Serial.println("water your plant");
    }
    else if (readSoil() > 175) {
      //digitalWrite(ledPin,LOW);
    }
    Serial.print("Soil Moisture = ");
    ////get soil moisture value from the function below and print it
    Serial.println(readSoil());

    previousMillis = currentMillis;

  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  // Read the humidity in %:
  humidity = dht.readHumidity();
  // Read the temperature as Celsius:
  t = dht.readTemperature();
  // Read the temperature as Fahrenheit:
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again):
  if (isnan(humidity) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (default):
  float hif = dht.computeHeatIndex(f, humidity);
  // Compute heat index in Celsius:
  float hic = dht.computeHeatIndex(t, humidity, false);

  //polkadot plants prefer temperature between 70 & 80 and humidity above 50%)

if((readSoil() >= 50 && readSoil() <= 300) && (f >= 69 && f <= 75)){
    Serial.println("above 180 and less than 300 moisture");
    RGB_color(0, 255, 0); // green
}

else if(humidity >40 && humidity <=59){
   RGB_color(0, 0, 255); // blue
   
}

else{
  RGB_color(255, 0, 0); // red
}

//if humidity is not accurate turn blue 
//if temperature is not right turn yellow
//if need water turn magenta 
//if all bad turn red 



  Serial.print("Soil Moisture: ");

  Serial.println(readSoil());
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" % ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" \xC2\xB0");
  Serial.print("C | ");
  Serial.print(f);
  Serial.print(" \xC2\xB0");
  Serial.print("F ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" \xC2\xB0");
  Serial.print("C | ");
  Serial.print(hif);
  Serial.print(" \xC2\xB0");
  Serial.println("F");

  
//  RGB_color(255, 0, 0); // Red
//  delay(1000);
//  RGB_color(0, 255, 0); // Green
//  delay(1000);
//  RGB_color(0, 0, 255); // Blue
//  delay(1000);
//  RGB_color(255, 255, 125); // Raspberry
//  delay(1000);
//  RGB_color(0, 255, 255); // Cyan
//  delay(1000);
//  RGB_color(255, 0, 255); // Magenta
//  delay(1000);
//  RGB_color(255, 255, 0); // Yellow
//  delay(1000);
//  RGB_color(255, 255, 255); // White
//  delay(1000);
}





//This is a function used to get the soil moisture content
int readSoil() {
  digitalWrite(soilPower, HIGH);//turn D7 "On"
  delay(100);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  digitalWrite(soilPower, LOW);//turn D7 "Off"
  return val;//send current moisture value

}


int calcAvg() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(soilPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average);
  delay(1);        // delay in between reads for stability
  return average;

}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  Serial.println("reading colors");
  analogWrite(redPin, red_light_value);
  analogWrite(greenPin, green_light_value);
  analogWrite(bluePin, blue_light_value);
}

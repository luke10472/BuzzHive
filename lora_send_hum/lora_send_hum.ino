#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

Adafruit_Sensor *aht_humidity, *aht_temp;

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

float myFloat = 0.0;
float temperature = 0;
float humidity_value = 0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!aht.begin()) {
    Serial.println("Failed to find AHT10/AHT20 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("AHT10/AHT20 Found!");
  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();

  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  //865E6 -867E6 For India
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  Load_AHT20_Data();

  // Increment the float for demonstration purposes
  myFloat += 0.1;

  Serial.print("Sending float value: ");
  Serial.println(humidity_value);

  // Convert the float to a string
  String floatString = String(humidity_value, 2); // 2 decimal places

  // Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hum: "); //This is how to transmit strings
  LoRa.print(floatString);
  LoRa.endPacket();

  delay(10000);
}
void Load_AHT20_Data() {
  sensors_event_t humidity_event, temp_event;
  aht_humidity->getEvent(&humidity_event);
  aht_temp->getEvent(&temp_event);
  //-----------------------------------------------------------
  temperature = temp_event.temperature; // Celsius
  humidity_value = humidity_event.relative_humidity;
  //-----------------------------------------------------------
  Serial.printf("Temperature: %.2f °C\n", temperature);
  Serial.printf("Humidity: %.2f %%\n", humidity_value);
}


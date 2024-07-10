#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

float myFloat = 0.0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  //865E6 -867E6 For India
  while (!LoRa.begin(865.0625E6)) {
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
  // Increment the float for demonstration purposes
  myFloat += 0.1;

  Serial.print("Sending float value: ");
  Serial.println(myFloat);

  // Convert the float to a string
  String floatString = String(myFloat, 2); // 2 decimal places

  // Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hum: "); //This is how to transmit strings
  LoRa.print(floatString);
  LoRa.endPacket();

  delay(10000);
}

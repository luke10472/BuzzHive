// this code receives temperature and humidity from an AHT20 via lora module
// Luke Riddoch 07/10/24

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
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
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet- ");

    // read packet
    String LoRaData = "";
    while (LoRa.available()) {
      LoRaData += (char)LoRa.read();
    }
    Serial.print(LoRaData);

    // Extract float value from the received string
    // int floatStart = LoRaData.indexOf("Float value: ") + String("Float value: ").length();
    int floatStart1 = LoRaData.indexOf("hum:") + String("hum:").length(); //finds the index of the phrase in LoRaData then adds the length of the phrase to index the corresponding value
    int floatStart2 = LoRaData.indexOf("temp:") + String("temp:").length(); 
    if (floatStart1 > 0) {
      String floatString1 = LoRaData.substring(floatStart1);
      float receivedFloat1 = floatString1.toFloat();
      Serial.print(" -> Humidity Mesurement: ");
      Serial.print(receivedFloat1);

      String floatString2 = LoRaData.substring(floatStart2);
      float receivedFloat2 = floatString2.toFloat();
      Serial.print(" -> Temperature Mesurement: ");
      Serial.print(receivedFloat2);
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

// this code receives temperature and humidity from an AHT20 via lora module
// Luke Riddoch 07/10/24

#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <LoRa.h>

// define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

// wifi setup
String URL = "http://192.168.86.43/dht11_project/test_data.php";
const char* ssid = "Border Security Network"; 
const char* password = "happyboat736"; 

float humidity_value = 0;
float temperature = 0;

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

  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

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
    int hum_float_start = LoRaData.indexOf("hum:") + String("hum:").length(); //finds the index of the phrase in LoRaData then adds the length of the phrase to index the corresponding value
    int temp_float_start = LoRaData.indexOf("temp:") + String("temp:").length(); 
    if (hum_float_start > 0) {
      String hum_float_string = LoRaData.substring(hum_float_start);
      float received_hum = hum_float_string.toFloat();
      Serial.print(" -> Humidity Mesurement: ");
      Serial.print(received_hum);

      String temp_float_string = LoRaData.substring(temp_float_start);
      float received_temp = temp_float_string.toFloat();
      Serial.print(" -> Temperature Mesurement: ");
      Serial.print(received_temp);

      humidity_value = received_hum;
      temperature = received_temp;
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity_value);
  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData);
  Serial.print("httpCode: "); Serial.println(httpCode);
  Serial.print("payload : "); Serial.println(payload);
  Serial.println("--------------------------------------------------");

  }
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to: "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

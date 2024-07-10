
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

Adafruit_Sensor *aht_humidity, *aht_temp;

String URL = "http://192.168.86.43/dht11_project/test_data.php";

const char* ssid = "Border Security Network"; 
const char* password = "happyboat736"; 

float temperature = 0;
float humidity_value = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("Adafruit AHT10/AHT20 test!");

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

  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  Load_AHT20_Data();
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

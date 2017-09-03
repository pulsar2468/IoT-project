#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SimpleTimer.h>
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "test1";
const char* password = "magic";
String name_board="riccardo_HomeArchirafi_Palermo"; //this is important!

HTTPClient http;
WiFiServer server(8001);
SimpleTimer timer; 
byte id_timer;


void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
  id_timer=timer.setInterval(5000, get_data);  //I get weather data each 5 seconds, in this way I listen the requests
  
}


void loop() {
  timer.run();
}


void post_to_server(int h, int t) {
http.begin("http://192.168.42.155:8000/weather/DataFromWs/?data="+name_board+","+String(14.0)+","+String(15.0));
http.addHeader("Content-Type", "text/plain");

int httpCode = http.GET();   //Send the request
//String payload = http.getString(); //Get the response payload

Serial.println("Response Code:" + String(httpCode));   //Print HTTP return code
//Serial.println(payload);    //Print request response payload
http.end();  //Close connection
}


void get_data(){
int h = dht.readHumidity();
int t = dht.readTemperature();

if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
}
post_to_server(h,t);
} 







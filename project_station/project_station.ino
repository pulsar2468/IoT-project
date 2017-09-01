#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>
#include <SimpleTimer.h>

#define REQ_BUF_SZ   20
#define PIN D2 // Led RGB
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
const int chipSelect = D8; // Sd card pin
float array_data[18];
String name_train;
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
File myFile;
int i=0,n_train=0;
int flag=0;
bool flagWind=true;
float startTime,endTime;
float totalTime;
float v;
String file_name_text;
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0; 
const char* ssid = "test1";
const char* password = "magic";
String name_board="shiva_my_station1_Palermo"; //this is important!
char x[20]="DATA/";
int g;
File webFile;
HTTPClient http;
WiFiServer server(8001);
SimpleTimer timer; 
byte id_timer;


void setup() {
  Serial.begin(115200);
  delay(10);
  

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
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
  id_timer=timer.setInterval(5000, get_data);  //I get weather data each 10 seconds, in this way I listen the requests
  
}


void loop() {
  timer.run();
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind,char index)
{
    char found = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}


void remove_file(){
  char name_f[20];
  String f;
  while(true){
    if (n_train==-1) {
      n_train=0;
    break;
  }
    f="DATA/"+ String(n_train)+".TXT";
    f.toCharArray(name_f,20);
    Serial.println(name_f);
    SD.remove(name_f);
    n_train--;
  }
  Serial.println("All data deleted");
  }

    
  
void save_to_file(){
  char name_f[20];
  String f;
  while(true){
    f="DATA/"+ String(n_train)+".TXT";
f.toCharArray(name_f,20);
Serial.println(name_f);
if (SD.exists(name_f)){
  n_train++;
}
else {
Serial.println("Created");
myFile = SD.open(name_f, FILE_WRITE);
n_train++;
break;
}

 }
if (myFile) {
    for (int j=0;j<18;j+=3){
      myFile.print(array_data[j]);
      myFile.print(" ");
      myFile.print(array_data[j+1]);
      myFile.print(" ");
      myFile.println(array_data[j+2]);
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("error IO file");
  }
}                 



int period() {
   if (flag == 0) {
    startTime=millis();
    flag=1;
    while(digitalRead(D1) == 0){
      if(millis()-startTime > 5000){
        flag=0;
        return 1;
      }
      delay(1);
    }  
    return 0;
   }

   if (flag == 1) {
    endTime=millis();
    totalTime=(endTime-startTime)/1000;
    v=(2*3.14*0.03)/(totalTime);
    flag=0;  
    array_data[i+2]=v;
    Serial.println(v*3.6);
    return 1;
   }
   
}


void iCanFeelWind(){
  float s1;
  Serial.println("I feel wind..");
  s1=millis();
  while(1){
    if(millis()-s1 > 5000){
      return;
    }
  if(!digitalRead(D1)){
    if(period()) return;
  }
  delay(1);
  }
}



void post_to_server(int h, int t) {
http.begin("http://192.168.42.155:8000/weather/DataFromWs/?data="+name_board+","+String(14.0)+","+String(15.0));
http.addHeader("Content-Type", "text/plain");

int httpCode = http.GET();   //Send the request
//String payload = http.getString();                                        //Get the response payload

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







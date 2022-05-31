
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>

//OLED Screen:
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//SCREEN INITIALIZATION
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//End of OLED INIT

//BLE init
BLEScanResults foundDevices;
int scanTime = 10; //In seconds
char placeHold[100];
char uhhh[100];
char BL_int[100];
char NAME[100] = "NODE 1";

//WIFI const variables
const char* ssid = "Senior Design";
const char* pass = "";
const char* user = "";

//MQTT const variables
const char* mqttServer = "192.168.1.121";
const int mqttPort = 1883;
const char* mqttUser = "CE490";
const char* mqttPass = "CE490";
const char* mqttTopic = "ce490connect";

WiFiClient espClient;
PubSubClient client(espClient);

BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("%s\n", advertisedDevice.getAddress().toString().c_str());
      sprintf(placeHold,advertisedDevice.getAddress().toString().c_str());
      
      client.publish(mqttTopic, placeHold);
      delay(20);
    }
};

//Generate characters on OLED screen
void DisplayText(char* GivenNumber, const char* GivenVariableName) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  for(int16_t i=0; i<strlen(GivenVariableName); i++) {
    if(i == '\n') display.write(' ');
    else          display.write(GivenVariableName[i]);
  }
  display.setCursor(0, 16); //Columns range: 0->127, Row range: 0->15, so every 16 pixels == new row
  for(int16_t j=0; j<2; j++) {
    if(j == '\n') display.write(' ');
    else          display.write(GivenNumber[j]);
  }
  
  display.display();      // Show initial text
  delay(2000);
  
}

void wifiSetup(){
  delay(2000);
  Serial.println();

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWIFI CONNECTED");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
}

// SETUP FUNCTION //

void setup() {
  Serial.begin(115200);

  Serial.println("connecting to wifi");
  wifiSetup();
  
    //OLED Boot:
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed: OLED Screen failed to load"));
    for(;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  // "MEM_RESET" message during purge 
  display.setTextSize(2);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.write('M');
  display.write('E');
  display.write('M');
  display.write('_');
  display.write('R');
  display.write('E');
  display.write('S');
  display.write('E');
  display.write('T');


  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  //End of OLED Boot

  
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("\nConnecting to MQTT...");
    if (client.connect(mqttTopic, mqttUser, mqttPass)) {
      Serial.println("connected");

      display.clearDisplay();
      
      display.setTextSize(2);
      display.setTextColor(WHITE); // Draw white text
      display.setCursor(0, 0);     // Start at top-left corner
      display.write('M');
      display.write('Q');
      display.write('T');
      display.write('T');
      display.write('_');
      display.write('S');
      display.write('U');
      display.write('C');
      display.write('C');
      display.write('E');
      display.write('S');
      display.write('S');
      
      display.display();
      delay(2000);
    }
    
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());

      display.clearDisplay();
      
      display.setTextSize(2);
      display.setTextColor(WHITE); // Draw white text
      display.setCursor(0, 0);     // Start at top-left corner
      display.write('M');
      display.write('Q');
      display.write('T');
      display.write('T');
      display.write('_');
      display.write('F');
      display.write('A');
      display.write('I');
      display.write('L');
      
      display.display();
      delay(2000);
    }
  }
  
  
  Serial.println("Scanning surrounding area...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

}

void loop() {

  int AN_Inl = 36;
  int sensorValue = analogRead(AN_Inl);

  client.loop();
  
  delay(5000); //delay 5 sec between scans
  
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Number of devices found: ");
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);

  //OLED functionality
  const char* VariableName = "DevFound:"; // Can only Display on a single line: "Random Num"
                                          //so at 2x scale characters, only 10 chars can be displayed
  char ConvertRand[2];                    //Char Array
  itoa(foundDevices.getCount(), ConvertRand, 10);    //Convert found devices to char array      
  DisplayText(ConvertRand, VariableName); //Display function

  Serial.println("Battery Life: ");
  Serial.print(BL_int);
  Serial.println("\n");

  //client.publish(mqttTopic, NAME);
  
  int topInTheMariot = foundDevices.getCount();
  sprintf(uhhh,"%d",topInTheMariot);
  client.publish(mqttTopic, uhhh);

  /*
  sprintf(BL_int, "%d", sensorValue);
  client.publish(mqttTopic, BL_int);
  */
  delay(20000);
}

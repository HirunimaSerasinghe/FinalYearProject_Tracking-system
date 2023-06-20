
//-----------------------------------------------------------------------------------

#include <FirebaseESP8266.h>  
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h> 


//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
#define FIREBASE_HOST "railway-ticketing-system-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "QqLkWH4Rzk9LQOuz53Xzy30nB4z8XmDGNOCqRlmi"
#define WIFI_SSID "OPPO F9"
#define WIFI_PASSWORD "masH@98@"
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------

FirebaseData firebaseData;

FirebaseJson json;
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------

const int RXPin = 4, TXPin = 5;
SoftwareSerial neo6m(RXPin, TXPin);
TinyGPSPlus gps;

void setup()
{

  Serial.begin(9600);

  neo6m.begin(9600);
  
  wifiConnect();

  Serial.println("Connecting Firebase.....");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase OK.");

}

void loop() {
  
  smartdelay_gps(1000);

  if(gps.location.isValid()) 
  {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    
    //-------------------------------------------------------------

    //-------------------------------------------------------------
    
    //-------------------------------------------------------------
    if(Firebase.setFloat(firebaseData, "/GPS/f_latitude", latitude))
      {print_ok();}
    else
      {print_fail();}
    //-------------------------------------------------------------
    if(Firebase.setFloat(firebaseData, "/GPS/f_longitude", longitude))
      {print_ok();}
    else
      {print_fail();}
   //-------------------------------------------------------------
  }
  else
  {
    Serial.println("No valid GPS data found.");
  }
  
  delay(5000);
}

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
M
void print_ok()
{
    Serial.println("------------------------------------");
    Serial.println("OK");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
}

void print_fail()
{
    Serial.println("------------------------------------");
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
}


void firebaseReconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
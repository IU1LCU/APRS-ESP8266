/*
APRS WEATHER STATION
A simply script to send data using http post request to APRS-IS
Using an ESP8266 (NodeMCU) and a DHT11
Developed by IU1LCU and IU8HPE
A&A Electronics 2024
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
ESP8266WiFiMulti WiFiMulti;
#define DHTPIN D2     // Pin del sensore DHT11
#define DHTTYPE DHT11  // Tipo di sensore DHT
DHT dht(DHTPIN, DHTTYPE);
// Temperature in degrees Fahrenheit (°F) = (Temperature in degrees Celsius (°C) * 1,8) + 32
/////// WiFi Data //////////////
const char* ssid = "WIFINAME";        //WIFI SSID 
const char* wifipass = "WIFIPASS";  //WIFI Password

/////// User Data APRS-IS///////
const char* callsign = "CALLSIGN";      //Callsign
const char* aprsid = "13";            //APRS-ID
const char* icon = "_";               // APRS Icon (see on google "APRS synbol")
const char* aprspass = "12345";       // APRS-IS pass (see on google "APRS passcode generator")
const char* vers = "TestESP";         //Device, not important
const char* comment = "TEST";
const char* lat = "XXXX.XXN";         //GPS Position Example: 4410.36N 00820.47E
const char* lon = "XXXX.XXE";
////// Server Data APRS-IS ///////
const uint16_t port = 14580;          //APRS-IS Default port
const char* host = "france.aprs2.net";  //APRS-IS Server
///// Delay for sending the packet /////
int wait = 1800000; // 30 min in mS

void setup() {
  Serial.begin(115200); 
  Serial.flush();
  Serial.println("");
  Serial.println(F("Starting..."));
  WiFiMulti.addAP(ssid, wifipass); 
  //WiFiMulti.addAP("SSID", "password"); // Here you can add a secondary wifi, the ESP8266 will connect on the most powerful wifi, you can add many as you want

if ((WiFiMulti.run() == WL_CONNECTED)) {
  delay(1000);
  Serial.println("Connected to WiFi");
  Serial.print("Local IP: ");
  Serial.print(WiFi.localIP());
  Serial.println(""); } 
  
  } // END of setup
 
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = t * 1.8 + 32; // Conversion from Celsius to fahrenheit
  int fi = round(f); //Remove the decimal numbers after dot
  int hi = round(h); //Remove the decimal numbers after dot
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    WiFiClient client; // Wifi client mode
    delay(5000);

    if (!client.connect(host, port)) {
      return; // If connection fail, quit the loop
    }
    String userpass = "user " + String(callsign) + "-" + String(aprsid) + " pass " + String(aprspass) + " vers " + String(vers);
    Serial.println(userpass);
    client.println(userpass);
    
    delay(300);
    String packet = String(callsign) + "-" + String(aprsid) + ">APRS:=" + String(lat) + "/" + String(lon) + String(icon) + ".../...g...t" + String(fi) + "r...p...P...h" + String(hi) + "b.....L...." + String(comment);
    //String packet = String(callsign) + "-" + String(aprsid) + ">APRSWX,TCPIP*,qAC,WIDE1-1:=" + String(lat) + "/" + String(lon) + String(icon); //Basic aprs beaconing
    client.println(packet);
    Serial.println(packet);
    delay(wait); 
} 
} // END of loop
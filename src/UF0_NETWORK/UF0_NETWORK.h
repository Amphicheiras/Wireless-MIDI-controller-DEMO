/*
    *** WiFi & WebServer related operations ***

          UF0_WEBSERVER class:

          UF0_WiFi class:

          HTMLprocessor:
            Parse & process HTML placeholders

          connectWiFi:
            Connect to WLAN

          setup_WiFi:
            Initialize WiFi systems
*/

#ifndef UF0_NETWORK_h
#define UF0_NETWORK_h

// Preliminary
    #include "UF0_HARDWARE/UF0_HARDWARE.h"
    UF0_POWER *pwr;
    #include "UF0_OS/UF0_DBG.h"
    #include "UF0_MUSIC/UF0_MIDI.h"
    UF0_MIDI *midii;    
    #include "UF0_OS/UF0_BLACKMAGIC.h"
    UF0_BLACKMAGIC *bm;
  // Wi-Fi
    #include <WiFi.h>
  // Server
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  // Web pages (HTML)
    #include "UF0_HTML_PAGES\credentialsPage.h"
    #include "UF0_HTML_PAGES\transmitGyroPage.h"
    
  // AP preliminary
    char AP_ssid[50];
  // STA preliminary
    // Set Static IP address
    IPAddress local_IP(192, 168, 137, 27);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 0, 0);
    IPAddress primaryDNS(8, 8, 8, 8);
    IPAddress secondaryDNS(8, 8, 4, 4);
    // char STA_ssid[50] = "Teles PC";
    // char STA_pass[50] = "987654321";
    String STA_ssid;
    String STA_pass;
  // V V V V V Save to disk
    char STA_IP_URL[50];
    IPAddress STA_IP;
    // AsyncWebServer setupServer(80);
    AsyncWebServer controlServer(80);
    bool connected2WiFi = false;
  // Timers
    unsigned long NETWORK_t0 = millis();    
//

///////////////////////////// S E R V E R ///////////////////////////////
// Website not found
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Propagate data to HTML
const char* HTMLprocessor(const String& var){
  if(var == "pitchTransmit"){
    return (pitchTransmit ? "On" : "Off");
  }else if(var == "rollTransmit"){
    return (rollTransmit ? "On" : "Off");
  }else if(var == "yawTransmit"){
    return (yawTransmit ? "On" : "Off");
  }else if(var == "isConnected"){
    return (connected2WiFi ? "Connected to WiFi!" : "Not connected to WiFi");
  }
  return "fail";
}

void controlServerRequests(){
  DBG("WFFW333", pwr->get_battery_percent());
  controlServer.onNotFound(notFound);
  
  controlServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    /////////////////////////////////////////////////////////
    int temp = pwr->get_battery_percent();
    // float temp = millis();
    String batteryStr = String(temp);
    request->send(200, "text/plain", batteryStr);
    /////////////////////////////////////////////////////////

    // request->send_P(200, "text/html", credentialsHTML, HTMLprocessor);
  });
  
  controlServer.on("/connect", HTTP_GET, [](AsyncWebServerRequest *request){
    // connectWiFi(request);
  });

  controlServer.on("/redirect", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->redirect(STA_IP_URL);
  // Go incognitto (close UF0 Setup AP)
    // WiFi.softAPdisconnect(connected2WiFi);
    // DBG("Disconnected Setup AP");
  });
  
  controlServer.on("/jam", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/updateWiFi", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (connected2WiFi ? "Connected to WiFi!" : "Not connected to WiFi"));
  });

  controlServer.on("/startX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit on:", pitchTransmit);
    midii->startTransmission(&pitchTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/startY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("roll transmit on:", rollTransmit);
    midii->startTransmission(&rollTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/startZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("yaw transmit on:", yawTransmit);
    midii->startTransmission(&yawTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/stopX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", pitchTransmit);
    midii->stopTransmission(&pitchTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/stopY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("roll transmit off:", rollTransmit);
    midii->stopTransmission(&rollTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/stopZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("yaw transmit off:", yawTransmit);
    midii->stopTransmission(&yawTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/soloX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&pitchTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/soloY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&rollTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/soloZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&yawTransmit);
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/enableAll", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->startTransmissionAll();
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/disableAll", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->stopTransmissionAll();
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) {
    bm->savePreset();
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/load", HTTP_GET, [] (AsyncWebServerRequest *request) {
    bm->loadPreset();
    request->send_P(200, "text/html", transmitGyroHTML, HTMLprocessor);
  });

  controlServer.on("/updateX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (pitchTransmit ? "On" : "Off"));
  });

  controlServer.on("/updateY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (rollTransmit ? "On" : "Off"));
  });

  controlServer.on("/updateZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (yawTransmit ? "On" : "Off"));
  });
}

void connectWiFi(AsyncWebServerRequest *request = NULL){
  // STA_ssid = request->arg("ssid");
  // STA_pass = request->arg("password");
  STA_ssid = "Oxo Fanarii";
  STA_pass = "internet";
  DBG("Credentials entered: UN =", STA_ssid, "PW =", STA_pass);
// Opening Station mode
  DBG(F("Initializing Wi-Fi station mode"));
  WiFi.begin(STA_ssid, STA_pass);
  Serial.print("Establishing Wi-Fi connection to ");
  Serial.print(STA_ssid);
  while (WiFi.status() != connected2WiFi){
    if ((millis() - NETWORK_t0) > 300){
      Serial.print(".");
      NETWORK_t0 = millis();
    }
  }
  snprintf(AP_ssid, sizeof(AP_ssid), "%d.%d.%d.%d", STA_IP[0], STA_IP[1], STA_IP[2], STA_IP[3]);
  snprintf(STA_IP_URL, sizeof(STA_IP_URL), "http://%s/jamm~!", AP_ssid);
}

void WiFiStationConnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info){
  DBG("\nConnected to the dashing \'*oO", STA_ssid, "Oo*\'");
  WiFi.softAPdisconnect(true);
  connected2WiFi = WiFi.status();
}

void WiFiStationDisconnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info){
  DBG("Disconnected from the smashing \'*oO", STA_ssid, "Oo*\'");
  connected2WiFi = WiFi.status();
  DBG(F("Attempting to reconnect.."));
  WiFi.reconnect();
}

void setup_WiFi(){
  // Init & connect WiFi
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.onEvent(WiFiStationConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  // Start AP mode & setup server to enter credentials
    DBG("Initializing UF0 network");
    WiFi.softAP("UF0 Setup");
  // Start server
    DBG("Starting setup server");
    controlServer.begin(); 
  // Configure WiFi
    // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
    //   DBG(F("!! WiFi station monde failed to configure !!"));
    // }
  // Connect to WiFi
    connectWiFi();
}

class UF0_WEBSERVER{
  private:

  public:
    UF0_WEBSERVER(){}

    UF0_WEBSERVER(UF0_POWER *pwrr, UF0_MIDI *midiii, UF0_BLACKMAGIC *bmm){
      // DBG("WFFW1", pwr->get_battery_percent());
      pwr = pwrr;
      midii = midiii;
      bm = bmm;

      setup_webServer();
    }

    // Setup WebServer
    void setup_webServer(){
      // setupServerRequests();
      controlServerRequests();
      DBG("WFFW1", pwr->get_battery_percent());
    }

    // Define website server requests
    // void setupServerRequests(){}

};

//////////////////////////////// W i F i ////////////////////////////////////
class UF0_WiFi{
  private:

  public:

    UF0_WiFi(){}

    UF0_WiFi(bool cmon){
      setup_WiFi();
    }
};

#endif
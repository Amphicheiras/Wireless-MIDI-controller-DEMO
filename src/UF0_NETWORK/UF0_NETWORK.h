#ifndef UF0_NETWORK_h
#define UF0_NETWORK_h

// Preliminary
  // // Enterprise
    #include <UF0_DBG.h>
    #include "UF0_MIDI.h"
    UF0_MIDI *midii;
    #include "UF0_BLACKMAGIC.h"
    UF0_BLACKMAGIC *bm;
  // Wi-Fi
    #include <WiFi.h>
  // Server
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  // Web pages (HTML)
    #include "UF0_HTML_PAGES\credentialsPage.h"
    #include "UF0_HTML_PAGES\presetsPage.h"
    
  // AP preliminary
    char AP_ssid[50];
  // STA preliminary
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
  return "";
}

void controlServerRequests(){
  controlServer.onNotFound(notFound);
  
  controlServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", credentialsHTML, HTMLprocessor);
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
  
  controlServer.on("/jamm~!", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/startX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit on:", pitchTransmit);
    midii->startTransmission(&pitchTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/updateWiFi", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (connected2WiFi ? "Connected to WiFi!" : "Not connected to WiFi"));
  });

  controlServer.on("/startY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", rollTransmit);
    midii->startTransmission(&rollTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/startZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", yawTransmit);
    midii->startTransmission(&yawTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/stopX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", pitchTransmit);
    midii->stopTransmission(&pitchTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/stopY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", rollTransmit);
    midii->stopTransmission(&rollTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/stopZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    DBG("pitch transmit off:", yawTransmit);
    midii->stopTransmission(&yawTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/soloX", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&pitchTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/soloY", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&rollTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/soloZ", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->soloTransmission(&yawTransmit);
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/enableAll", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->startTransmissionAll();
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/disableAll", HTTP_GET, [] (AsyncWebServerRequest *request) {
    midii->stopTransmissionAll();
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) {
    bm->savePreset();
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
  });

  controlServer.on("/load", HTTP_GET, [] (AsyncWebServerRequest *request) {
    bm->loadPreset();
    request->send_P(200, "text/html", presetsHTML, HTMLprocessor);
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
  STA_ssid = "Teles PC";
  STA_pass = "987654321";
  DBG("Credentials entered: UN =", STA_ssid, "PW =", STA_pass);
// Opening Station mode
  DBG(F("Initializing Wi-Fi station mode"));
  WiFi.begin(STA_ssid, STA_pass);
  DBG(F("Establishing Wi-Fi connection to"), STA_ssid);
  while (WiFi.status() != connected2WiFi){
    if ((millis() - NETWORK_t0) > 1000){
      Serial.print(".");
      NETWORK_t0 = millis();
    }
  }
  DBG("");
  STA_IP = WiFi.localIP();
  snprintf(AP_ssid, sizeof(AP_ssid), "%d.%d.%d.%d", STA_IP[0], STA_IP[1], STA_IP[2], STA_IP[3]);
  snprintf(STA_IP_URL, sizeof(STA_IP_URL), "http://%s/jamm~!", AP_ssid);
}

void WiFiStationConnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info){
  DBG("Connected to the dashing \'*oO", STA_ssid, "Oo*\'");
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
  // Connect to WiFi
    connectWiFi();
}

class UF0_WEBSERVER{
  private:

  public:
    UF0_WEBSERVER(){}

    UF0_WEBSERVER(UF0_MIDI *midiii, UF0_BLACKMAGIC *bmm){
      midii = midiii;
      bm = bmm;

      setup_webServer();
    }

    // Setup WebServer
    void setup_webServer(){
      // setupServerRequests();
      controlServerRequests();
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
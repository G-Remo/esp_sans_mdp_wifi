#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#include "esp_wifi.h"
#else
#error "This ain't a ESP8266 or ESP32, dumbo!"
#endif

#include <ArduinoOTA.h>

//#define AP_STA

String Hote = "ESP";

bool init_wifi() {

  // Tentative de recuperation des identifants
  String ssid = readFile( "/ssid.txt");
  String password = readFile( "/password.txt");
  String local_ip = readFile( "/local_ip.txt");
  String passerelle = readFile( "/passerelle.txt");
  String masque = readFile( "/masque.txt");
  String dns = readFile( "/dns.txt");
  String hote = readFile( "/hote.txt");

  IPAddress staticIP;
  IPAddress gateway;
  IPAddress subnet;
  IPAddress DNS;

  staticIP.fromString( local_ip.c_str());
  gateway.fromString( passerelle.c_str());
  subnet.fromString( masque.c_str());
  DNS.fromString( dns.c_str());

  if( local_ip.length() > 0) WiFi.config(staticIP, gateway, subnet, DNS);
  if( hote.length() > 0) Hote = hote;
  if( ssid.length() > 0){
    
    #ifdef AP_STA
    WiFi.mode(WIFI_AP_STA);
    #endif
    
    WiFi.setHostname(Hote.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print( String("Connexion a ") + ssid);

    uint8_t attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    Serial.println();
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println( String("Connecté, IP: ") + WiFi.localIP().toString().c_str());

    #ifdef AP_STA
    WiFi.softAP( Hote.c_str(), password.c_str());
    #endif
    
    // Configuration de l'OTA
    ArduinoOTA.onStart([]() {
      Serial.println("Début de la mise à jour OTA...");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nFin de la mise à jour OTA");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progression: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Erreur [%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Erreur d'authentification");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Erreur de début");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Erreur de connexion");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Erreur de réception");
      else if (error == OTA_END_ERROR) Serial.println("Erreur de fin");
    });
    ArduinoOTA.setHostname(Hote.c_str());
    ArduinoOTA.begin();
    Serial.println("OTA demarre");
    
    return true; //wifi ok
    
  } else {
    Serial.println("Échec de la connexion au WiFi");
    WiFi.softAP( Hote.c_str(), NULL); //point d'accès sans mdp

    Serial.println("Point d'accès créé");
    Serial.println( String("http://") + WiFi.softAPIP().toString().c_str());
    return false; //active la page web de configuration wifi
  }
}

void loop_ota(){
  static uint32_t TimeOta;
  if( millis() - TimeOta < 2000) return;
  TimeOta = millis();
  
  Serial.println((String)"RAM: " + ESP.getFreeHeap());
  ArduinoOTA.handle();
  
#if defined(ESP32)
#ifdef AP_STA
  wifi_sta_list_t stationList;
 
  esp_wifi_ap_get_sta_list(&stationList);  
 
  Serial.print("N of connected stations: ");
  Serial.println(stationList.num);
 
  for(int i = 0; i < stationList.num; i++) {
 
    wifi_sta_info_t station = stationList.sta[i];
 
    for(int j = 0; j< 6; j++){
      char str[3];
 
      sprintf(str, "%02x", (int)station.mac[j]);
      Serial.print(str);
 
      if(j<5){
        Serial.print(":");
      }
    }
    Serial.println("-----------------");
  }
#endif
#endif
}

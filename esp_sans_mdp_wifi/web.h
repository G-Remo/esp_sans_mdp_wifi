#include <ESPAsyncWebServer.h>
#include "html.h"

AsyncWebServer server(80);

void restart_web(AsyncWebServerRequest *request){
  request->send(200, "text/plain", "Redemarrage ESP");
  delay(500);
  ESP.restart();
}

void reset_wifi(AsyncWebServerRequest *request){
  deleteFile( "/ssid.txt");
  request->send(200, "text/plain", "Supression config WiFi");
  delay(500);
  ESP.restart();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void init_web_ap(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", html_mdp);});
  server.on("/setwifi", HTTP_GET, [](AsyncWebServerRequest *request){
    const char* newSsid = request->arg("ssid").c_str();
    const char* newPassword = request->arg("password").c_str();
    const char* newPasserelle = request->arg("passerelle").c_str();
    const char* newMasque = request->arg("masque").c_str();
    const char* newLocal_ip = request->arg("local_ip").c_str();
    const char* newDns = request->arg("dns").c_str();
    const char* newHote = request->arg("hote").c_str();
    
    // Enregistrer les nouveaux paramètres WiFi dans la mémoire SPIFFS
    writeFile( "/ssid.txt", newSsid);
    writeFile( "/password.txt", newPassword);
    writeFile( "/passerelle.txt", newPasserelle);
    writeFile( "/masque.txt", newMasque);
    writeFile( "/local_ip.txt", newLocal_ip);
    writeFile( "/dns.txt", newDns);
    writeFile( "/hote.txt", newHote);

    request->send(200, "text/plain", "Parametres WiFi enregistrés");
    delay(500);
    ESP.restart();
  });
  server.onNotFound(notFound);
  server.begin();
}

void init_web(){
  server.on("/restart", HTTP_GET, restart_web);
  server.on("/reset_wifi", HTTP_GET, reset_wifi);
  server.onNotFound(notFound);
  server.begin();
}

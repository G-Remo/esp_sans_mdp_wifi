#include "spiffs.h"
#include "wifi.h"
#include "web.h"

void setup() {
  Serial.begin(115200);
  init_spiffs();
  
  if( !init_wifi()){
    init_web_ap();
    return;
  }
  
  init_web();
}

void loop_sta(){
  if( WiFi.status() != WL_CONNECTED) return;
  loop_ota();
}

void loop() {
  loop_sta();
}

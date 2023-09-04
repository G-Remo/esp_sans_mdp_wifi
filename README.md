# esp_sans_mdp_wifi
Connexion au reseau WiFi sans enregistrer ssid et password dans le code.
Lors du premier demarrage ou en cas d'échec de connexion, demarre en point d'acces (SSID: ESP ou ancien Nom et http://192.168.4.1) avec une page web pour enregistrer SSID et password.
Le tout est enregisté en spiffs ou littlefs.
Il y a l'OTA.
Donc dans le choix de partition prendre avec fs et OTA (ou supprimer l'OTA).
les champs à remplir: SSID, Mode passe, le reste est facultatif.

Inspiré de https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver/

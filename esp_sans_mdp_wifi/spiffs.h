#include <FS.h>
#if defined(ESP8266)
#include <LittleFS.h>
#define FILESYSTEM LittleFS
#else
#include <SPIFFS.h>
#define FILESYSTEM SPIFFS
#endif


void writeFile( const char * path, const char * message){
  Serial.printf("Ecrture de: %s -> ", path);

  File file = FILESYSTEM.open(path, "w");
  if(!file){
    Serial.println("erreur d'acces");
    return;
  }
  if(file.print(message)) Serial.println(message);
  else Serial.println("erreur d'ecriture");
  file.close();
}

String readFile(const char * path){
  Serial.printf("Lecture de: %s -> ", path);

  File file = FILESYSTEM.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("erreur d'acces");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  
  file.close();
  Serial.println( fileContent);
  return fileContent;
}

void deleteFile(const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(FILESYSTEM.remove(path)) Serial.println("File deleted");
  else                        Serial.println("Delete failed");
}

void init_spiffs() {
  #if defined(ESP8266)
  if( !FILESYSTEM.begin()) Serial.println("Pas de systeme de fichier\n Formatage ...");
  #else
  if( !FILESYSTEM.begin(true)) Serial.println("Pas de systeme de fichier\n Formatage ...");
  #endif
}

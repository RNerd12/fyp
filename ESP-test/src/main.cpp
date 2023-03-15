#include <SD.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#define SSID "tharang_home"
#define PASS "raj26@KT"
#define DB_URL "soilqualitydb-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDvuhWQSnAJNNcCjMOyuti-t9BPJoz4G1M"
FirebaseData fdbo;
FirebaseAuth auth;
FirebaseConfig config;
bool signup = false;
int isPush = 0;
void setup() {
  // put your setup code here, to run once:
  int ledPin = 13;
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID,PASS);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(". ");
  }
  Serial.println("Wifi connected");
  config.api_key = API_KEY;
  config.database_url = DB_URL;
  if(Firebase.signUp(&config,&auth,"","")){
    Serial.print("ok");
    signup = true;
  }
  else
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  for(int i=0;i<5;i++){
    Firebase.RTDB.setInt(&fdbo, "test/int", i);
    delay(2000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Still connected");
  float N, P, K, ph, temp, hum;
  putData(N,P,K,ph,temp,hum);
  delay(20000);
}

void putData(float N, float P, float K, float ph, float temp, float hum) {
  try{
    Firebase.RTDB.setFloat(&fdbo, "main/Nitrogen", N);
    Firebase.RTDB.setFloat(&fdbo, "main/Phosphorous", P);
    Firebase.RTDB.setFloat(&fdbo, "main/Potassium", K);
    Firebase.RTDB.setFloat(&fdbo, "main/pH", ph);
    Firebase.RTDB.setFloat(&fdbo, "main/Temperature", temp);
    Firebase.RTDB.setFloat(&fdbo, "main/Humidity", hum);
    Firebase.RTDB.setBool(&fdbo, "main/isPush", true);
  }
  catch(Exception e){
    Serial.println(e.toString());
    if(isPush<3){
      i++;
    }
    else{
      i = 0;
      Firebase.RTDB.setBool(&fdbo, "main/isPush", false)
    }
  }
}
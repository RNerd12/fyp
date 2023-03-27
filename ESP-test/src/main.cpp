#include <SD.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#define SSID "tharang1"
#define PASS "12345678"
#define DB_URL "soilqualitydb-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDvuhWQSnAJNNcCjMOyuti-t9BPJoz4G1M"
FirebaseData fdbo;
FirebaseAuth auth;
FirebaseConfig config;
bool signup = false;
int isPush = 0;
float N, P, K, ph, temp, hum;

void setup() {
  // put your setup code here, to run once:
  int ledPin = 13;
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(9600);
  Serial.print("Connecting to ");
  
  // connecting nodemcu to wifi
  Serial.println(SSID);
  WiFi.begin(SSID,PASS);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(". ");
  }
  Serial.println("Wifi connected");
  
  // connect nodemcu to firebase real time db
  config.api_key = API_KEY;
  config.database_url = DB_URL;
  if(Firebase.signUp(&config,&auth,"","")){
    Serial.print("firebase connected");
    signup = true;
  }
  else
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  
  // testing data push
  for(int i=0;i<5;i++){
    Firebase.RTDB.setInt(&fdbo, "test/int", i);
    delay(500);
  }
}

//Pushes data to Firebase RTDB
void putData(float N, float P, float K, float ph, float temp, float hum) {
  try{
    // try to push data to firebase db
    Firebase.RTDB.setFloat(&fdbo, "main/Nitrogen", N);
    Firebase.RTDB.setFloat(&fdbo, "main/Phosphorous", P);
    Firebase.RTDB.setFloat(&fdbo, "main/Potassium", K);
    Firebase.RTDB.setFloat(&fdbo, "main/pH", ph);
    Firebase.RTDB.setFloat(&fdbo, "main/Temperature", temp);
    Firebase.RTDB.setFloat(&fdbo, "main/Humidity", hum);
    Firebase.RTDB.setBool(&fdbo, "main/isPush", true);
  }
  catch(Exception e){
    // update ispush to false if push fails 3 times
    Serial.println(e.toString());
    if(isPush<3){
      i++;
    }
    else{
      isPush = 0;
      Firebase.RTDB.setBool(&fdbo, "main/isPush", false);
    }
  }
  else
    Serial.println("pushed to firebase");
}

float getNitrogen(){
  float N = 0;

  return N;
}

float getPhosphorus(){
  float P = 0;

  return P;
}

float getPotassium(){
  float K = 0;

  return K;
}

float getPh(){
  float ph = 0;

  return ph;
}

float getTemperature(){
  float temp = 0;

  return temp
}

float getHumidity(){
  float hum = 0;

  return hum;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Still connected");
  N = 1+random(10);
  P = 2+random(10);
  K = 3+random(10);
  ph = 4+random(10);
  temp = 5+random(10);
  hum = 6+random(10);
  putData(N,P,K,ph,temp,hum);
  delay(1000);
}
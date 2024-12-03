#include<ESP8266HTTPClient.h>
#include<ESP8266WiFi.h>
//#include<WiFiClient.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
WiFiClient client;
HTTPClient http;

const char* ssid="Redmi 11 Prime 5G";
const char* password="123456789";
String URL="http://192.168.3.74:8080/api/saveData?data=";
//const char* URL="http://192.168.1.36/GAURAV/project_patient_monitoring/sends_sensor_data_database.php";
String SURL="";
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;   // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int myBPM=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

    wifi_setup_connection();
 //  Configure the PulseSensor object, by assigning our variables to it. 

    pulse_sensor_setup();
   


}

void loop() {
 
  // put your main code here, to run repeatedly:
  pulse_sensor_running();


//   if(WiFi.status()==3){
// http_sending_request();}
// else{
//  //wifi_setup_connection();
// }
http_sending_request();

// delay(5000);

}

void wifi_setup_connection(){
  WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
  Serial.println("connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.printf(" \nWIfi connected successfull\n");
  Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

}

void pulse_sensor_setup(){
   pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);  
  

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.printf("We created a pulseSensor Object !\n");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
}

void pulse_sensor_running(){
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
 Serial.printf("\n♥️  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);   // Print the value inside of myBPM. 




 //http_request_post();
 
}
}

void http_sending_request(){
 
SURL="";
SURL=URL+(String)myBPM;
 if(WiFi.status()==WL_CONNECTED){
  
  http.begin(client,SURL);
  http.addHeader("content-Type","application/x-www-form-urlencoded");
 Serial.printf("\nwifi status= ");
  Serial.println(WiFi.status());
 
Serial.print("\n"+SURL+"\n");
 int httpResponseCode =http.GET();

 
 if(httpResponseCode>0){
  Serial.printf("\nHTTP Response code : ");
  Serial.print(httpResponseCode);
 }
else {
   Serial.printf("\nError code :");
  Serial.print(httpResponseCode);

}

http.end();
   
}

else {
   Serial.printf("\nwifi status= ");
  Serial.println(WiFi.status());
}
delay(2000);
}


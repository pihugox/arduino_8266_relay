/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "---";
const char* password = "---";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  // prepare GPIO2
  pinMode(2, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(2, 1);
  digitalWrite(0, 1);
  
  Serial.begin(115200);
  delay(10);
  
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //Static ip
  IPAddress ip(192,168,101,200);   
  IPAddress gateway(192,168,101,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void encenderRelay(){
  digitalWrite(2,LOW);           // Turns ON Relays 
   delay(2000);                                      
   digitalWrite(2,HIGH);          // Turns Relay Off
}

void encenderCamara(){
  digitalWrite(0,LOW);           // Turns ON Relays 
   delay(100);                                      
   digitalWrite(0,HIGH);          // Turns Relay Off
}


void loop() {
  //delay(50);
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    //val = 0;
    digitalWrite(2, 0);
  else if (req.indexOf("/gpio/1") != -1)
    //val = 1;
    digitalWrite(2, 1);
  else if (req.indexOf("/relay") != -1){
    encenderRelay();  
  }else if (req.indexOf("/camara") != -1){
    encenderCamara();  
  }else if(req.indexOf("/") != -1){
    // 
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  //digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  //String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel=\"icon\" href=\"data:,\">  </head><button onclick=\"window.location.href='../gpio/0'\">Encendido</button><button onclick=\"window.location.href='../gpio/1'\">Apagado</button><button onclick=\"window.location.href='relay'\">Parpadear</button></html>\n";

  // Send the response to the client
  //client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

 
}


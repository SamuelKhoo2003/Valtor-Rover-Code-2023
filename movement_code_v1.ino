#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <string> 
#include <WiFiWebServer.h>
const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 23; // Set your group number to make the IP address constant - only do this on the EEERover network
const int dir_left = 1; 
const int dir_right = 3;
const int en_left = 0; 
const int en_right = 2; 
//Logo Addition: 
//.logo {max-width: 300px;height: auto;margin: 20px;}\
//<img class=\"logo\" src=\"file:///C:/Users/samue/Desktop/EEE%20Rover%20Project/finalinterimcode/Screenshot%202023-06-01%20164712.png\" alt=\"Logo\">\

//Webpage to return when root is requested
const char webpage[] = "<html><head><style>.container{display:flex;flex-direction:column;align-items:center;}.row{display:flex;justify-content:center;}.btn{background-color:red;padding:20px 40px;font-size:25px;margin:15px;}.btn:hover{background:#eee;}</style></head><body><div class=\"container\"><div class=\"row\"><button class=\"btn\" onmousedown=\"d('forward')\" onmouseup=\"d('stop')\">&uarr;</button></div><div class=\"row\"><button class=\"btn\" onmousedown=\"d('left')\" onmouseup=\"d('stop')\">&larr;</button><button class=\"btn\" onmousedown=\"d('right')\" onmouseup=\"d('stop')\">&rarr;</button></div><div class=\"row\"><button class=\"btn\" onmousedown=\"d('reverse')\" onmouseup=\"d('stop')\">&darr;</button></div><br>DRIVE STATE: <span id=\"state\">STOP</span></body><script>var x=new XMLHttpRequest();x.onreadystatechange=function(){if(4==this.readyState&&200==this.status)document.getElementById(\"state\").innerHTML=this.responseText};function d(t){x.open(\"GET\",\"/\"+t);x.send()}</script></html>";

WiFiWebServer server(80);
//Return the web page
void handleRoot()
{
  server.send(200, F("text/html"), webpage);
}

void d_forward()
{
  digitalWrite(LED_BUILTIN,1);
  digitalWrite(dir_right, 1); 
  digitalWrite(dir_left, 1); 
  analogWrite(en_left, 255); 
  analogWrite(en_right, 255); 
  server.send(200, F("text/plain"), F("FORWARD"));
}

void d_reverse()
{
  digitalWrite(LED_BUILTIN,1);
  digitalWrite(dir_right, 0); 
  digitalWrite(dir_left, 0); 
  analogWrite(en_left, 255); 
  analogWrite(en_right, 255); 
  server.send(200, F("text/plain"), F("REVERSE"));
}
void d_stop()
{
  digitalWrite(LED_BUILTIN, 0); 
  digitalWrite(dir_right, 0); 
  digitalWrite(dir_left, 0); 
  analogWrite(en_right, 0); 
  analogWrite(en_left, 0); 
  server.send(200, F("text/plain"), F("STOP")); 
}
// after the interim presentation, I took the feedback to program the left and right wheels such that they spin in opposite directions during turning
// this would reduce the turning radius and speed up turning
void d_right()
{
  digitalWrite(LED_BUILTIN, 1); 
  digitalWrite(dir_right, 0); 
  digitalWrite(dir_left, 1); 
  analogWrite(en_right, 255); 
  analogWrite(en_left, 255); 
  server.send(200, F("text/plain"), F("RIGHT")); 
}
void d_left()
{
  digitalWrite(LED_BUILTIN, 1); 
  digitalWrite(dir_right, 1); 
  digitalWrite(dir_left, 0); 
  analogWrite(en_right, 255); 
  analogWrite(en_left, 255); 
  server.send(200, F("text/plain"), F("LEFT")); 
}
//Generate a 404 response with details of the failed request
void handleNotFound()
{
  String message = F("File Not Found\n\n"); 
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
}
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  pinMode(2, OUTPUT); 
  pinMode(0, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(1, OUTPUT); 

  //Use baud 115200 as it is higher ping rate and more responsive compared to 9600 
  Serial.begin(115200);
  //Wait 10s for the serial connection before proceeding
  //This ensures you can see messages from startup() on the monitor
  //Remove this for faster startup when the USB host isn't attached
  while (!Serial && millis() < 5000);  
  Serial.println(F("\nStarting Web Server"));
  //Check WiFi shield is present
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("WiFi shield not present"));
    while (true);
  }
  //Configure the static IP address if group number is set
  if (groupNumber)
    WiFi.config(IPAddress(192,168,0,groupNumber+1));
  // attempt to connect to WiFi network
  Serial.print(F("Connecting to WPA SSID: "));
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }
  //Register the callbacks to respond to HTTP requests
  server.on(F("/"), handleRoot);
  server.on(F("/forward"), d_forward);
  server.on(F("/reverse"), d_reverse);
  server.on(F("/stop"), d_stop); 
  server.on(F("/right"), d_right); 
  server.on(F("/left"), d_left); 
  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));
}
//Call the server polling function in the main loop
void loop()
{
  server.handleClient();
}
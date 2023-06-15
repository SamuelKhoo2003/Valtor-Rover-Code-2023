#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <string> 
#include <WiFiWebServer.h>
#include <Keyboard.h> 

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 23; // Set your group number to make the IP address constant - only do this on the EEERover network
const int age_pin = 4; 
const int magnet_input_pin1 = 12; 
const int magnet_input_pin2 = 13; 
const int name_pin = 8; 
const int dir_left = 1; 
const int dir_right = 3;
const int en_left = 0; 
const int en_right = 2; 
//Webpage to return when root is requested
const char webpage0[] = 
"<html><head><style>"
".container {display: flex; flex-direction: column; align-items: center;}"
".row {display: flex; justify-content: center;}"
".btn {padding: 20px 40px; font-size: 25px; margin: 15px;}"
".btn-movement {background-color: red;}"
".btn-sensor {background-color: green;}"
".btn:hover {background: #eee;}"
".result {margin-top: 10px;}"
".btn-group {display: flex; justify-content: space-between;}"
"</style></head><body>"; 

const char webpage1[] = 
"<div class=\"container\">"
"<div class=\"row btn-group\">"
"<div class=\"btn-movement\">"
"<button class=\"btn\" onmousedown=\"sendCommand('forward')\" onmouseup=\"sendCommand('stop')\" onmouseleave=\"sendCommand('stop')\">&uarr;</button>"
"</div>"
"</div>"
"<div class=\"row btn-group\">"
"<div class=\"btn-movement\">"
"<button class=\"btn\" onmousedown=\"sendCommand('left')\" onmouseup=\"sendCommand('stop')\" onmouseleave=\"sendCommand('stop')\">&larr;</button>"
"<button class=\"btn\" onmousedown=\"sendCommand('right')\" onmouseup=\"sendCommand('stop')\" onmouseleave=\"sendCommand('stop')\">&rarr;</button>"
"</div>"
"</div>"
"<div class=\"row btn-group\">"
"<div class=\"btn-movement\">"
"<button class=\"btn\" onmousedown=\"sendCommand('reverse')\" onmouseup=\"sendCommand('stop')\" onmouseleave=\"sendCommand('stop')\">&darr;</button>"
"</div>"
"</div>"
"<div class=\"row btn-group\">"
"<div class=\"btn-sensor\">"
"<button class=\"btn\" onclick=\"startDetect('age')\">Start Age Detection</button>"
"<div id=\"ageResult\" class=\"result\">N/A</div>"
"</div>"
"</div>"; 

const char webpage2[] = 
"<div class=\"row btn-group\">"
"<div class=\"btn-sensor\">"
"<button class=\"btn\" onclick=\"startDetect('magnetism')\">Start Magnetism Detection</button>"
"<div id=\"magnetismResult\" class=\"result\">N/A</div>"
"</div>"
"</div>"
"<div class=\"row btn-group\">"
"<div class=\"btn-sensor\">"
"<button class=\"btn\" onclick=\"startDetect('name')\">Start Name Detection</button>"
"<div id=\"nameResult\" class=\"result\">N/A</div>"
"</div>"
"</div>"
"</div></body>"
"<script>"; 

const char webpage3[] = 
"function startDetect(type) {"
"var xhttp = new XMLHttpRequest();"
"xhttp.onreadystatechange = function() {"
"if (this.readyState === 4 && this.status === 200) {"
"var response = JSON.parse(this.responseText);"
"document.getElementById(type + 'Result').innerHTML = 'Result: ' + response.state;"
"}"
"};"
"xhttp.open('GET', '/detect?type=' + type, true);"
"xhttp.send();"
"}"
"function sendCommand(command) {"
"var xhttp = new XMLHttpRequest();"
"xhttp.onreadystatechange = function() {"
"if (this.readyState === 4 && this.status === 200) {"
"console.log('Command sent: ' + command);"
"}"
"};"
"xhttp.open('GET', '/command?type=' + command, true);"
"xhttp.send();"
"}"
"</script>"
"</html>";



WiFiWebServer server(80);
//Return the web page
void handleRoot()
{
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); 
  server.sendContent(webpage0); 
  server.sendContent(webpage1); 
  server.sendContent(webpage2); 
  server.sendContent(webpage3); 
  server.client().stop(); 
}

//Switch LED on and acknowledge
void d_forward()
{
  digitalWrite(LED_BUILTIN,1);
  digitalWrite(3, 1); 
  digitalWrite(1, 1); 
  analogWrite(2, 255); 
  analogWrite(0, 255); 
}
//Switch LED off and acknowledge
void d_reverse()
{
  digitalWrite(LED_BUILTIN,1);
  digitalWrite(3, 0); 
  digitalWrite(1, 0); 
  analogWrite(2, 255); 
  analogWrite(0, 255); 
}
void d_stop()
{
  digitalWrite(LED_BUILTIN, 0); 
  digitalWrite(3, 0); 
  digitalWrite(1, 0); 
  analogWrite(2, 0); 
  analogWrite(0, 0); 
}
// after the interim presentation, I took the feedback to program the left and right wheels such that they spin in opposite directions during turning
// this would reduce the turning radius and speed up turning
void d_right()
{
  digitalWrite(LED_BUILTIN, 1); 
  digitalWrite(3, 0); 
  digitalWrite(1, 1); 
  analogWrite(2, 255); 
  analogWrite(0, 255);  
}
void d_left()
{
  digitalWrite(LED_BUILTIN, 1); 
  digitalWrite(3, 1); 
  digitalWrite(1, 0); 
  analogWrite(2, 255); 
  analogWrite(0, 255); 
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
  pinMode(A0, OUTPUT); 
  pinMode(12, INPUT); 
  pinMode(13, INPUT); 
  pinMode(8, INPUT); 
  pinMode(4, INPUT); 
  //Use baud 115200 as it is higher ping rate and more responsive compared to 9600 
  Serial.begin(9600);

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
    delay(5000);
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
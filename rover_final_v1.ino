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

void detect_age()
{
  unsigned long alien_age = 0;
  unsigned long sum = 0; 
  unsigned long period = 0; 
  String age_result = "N/A"; 
  while(digitalRead(age_pin) == HIGH){}
    for( int i = 0; i < 100; i++)
    {
      unsigned long ontime = pulseIn(4, HIGH, 3000000UL);
      unsigned long offtime = pulseIn(4,LOW, 3000000UL);
      unsigned long pulseDuration = ontime + offtime;
      sum = sum + pulseDuration;
    }
    period = sum / 100; 
    alien_age = period / 10; 

    if(alien_age != 0){
    age_result = String(alien_age); 
    }
  server.send(200, "application/json", "{\"state\": \"" + age_result + "\"}");
  Serial.println(age_result); 
}

void detect_magnetism()
{
  String magnet_result = "No magnetic field detected"; 
  digitalWrite(A0, HIGH);
  int inputState1 = digitalRead(magnet_input_pin1);
  int inputState2 = digitalRead(magnet_input_pin2);
  
  if (inputState1 == HIGH) {
    magnet_result = "DOWN"; 
  }

  else if (inputState2 == HIGH) {
    magnet_result = "UP"; 
  }

  server.send(200, "application/json", "{\"state\": \"" + magnet_result + "\"}");
  Serial.println(magnet_result); 

  digitalWrite(A0, LOW); 
  //short delay allow for switching between 0 and 3.3V output
  delay(1500); 
}

void detect_name()
{
  String name_result = "N/A"; 
  char temp[4] = {0, 0, 0, 0}; 
  while(digitalRead(name_pin) == HIGH){}
    delayMicroseconds(1200000 / 600); //when the start bit is detected, apply a delay before reading the character
      for (int i = 0; i < 8; i++) {
        temp[0] |= (digitalRead(8) << i); //each time we read a bit we shift it left by i (because each character is written in reverse order)
        delayMicroseconds(1000000 / 600); //this delay is to allow a suitable transition from bit to bit before attempting to read each bit
      }
  
    delayMicroseconds(2000000 / 600);
      for (int i = 0; i < 8; i++) {
        temp[1] |= (digitalRead(8) << i);
        delayMicroseconds(1000000 / 600);
      }

    delayMicroseconds(2000000 / 600);
      for (int i = 0; i < 8; i++) {
        temp[2] |= (digitalRead(8) << i);
        delayMicroseconds(1000000 / 600);
      }

    delayMicroseconds(2000000 / 600);
      for (int i = 0; i < 8; i++) {
        temp[3] |= (digitalRead(8) << i);
        delayMicroseconds(1000000 / 600);
      }

    delayMicroseconds(1000000 / 600);
    name_result = String(temp[0]) + String(temp[1]) + String(temp[2]) + String(temp[3]); 

  server.send(200, "application/json", "{\"state\": \"" + name_result + "\"}");
  Serial.println(name_result); 
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

void handleDetect() {
  String type = server.arg("type");
  
  if (type == "age") {
    detect_age();
  } else if (type == "magnetism") {
    detect_magnetism();
  } else if (type == "name") {
    detect_name();
  } else {
    server.send(400, "text/plain", "Invalid detect type");
  }
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
  server.on(F("/detect"), handleDetect);
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
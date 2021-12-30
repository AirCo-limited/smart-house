#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid     = "test";
const char* password = "12345678abc";

WiFiServer server(80);

String header;
String output5State = "off";
String output4State = "off";
String output2State = "off";
String tp="";

const int output5 = 5;
const int output4 = 4;
const int output2 = 2;
int light = 50;
int temp = 37;
int scold = 20;
int sheat = 15;
int sligh = 50;
int i = 0;
int AR = 0;

unsigned long currentTime = millis();
unsigned long ct = 0;
unsigned long previousTime = 0; 
// milliseconds ( 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output2, OUTPUT);
  
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output2, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("HTTP server started");
}

void loop(){

  if ( currentTime - ct > 10000){
    ct = currentTime;
    AR = analogRead(A0);
    light = AR / 11;
    temp = AR / 20;
  }
  
  
  if (output5State == "auto"){
    if(scold >= temp){
      digitalWrite(output5, HIGH);
    }
    else{
      digitalWrite(output5, LOW);
    }
  }
  if (output4State == "auto"){
    if(sheat >= temp){
      digitalWrite(output4, LOW);
    }
    else{
      digitalWrite(output4, HIGH);
    }
  }
  if (output2State == "auto"){
    if(sligh >= light){
      digitalWrite(output2, LOW);
    }
    else{
      digitalWrite(output2, HIGH);
    }
  }
  
  WiFiClient client = server.available();   

  if (client) {                            
    Serial.println("New Client.");          
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                    
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            }
            
            else if (header.indexOf("GET /5/auto") >= 0) {
              Serial.println("GPIO 5 auto");
              output5State = "auto";
            }
            
            else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } 
      //----------------------------------------------------------      
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            }
            
            else if (header.indexOf("GET /4/auto") >= 0) {
              Serial.println("GPIO 4 auto");
              output4State = "auto";
            }
            
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }
      //--------------------------------------------------------      
            else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
              digitalWrite(output2, LOW);
            }
            
            else if (header.indexOf("GET /2/auto") >= 0) {
              Serial.println("GPIO 2 auto");
              output2State = "auto";
            }
            
            else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, HIGH);
            }
      //-----------------------------------------------------
          if (header.indexOf("GET /action_page?cold") >= 0) { 
            Serial.println(" -- ");
            i = header.indexOf("GET /action_page?cold");
            tp = "";
            tp += header[i+22];
            if ( header[i+23] != ' ')
              tp += header[i+23];
            Serial.println(tp);
            scold = tp.toInt();
          }
          if (header.indexOf("GET /action_page?heat") >= 0) {
            Serial.println(" -- ");
            i = header.indexOf("GET /action_page?heat");
            tp = "";
            tp += header[i+22];
            if ( header[i+23] != ' ')
              tp += header[i+23];
            Serial.println(tp);
            sheat = tp.toInt();
          }
          if (header.indexOf("GET /action_page?ligh") >= 0) {
            Serial.println(" -- ");
            i = header.indexOf("GET /action_page?ligh");
            tp = "";
            tp += header[i+22];
            if ( header[i+23] != ' ')
              tp += header[i+23];
            Serial.println(tp);
            sligh = tp.toInt();
          }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css\">");
            client.println("<body style=\"background-color:#ddc888;\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".h1 {text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}");
            client.println(".button3 {background-color: #f44336;}</style></head>");
            
            client.println("<body><h1 style=\"font-family:courier; text-align: center;\"><b>Smart Control</b></h1>");

            client.println("<div class=\"d-flex flex-column\">");
            client.println("<div class=\"d-flex flex-row justify-content-center\">");
            tp = String(temp);
            client.println("<div class=\"p-1\"><h3 style=\"text-align: center;\">Temperature </h3><div style=\"align-items: center;\"class=\"p-1\"><div role=\"progressbar\" id=\"c1\" aria-valuenow=\"25\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"--value:"+tp+"\"></div></div></div>");
            tp = String(light);
            client.println("<div class=\"p-1\"><div class=\"circle-wrap\"><h3 style=\"text-align: center;\">Light </h3><div class=\"p-1\"><div role=\"progressbar\" id=\"c2\" aria-valuenow=\"35\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"--value:"+tp+"\"></div></div></div></div></div></div></div>");
    
            //----------------------------------------------------------------------------------------------------
            


            
            // GPIO 5  
            client.println("<h1 style=\"text-align: center;\"> COOLER ");      
            if (output5State=="off") {
              client.println("<a href=\"/5/on\"><button class=\"button button2\">OFF</button></a>");
            }
            else if (output5State=="on"){
              client.println("<a href=\"/5/auto\"><button class=\"button\">ON</button></a>");
            }
            else {
              client.println("<a href=\"/5/off\"><button class=\"button button3\">AUTO</button></a>");
              client.println("<form action=\"/action_page\">");
              client.println("<label for=\"cold\">tempeture (1 - 99):</label>");
              tp = String(scold);
              client.println("<input type=\"number\" id=\"cold\" name=\"cold\" value=" + tp + " min=\"1\" max=\"99\">");
              client.println("<input class=\"button\" type=\"submit\" value=\"send\">");
              client.println(" </form>");
            } 
            client.println(" </h1>");
             //-------------------------------------------------------------------------------------------------- 
              
            // GPIO 4  
            client.println("<h1 style=\"text-align: center;\">HEATER ");      
            if (output4State=="off") {
              client.println("<a href=\"/4/on\"><button class=\"button button2\">OFF</button></a>");
            } 
            else if (output4State=="on"){
              client.println("<a href=\"/4/auto\"><button class=\"button\">ON</button></a>");
            }
            else {
              client.println("<a href=\"/4/off\"><button class=\"button button3\">AUTO</button></a>");
              client.println("<form action=\"/action_page\">");
              client.println("<label for=\"heat\">tempeture (1 - 99):</label>");
              tp = String(sheat);
              client.println("<input type=\"number\" id=\"heat\" name=\"heat\" value=" + tp + " min=\"1\" max=\"99\">");
              client.println("<input class=\"button\" type=\"submit\" value=\"send\">");
              client.println(" </form>");
            }
            client.println(" </h1>");
            //--------------------------------------------------------------------------------------------------

            
            // GPIO 2 
            client.println("<h1 style=\"text-align: center;\">LAMP ");     
            if (output2State=="off") {
              client.println("<a href=\"/2/on\"><button class=\"button button2\">OFF</button></a>");
            } 
            else if (output2State=="on"){
              client.println("<a href=\"/2/auto\"><button class=\"button\">ON</button></a>");
            }
            else {
              client.println("<a href=\"/2/off\"><button  class=\"button button3\">AUTO</button></a>");
              client.println("<form action=\"/action_page\">");
              client.println("<label for=\"ligh\">light (1 - 99):</label>");
              tp = String(sligh);
              client.println("<input type=\"number\" id=\"ligh\" name=\"ligh\" value=" + tp + " min=\"1\" max=\"99\">");
              client.println("<input class=\"button\" type=\"submit\" value=\"send\">");
              client.println(" </form>");
            } 
            client.println(" </h1>");
            client.println("<script>;var sliderTemp=document.getElementById('myTemp'),TempValue=document.getElementById('TempValue');TempValue.innerHTML=sliderTemp.value;sliderTemp.oninput=function(){TempValue.innerHTML=this.value};var sliderLight=document.getElementById('myLight'),LightValue=document.getElementById('LightValue');LightValue.innerHTML=sliderLight.value;sliderLight.oninput=function(){LightValue.innerHTML=this.value};</script>");
            client.println("<style>.sliderForValue { -webkit-appearance: none; width: 100%; height: 15px; border-radius: 5px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; } .sliderForValue:hover { opacity: 1; } .sliderForValue::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 50%; background: #8caa04; cursor: pointer; } .sliderForValue::-moz-range-thumb { width: 25px; height: 25px; border-radius: 50%; background: #9faa04; cursor: pointer; } .setting-box{ width: 100%; height: 300px; } .very-big-box{ width: 100%; height: 300px; } .bigBox{ resize: both; overflow: auto; } .boxRow{ display: flex; flex-direction: row; flex: 1; padding: 10px; } .sliderBox{ flex: 1; background: whitesmoke; border-radius: 15px; margin: auto; width: 50%; padding: 10px; } .center{ width: 50%; margin: auto; } .switchBox{ flex: 1;background: whitesmoke; margin-left: 10px; border-radius: 15px; width: 50%; } @keyframes growProgressBar { 0%, 33% { --pgPercentage: 0; } 100% { --pgPercentage: var(--value); } } @property --pgPercentage { syntax: '<number>'; inherits: false; initial-value: 0; } div[role=\"progressbar\"] { --me : 100%; --size: 12rem; --fg: rgb(153, 51, 139); --bg: #def; --pgPercentage: var(--value); animation: growProgressBar 3s 1 forwards; width: var(--size); height: var(--size); border-radius: 50%; display: grid; place-items: center; background: radial-gradient(closest-side, white 80%, transparent 0 99.9%, white 0), conic-gradient(var(--fg) calc(var(--pgPercentage) * 1%), var(--bg) 0) ; font-family: Helvetica, Arial, sans-serif; font-size: calc(var(--size) / 5); color: var(--fg); } div[role=\"progressbar\"]::before { counter-reset: percentage var(--value); content: counter(percentage); }</style>");
            
            client.println("</body></html>");
            
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

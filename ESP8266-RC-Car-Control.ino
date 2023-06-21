#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ssid";
const char* password = "password";

AsyncWebServer server(80);

const int motorLeft1 = D1;
const int motorLeft2 = D2;
const int motorRight1 = D3;
const int motorRight2 = D4;

// HTML and JavaScript for the webpage
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<body>
  <h2>RC Car Control w/Incremental Steering</h2>
  <p id="status">Status: Idle</p>
  <div style="display: flex; justify-content: center; align-items: center; height: 300px;">
    <svg id="rotating-vector" width="200" height="200">
      <circle cx="100" cy="100" r="80" fill="none" stroke="black"/>
      <line id="vector-line" x1="100" y1="100" x2="100" y2="20" stroke="red" stroke-width="2" />
    </svg>
  </div>
  <script>
    let angle = 0;
    const line = document.getElementById('vector-line');
    const statusText = document.getElementById('status');
    const rotateSpeed = 0.2;  // Change rotation speed here

    window.addEventListener("keydown", (event) => {
      const x = event.key;
      switch (x) {
        case 'ArrowUp':
          fetch(`/move?angle=${angle}`);
          statusText.textContent = 'Status: Moving Forward';
          break;
        case 'ArrowDown':
          fetch(`/move?angle=${angle}`);
          statusText.textContent = 'Status: Moving Backward';
          break;
        case 'ArrowLeft':
          fetch(`/move?angle=${angle}`);
          statusText.textContent = 'Status: Turning Left';
          angle -= rotateSpeed;
          break;
        case 'ArrowRight':
          fetch(`/move?angle=${angle}`);
          statusText.textContent = 'Status: Turning Right';
          angle += rotateSpeed;
          break;
      }

      line.setAttribute('transform', `rotate(${angle * 180 / Math.PI} 100 100)`);
    }, false);
  </script>
</body></html>
)rawliteral";


void setup() {
  Serial.begin(115200);
  pinMode(motorLeft1, OUTPUT);
  pinMode(motorLeft2, OUTPUT);
  pinMode(motorRight1, OUTPUT);
  pinMode(motorRight2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  // Route for handling movement
  server.on("/move", HTTP_GET, [](AsyncWebServerRequest *request){
      String message;
      float angle;
    if (request->hasParam("angle")) {
      message = request->getParam("angle")->value();
      // Now the 'message' string contains the value of 'angle'
      angle = message.toFloat() * 180 / M_PI;
      Serial.println(angle, 2);

      // Do something with it (send to servo/motors)
      
    }
    request->send(200);
  });



  server.begin();
}

void loop(){
}

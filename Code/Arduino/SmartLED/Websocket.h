void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_DISCONNECTED: Serial.printf("[%u] Disconnected!\n", num); break;
    case WStype_CONNECTED: {
        IPAddress ip = websocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        String message = "Server Connected!";
        websocket.sendTXT(num, message);
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[%u] get Text: %s\n", num, payload);
        String message = String((char*)( payload));

        DynamicJsonDocument doc(100);
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        if (doc.containsKey("X")) {
          DrawX = doc["X"], DrawY = doc["Y"];
          if (doc["LED"] == "ON")isShow = true;
          else isShow = false;
        }
        else if (doc.containsKey("Direction")) {
          Snake.Direction(doc["Direction"]);
        }
        else if (doc.containsKey("R")) {
          MyColor = CRGB(doc["R"], doc["G"], doc["B"]);
          DrawX = LEDMatrix.Width(), DrawX = LEDMatrix.Height();
        }
        else if (doc.containsKey("Mode")) {
          Mode = doc["Mode"];
          isShow = false;
          FastLED.clear();
        }
        websocket.broadcastTXT(message);
      }
      break;
  }
}

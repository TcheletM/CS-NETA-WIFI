//================= functionsWebSockets ===================
// מצב לוח יחיד: השלט עצמו מריץ שרת WebSocket (לא מתחבר כלקוח למכשיר אחר).
// הדפדפן מתחבר ישירות לשרת הזה כדי לקבל את נתוני השלט בזמן אמת.
void initWebSocket() {
  wsServer.begin();
  wsServer.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("Browser #%u connected\n", num);
    functionsForPrintingAmessage(0, 30, "Browser Linked");
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("Browser #%u disconnected\n", num);
  }
}
//===========================================================

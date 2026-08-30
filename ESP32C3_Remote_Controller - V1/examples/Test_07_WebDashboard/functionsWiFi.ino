//================= functionsWiFi ===================
// מצב לוח יחיד: השלט עצמו משדר AP (במקום להתחבר כתחנה לרשת חיצונית).
void initAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  esp_wifi_set_ps(WIFI_PS_NONE); // ביטול מצב שינה של מודול התקשורת

  IPAddress ip = WiFi.softAPIP();
  String ipAddress = ip.toString();

  String line1 = ipAddress;
  String line2 = "";
  int dotPos2 = -1, dots = 0;

  for (int i = 0; i < ipAddress.length(); i++) {
    if (ipAddress[i] == '.') {
      dots++;
      if (dots == 2) { dotPos2 = i; break; }
    }
  }
  if (dotPos2 > 0) {
    line1 = ipAddress.substring(0, dotPos2 + 1);
    line2 = ipAddress.substring(dotPos2 + 1);
  }

  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(0, 10, "AP Ready!");
  display.drawStr(0, 20, line1.c_str());
  display.drawStr(0, 30, line2.c_str());
  display.sendBuffer();

  Serial.print("AP SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP: ");
  Serial.println(ipAddress);

  delay(1500); // רק כדי שיהיה זמן לקרוא את הכתובת לפני ש-drawStatus() ידרוס את המסך
}
//===================================================

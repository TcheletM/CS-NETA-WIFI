void setup() {
  Serial.begin(115200);

  // Wait for Serial
  while (!Serial) {
    delay(10);
  }

  delay(1000);
  Serial.println("TEST START");
}

void loop() {
  Serial.println("HELLO");
  delay(1000);
}

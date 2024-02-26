const int LEDPIN1 = 22;
const int LEDPIN2 = 23;
const int PushButton = 15;
unsigned long buttonPressTime = 0;
bool led2State = LOW;

void setup() {
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(PushButton, INPUT);
}

void loop() {
  int Push_button_state = digitalRead(PushButton);

  if (Push_button_state == HIGH) {
    // Tombol ditekan
    buttonPressTime = millis(); // Ambil waktu tombol ditekan
    digitalWrite(LEDPIN1, HIGH);
    digitalWrite(LEDPIN2, LOW);
  } else {
    // Tombol tidak ditekan
    digitalWrite(LEDPIN1, LOW); // Nyalakan LED pertama
    if (millis() - buttonPressTime > 5000) {
      // Jika tombol telah ditekan lebih dari 5 detik
      led2State = HIGH; // Nyalakan LED kedua
    } else {
      led2State = LOW; // Matikan LED kedua
    }
    digitalWrite(LEDPIN2, led2State); // Update status LED kedua
  }
}

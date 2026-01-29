#include <Servo.h> // Biblioteca pentru controlul servo-motorului

Servo myservo; // Creăm un obiect de tip Servo pentru barieră
int IR1 = 2; // Pinul pentru primul senzor IR (intrare în parcare)
int IR2 = 3; // Pinul pentru al doilea senzor IR (ieșire din parcare)
int Slot = 4; // Numărul total de locuri de parcare disponibile
const int maxSlots = 4; // Maximum slots available

// Pinii conectați la segmentele afișajului 7 segmente (a, b, c, d, e, f, g)
int segmentPins[] = {8, 9, 10, 11, 12, 13, A0};

// Configurația segmentelor pentru afișarea cifrelor 0-9
byte digits[] = {
  0b00111111, // 0: Segmentele a, b, c, d, e, f aprinse
  0b00000110, // 1: Segmentele b, c aprinse
  0b01011011, // 2: Segmentele a, b, d, e, g aprinse
  0b01001111, // 3: Segmentele a, b, c, d, g aprinse
  0b01100110, // 4: Segmentele b, c, f, g aprinse
  0b01101101, // 5: Segmentele a, c, d, f, g aprinse
  0b01111101, // 6: Segmentele a, c, d, e, f, g aprinse
  0b00000111, // 7: Segmentele a, b, c aprinse
  0b01111111, // 8: Toate segmentele aprinse
  0b01101111  // 9: Segmentele a, b, c, d, f, g aprinse
};

void setup() {
  Serial.begin(9600); // Inițializare port serial pentru debugging

  pinMode(IR1, INPUT); // Configurăm senzorul IR1 ca intrare
  pinMode(IR2, INPUT); // Configurăm senzorul IR2 ca intrare
  myservo.attach(4);   // Atașăm servo-motorul la pinul 4
  myservo.write(120);  // Inițial setăm bariera în poziție închisă (120 grade)

  // Configurăm pinii pentru afișajul 7 segmente ca ieșiri
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW); // Inițial toate segmentele sunt stinse
  }

  // Afișăm numărul inițial de locuri disponibile
  displayDigit(Slot);
}

void loop() {
  // Citim valorile senzorilor IR și afișăm în monitorul serial
  int val1 = digitalRead(IR1);
  int val2 = digitalRead(IR2);
  Serial.print("IR1: ");
  Serial.println(val1);
  Serial.print("IR2: ");
  Serial.println(val2);

  // Detectăm o mașină la intrare
  if (val1 == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(IR1) == LOW) { // Verificare dublă pentru intrare validă
      if (Slot > 0) { // Verificăm dacă mai sunt locuri disponibile
        myservo.write(20); // Deschidem bariera (20 grade)
        Slot = Slot - 1; // Reducem numărul de locuri disponibile
        displayDigit(Slot); // Actualizăm afișajul
        Serial.println("Car entering, barrier opened");
        delay(2000); // Așteptăm 2 secunde înainte de închiderea barierei
        myservo.write(120); // Închidem bariera (120 grade)
        Serial.println("Barrier closed after delay");
      } else {
        Serial.println("Parking full");
      }
    }
  }

  // Detectăm o mașină la ieșire
  if (val2 == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(IR2) == LOW) { // Verificare dublă pentru ieșire validă
      if (Slot < maxSlots) { // Verificăm dacă numărul de sloturi nu depășește limita
        myservo.write(20); // Deschidem bariera (20 grade)
        Slot = Slot + 1; // Creștem numărul de locuri disponibile
        displayDigit(Slot); // Actualizăm afișajul
        Serial.println("Car exiting, barrier opened");
        delay(2000); // Așteptăm 2 secunde înainte de închiderea barierei
        myservo.write(120); // Închidem bariera (120 grade)
        Serial.println("Barrier closed after delay");
      } else {
        alarm(); // Trigger alarm if slots are out of bounds
      }
    }
  }

  // Check if slot count goes below 0
  if (Slot < 0) {
    alarm(); // Trigger alarm if slots are out of bounds
  }
}

// Funcția pentru afișarea unei cifre pe afișajul 7 segmente
void displayDigit(int num) {
  if (num < 0 || num > 9) return; // Dacă numărul nu e între 0 și 9, nu facem nimic

  byte segments = digits[num]; // Obținem configurația segmentelor pentru cifra dorită
  for (int i = 0; i < 7; i++) {
    // Aprindem sau stingem fiecare segment în funcție de configurație
    digitalWrite(segmentPins[i], (segments >> i) & 0x01);
  }
}

// Funcția pentru alarma pe afișajul 7 segmente
void alarm() {
  Serial.println("Alarm: Slot count out of bounds!");
  for (int i = 0; i < 3; i++) { // Flash alarm 3 times
    for (int j = 0; j < 7; j++) {
      digitalWrite(segmentPins[j], HIGH);
    }
    delay(500);
    for (int j = 0; j < 7; j++) {
      digitalWrite(segmentPins[j], LOW);
    }
    delay(500);
  }
  // After flashing alarm, display current slot count
  displayDigit(Slot);
}

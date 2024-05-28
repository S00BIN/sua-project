const int sensorPin = 2;
volatile unsigned int pulseCount = 0;
unsigned long lastTime = 0;
unsigned int rpm = 0;

void countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 500) {
    detachInterrupt(digitalPinToInterrupt(sensorPin));

    rpm = pulseCount * 60;
    pulseCount = 0;
    lastTime = currentTime;

    Serial.print("RPM: ");
    Serial.println(rpm);

    attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING);
  }
  
}

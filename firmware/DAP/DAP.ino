const int buzzerPin = 27;
const int encoderCenterButtonPin = 15;

int encoderCenterButtonState = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(encoderCenterButtonPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  encoderCenterButtonState = digitalRead(encoderCenterButtonPin);
  if (encoderCenterButtonState == HIGH) {
    tone(buzzerPin, 1000, 1000);
    delay(3000);
  }



  tone(buzzerPin, 1000, 1000);
  delay(3000);



}

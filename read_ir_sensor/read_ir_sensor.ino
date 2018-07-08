
#define LED D4
#define SENSOR1 D5
#define SENSOR2 D6

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor1 = digitalRead(SENSOR1);
  
  Serial.println(sensor1);
  
  if(sensor1 == HIGH) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  delay(10);
}

void blink() {
  
}


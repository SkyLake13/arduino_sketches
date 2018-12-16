#ifndef StatusLED_H
#define StatusLED_H

class StatusLED {
  private:
    int pin;
    int _delay = 3000;
  public:
    StatusLED(int pin) {
      this->pin = pin;
      pinMode(this->pin, OUTPUT);
    }

    void shortBlink() {
      digitalWrite(this->pin, HIGH);
      delay(this->_delay);
      
      digitalWrite(this->pin, LOW);
      delay(100);
    }

    void longBlink() {
      digitalWrite(this->pin, HIGH);
      delay(this->_delay);
      
      digitalWrite(this->pin, LOW);
      delay(500);
    }

    void strobe() {
      digitalWrite(this->pin, HIGH);
      delay(100);
      digitalWrite(this->pin, LOW);
      delay(100);
    }
};

#endif


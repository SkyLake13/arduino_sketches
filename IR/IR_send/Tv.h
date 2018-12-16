#include <IRremoteESP8266.h>
#include <IRsend.h>

class Tv {
  private:
    String name;
    int frequency;
    IRsend & irSend;
  
  public:
    Tv(IRsend & _irSend): irSend(_irSend) {
      this->frequency = 32;
      this->name = "Tv";
    }

    void powerToggle() {
      uint64_t data = 0x10EFEB14;
    
      this->sendIR(data);
    }

    void changeSource() {
      uint64_t data = 0x10EFC33C;
      
      this->sendIR(data);
    }


  private:
    void sendIR(uint64_t data) {    
      irSend.sendNEC(data, this->frequency);
    }
};


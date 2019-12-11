#ifndef SkylakeWifi_h
#define SkylakeWifi_h

class SkylakeWifi
{
  public:
    SkylakeWifi();
    ~SkylakeWifi();
    void connect();
    void disconnect();
    
  private:
    char* _ssid;
    char* _password;
};

#endif

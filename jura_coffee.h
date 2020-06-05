#include "esphome.h"

class JuraCoffee : public PollingComponent, public UARTDevice {
 Sensor *xsensor1 {nullptr};
 Sensor *xsensor2 {nullptr};
 Sensor *xsensor3 {nullptr};
 Sensor *xsensor4 {nullptr};
 Sensor *xsensor5 {nullptr};

 public:
  JuraCoffee(UARTComponent *parent, Sensor *sensor1, Sensor *sensor2, Sensor *sensor3, Sensor *sensor4, Sensor *sensor5) : UARTDevice(parent) , xsensor1(sensor1) , xsensor2(sensor2) , xsensor3(sensor3) , xsensor4(sensor4) , xsensor5(sensor5) {}

  long num_single_espresso, num_double_espresso, num_coffee, num_double_coffee, num_clean;

  // Jura communication function taken in entirety from cmd2jura.ino, found at https://github.com/hn/jura-coffee-machine
  String cmd2jura(String outbytes) {
    String inbytes;
    int w = 0;

    while (available()) {
      read();
    }

    outbytes += "\r\n";
    for (int i = 0; i < outbytes.length(); i++) {
      for (int s = 0; s < 8; s += 2) {
        char rawbyte = 255;
        bitWrite(rawbyte, 2, bitRead(outbytes.charAt(i), s + 0));
        bitWrite(rawbyte, 5, bitRead(outbytes.charAt(i), s + 1));
        write(rawbyte);
      }
      delay(8);
    }

    int s = 0;
    char inbyte;
    while (!inbytes.endsWith("\r\n")) {
      if (available()) {
        byte rawbyte = read();
        bitWrite(inbyte, s + 0, bitRead(rawbyte, 2));
        bitWrite(inbyte, s + 1, bitRead(rawbyte, 5));
        if ((s += 2) >= 8) {
          s = 0;
          inbytes += inbyte;
        }
      } else {
        delay(10);
      }
      if (w++ > 500) {
        return "";
      }
    }

    return inbytes.substring(0, inbytes.length() - 2);
  }

  void setup() override {
    this->set_update_interval(600000); // 600 seconds (10 minutes)
  }

  void loop() override {
  }

  void update() override {
    String result;

    // Get Single Espressos made
    result = cmd2jura("RE:0000");
    result.remove(0,3); // Strip first 3 characters of response, which in this case are "re:" (repeated back to us)
    num_single_espresso = strtol(result.c_str(),NULL,16);

    // Get Double Espressos made
    result = cmd2jura("RE:0001");
    result.remove(0,3);
    num_double_espresso = strtol(result.c_str(),NULL,16);

    // Get Coffees made
    result = cmd2jura("RE:0002");
    result.remove(0,3);
    num_coffee = strtol(result.c_str(),NULL,16);

    // Get Double Coffees made
    result = cmd2jura("RE:0003");
    result.remove(0,3);
    num_double_coffee = strtol(result.c_str(),NULL,16);

    // Get Cleanings done
    result = cmd2jura("RE:0008");
    result.remove(0,3);
    num_clean = strtol(result.c_str(),NULL,16);

    if (xsensor1 != nullptr)   xsensor1->publish_state(num_single_espresso);
    if (xsensor2 != nullptr)   xsensor2->publish_state(num_double_espresso);
    if (xsensor3 != nullptr)   xsensor3->publish_state(num_coffee);
    if (xsensor4 != nullptr)   xsensor4->publish_state(num_double_coffee);
    if (xsensor5 != nullptr)   xsensor5->publish_state(num_clean);

  }
};

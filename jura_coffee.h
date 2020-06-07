#include "esphome.h"

class JuraCoffee : public PollingComponent, public UARTDevice {
 Sensor *xsensor1 {nullptr};
 Sensor *xsensor2 {nullptr};
 Sensor *xsensor3 {nullptr};
 Sensor *xsensor4 {nullptr};
 Sensor *xsensor5 {nullptr};
 TextSensor *xsensor6 {nullptr};
 TextSensor *xsensor7 {nullptr};

 public:
  JuraCoffee(UARTComponent *parent, Sensor *sensor1, Sensor *sensor2, Sensor *sensor3, Sensor *sensor4, Sensor *sensor5, TextSensor *sensor6, TextSensor *sensor7) : UARTDevice(parent) , xsensor1(sensor1) , xsensor2(sensor2) , xsensor3(sensor3) , xsensor4(sensor4) , xsensor5(sensor5) , xsensor6(sensor6) , xsensor7(sensor7) {}

  long num_single_espresso, num_double_espresso, num_coffee, num_double_coffee, num_clean;
  std::string tray_status, tank_status;

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
    this->set_update_interval(60000); // 600000 = 10 minutes // Now 60 seconds
  }

  void loop() override {
  }

  void update() override {
    String result, hexString, substring;
    byte hex_to_byte;
    int trayBit, tankBit;
    // For Testing
    // int read_bit0,read_bit1,read_bit2,read_bit3,read_bit4,read_bit5,read_bit6,read_bit7;

    // Fetch our line of EEPROM
    result = cmd2jura("RT:0000");

    // Get Single Espressos made
    substring = result.substring(3,7);
    num_single_espresso = strtol(substring.c_str(),NULL,16);

    // Double Espressos made
    substring = result.substring(7,11);
    num_double_espresso = strtol(substring.c_str(),NULL,16);

    // Coffees made
    substring = result.substring(11,15);
    num_coffee = strtol(substring.c_str(),NULL,16);

    // Double Coffees made
    substring = result.substring(15,19);
    num_double_coffee = strtol(substring.c_str(),NULL,16);

    // Cleanings done
    substring = result.substring(35,39);
    num_clean = strtol(substring.c_str(),NULL,16);

    // Tray & water tank status
    // Much gratitude to https://www.instructables.com/id/IoT-Enabled-Coffee-Machine/ for figuring out how these bits are stored
    result = cmd2jura("IC:");
    hexString = result.substring(3,5);
    hex_to_byte = strtol(hexString.c_str(),NULL,16);
    trayBit = bitRead(hex_to_byte, 4);
    tankBit = bitRead(hex_to_byte, 5);
    if (trayBit == 1) { tray_status = "Missing"; } else { tray_status = "Present"; }
    if (tankBit == 1) { tank_status = "Fill Tank"; } else { tank_status = "OK"; }

    // For Testing
    // read_bit0 = bitRead(hex_to_byte, 0);
    // read_bit1 = bitRead(hex_to_byte, 1);
    // read_bit2 = bitRead(hex_to_byte, 2);
    // read_bit3 = bitRead(hex_to_byte, 3);
    // read_bit4 = bitRead(hex_to_byte, 4);
    // read_bit5 = bitRead(hex_to_byte, 5);
    // read_bit6 = bitRead(hex_to_byte, 6);
    // read_bit7 = bitRead(hex_to_byte, 7);
    // ESP_LOGD("main", "Raw IC result: %s", result.c_str());
    // ESP_LOGD("main", "Substringed: %s", hexString.c_str());
    // ESP_LOGD("main", "Converted_To_Long: %li", hex_to_byte);
    // ESP_LOGD("main", "As Bits: %d%d%d%d%d%d%d%d", read_bit7,read_bit6,read_bit5,read_bit4,read_bit3,read_bit2,read_bit1,read_bit0);

    if (xsensor1 != nullptr)   xsensor1->publish_state(num_single_espresso);
    if (xsensor2 != nullptr)   xsensor2->publish_state(num_double_espresso);
    if (xsensor3 != nullptr)   xsensor3->publish_state(num_coffee);
    if (xsensor4 != nullptr)   xsensor4->publish_state(num_double_coffee);
    if (xsensor5 != nullptr)   xsensor5->publish_state(num_clean);
    if (xsensor6 != nullptr)   xsensor6->publish_state(tray_status);
    if (xsensor7 != nullptr)   xsensor7->publish_state(tank_status);

  }
};

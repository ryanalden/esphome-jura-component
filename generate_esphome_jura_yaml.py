#!/usr/bin/python3
#
# Usage: ./generate_esphome_jura_yaml.py [String to encode]
#
# Prints Jura-encoded ESPhome-formatted YAML for use in a Template Switch
# May 2020, ryanalden
#
# Original project: https://github.com/Q42/coffeehack
# Q42
# jura.py created by taco ekkel
# protocol source: http://protocol-jura.do.am/index/protocol_to_coffeemaker/0-7

import sys
from bitarray import bitarray

arg1 = sys.argv[1]

def tojura(c):

    cc = bytes(c, 'utf-8')

    z0 = bitarray(8)
    z0.setall(True)
    z1 = bitarray(8)
    z1.setall(True)
    z2 = bitarray(8)
    z2.setall(True)
    z3 = bitarray(8)
    z3.setall(True)
    out = bitarray()
    out.frombytes(cc)

    z0[2] = out[0]
    z0[5] = out[1]
    z1[2] = out[2]
    z1[5] = out[3]
    z2[2] = out[4]
    z2[5] = out[5]
    z3[2] = out[6]
    z3[5] = out[7]

    one = int.from_bytes(z0, byteorder='big')
    two = int.from_bytes(z1, byteorder='big')
    three = int.from_bytes(z2, byteorder='big')
    four = int.from_bytes(z3, byteorder='big')

    print("      - uart.write: [", end = '')
    print("0x{:X}".format(four), end = '')
    print(", ", end = '')
    print("0x{:X}".format(three), end = '')
    print(", ", end = '')
    print("0x{:X}".format(two), end = '')
    print(", ", end = '')
    print("0x{:X}".format(one), end = '')
    print("]  ## " , end = '')
    print((repr(c)))
    if ("\\n" not in repr(c)):
      print("      - delay: 8ms")

    return [z0,z1,z2,z3]

def encoder():
    string = arg1 + "\r\n"
    bytes = []
    for c in string:
        bytes.append(tojura(c))

print()
encoder()
print()

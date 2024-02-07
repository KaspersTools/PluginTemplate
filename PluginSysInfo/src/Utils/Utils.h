//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#ifdef __APPLE__

#include <Security/Security.h>

#endif


namespace Plugin::SysInfo {
  namespace Utils {
    /**
    * Takes a string and returns the given string as a FourCharCode.
    * @param givenString   The given string
    * @return The four char code
    */
    static FourCharCode stringToFourCharCode(const std::string &givenString) {
      const char *bytes = givenString.c_str();
      UInt32 byte0 = (unsigned) bytes[0] << (unsigned) 24;
      UInt32 byte1 = (unsigned) bytes[1] << (unsigned) 16;
      UInt32 byte2 = (unsigned) bytes[2] << (unsigned) 8;
      UInt32 byte3 = (unsigned) bytes[3];

      return byte0 | byte1 | byte2 | byte3;
    }

    /**
    * Converts a given value from the fpe2 data type to int.
    * @param value     The given value as fpe2 type.
    * @return  The corresponding integer value.
    */
    static int fpe2ToInt(UInt8 value[2]) {
      return (value[0] << (unsigned) 6) + (value[1] >> (unsigned) 2);
    }

    /**
     * Converts a given value from the flt data type to int.
     * @param value     The given value as flt type.
     * @return  The corresponding integer value.
     */
    static int fltToInt(UInt8 value[4]) {
      float resultValue = 0;
      std::memcpy(&resultValue, value, 4);
      return (int) resultValue;
    }
  }// namespace Utils
}// namespace Plugin::SysInfo

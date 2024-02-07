//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once
#include <IOKit/IOKitLib.h>

#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>

#include <sys/sysctl.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "Utils/Utils.h"

namespace Plugin::SysInfo {
  /**
  * These enum and structs are defined in the Apple PowerManagement project:
  * https://opensource.apple.com/source/PowerManagement/PowerManagement-211/pmconfigd/PrivateLib.c.auto.html
  */
  enum SMCResult : UInt8 {
    kSMCSuccess = 0,
    kSMCError = 1,
    kSMCKeyNotFound = 132
  };

  enum SMCSelector : UInt8 {
    kSMCHandleYPCEvent = 2,
    kSMCReadKey = 5,
    kSMCWriteKey = 6,
    kSMCGetKeyFromIndex = 8,
    kSMCGetKeyInfo = 9
  };

  typedef UInt8 SMCBytes[32];

  typedef struct DataType {
    DataType(const std::string &givenString, UInt32 givenSize) {
      type = Utils::stringToFourCharCode(givenString);
      size = givenSize;
    }

    DataType(FourCharCode givenType, UInt32 givenSize) : type(givenType), size(givenSize) {
    }

    FourCharCode type;
    UInt32 size;
  } data_type_t;

  typedef struct DataTypes {
    DataType FDS = DataType("{fds", 16);
    DataType Flag = DataType("flag", 1);
    DataType FPE2 = DataType("fpe2", 2);
    DataType FLT = DataType("flt", 4);
    DataType SP78 = DataType("sp78", 2);
    DataType UInt8 = DataType("ui8", 1);
    DataType UInt32 = DataType("ui32", 4);
  } data_types_t;

  typedef struct SMCKey {
    SMCKey(const std::string &givenString, data_type_t &typeInfo) : info(typeInfo) {
      code = Utils::stringToFourCharCode(givenString);
    }

    FourCharCode code;
    DataType info;
  } smc_key_t;

  typedef struct SMCVersion {
    unsigned char major = 0;
    unsigned char minor = 0;
    unsigned char build = 0;
    unsigned char reserved = 0;
    unsigned short release = 0;
  } smc_version_t;

  typedef struct SMCLimitData {
    UInt16 version = 0;
    UInt16 length = 0;
    UInt32 cpuPLimit = 0;
    UInt32 gpuPLimit = 0;
    UInt32 memPLimit = 0;
  } smc_limit_data_t;

  typedef struct SMCKeyInfoData {
    IOByteCount dataSize = 0;
    UInt32 dataType = 0;
    UInt8 dataAttributes = 0;
  } smc_key_info_data_t;

  struct SMCParamStruct {

    UInt32 key = 0;

    SMCVersion vers = SMCVersion();

    SMCLimitData pLimitData = SMCLimitData();

    SMCKeyInfoData keyInfo = SMCKeyInfoData();

    UInt8 result = 0;

    UInt8 status = 0;

    UInt8 data8 = 0;

    UInt32 data32 = 0;

    SMCBytes bytes = {0};
  };

  typedef struct CpuTickStruct {
    int userTicks;
    int systemTicks;
    int idleTicks;
    int niceTicks;
  } cpu_tick_t;

  //Own structs
  //Network
  typedef struct NetworkData  {
    std::string name = "UNKOWN";             //1
    std::string ipv4 = "UNKOWN";             //2
    std::string netmaskipv4 = "UNKOWN";      //3
    std::string broadcastipv4 = "UNKOWN";    //4
    std::string ipv6 = "UNKOWN";             //5
    std::string netmaskipv6 = "UNKOWN";      //6
    std::string broadcastipv6 = "UNKOWN";    //7
    std::string mac = "UNKOWN";              //8
    std::string gateway = "UNKOWN";          //9
    std::string status = "UNKOWN";           //10
    std::string interfaceType = "UNKOWN";    //11
    int mtu = -1;                            //12
    int interfaceIndex = -1;                 //13
    unsigned long speed = -1;                //speed in Mbit/s 14
    std::vector<std::string> dnsServers = {};//15
    bool usingDhcp = false;                  //16
  } network_data_t;

  //memory
  typedef struct MemoryInfo{
    float totalMemory = 0;
    std::vector<float> memoryUsage = {};
  } memory_info_t;

  //cpu
  typedef struct CpuInfo{
    int tempature = 0;
    std::vector<float> usage = {};
    cpu_tick_t loadInfo = cpu_tick_t();
    cpu_tick_t prevCpuTicks= cpu_tick_t();
  } cpu_info_t;


  class SysInfo {
public:
    const inline static MemoryInfo& getMemoryInfo(){
      return s_memInfo;
    }

    const inline static CpuInfo& getCpuInfo(){
      return s_cpuInfo;
    }

private:
    inline static io_connect_t s_connectionHandle = 0;

    inline static MemoryInfo s_memInfo = MemoryInfo();
    inline static CpuInfo s_cpuInfo    = CpuInfo();

    inline static bool s_isInitialized = false;
public:
    DataTypes types = DataTypes();

    inline static void init() {
      if(s_isInitialized)
        return;

      s_isInitialized = true;

      fetchMemoryInfo();
      s_memInfo.totalMemory = fetchTotalMemory();

      fetchCpuInfo();
    }

    /**
    * Opens a connection to the SMC driver.
    * @throws runtime_error    Throws a runtime error if the connection can not be made.
    */
    static void open();

    /**
    * Closes the connection to the SMC driver.
    * @throws runtime_error    Throws a runtime error if closing the connection failed.
    */
    static void close();

    /**
    * Returns the data type of the given key.
    * @param keyString The given key as a string.
    * @return  The data type information for the given key.
    */
    static DataType getKeyInfo(std::string keyString);

    /**
    * Makes a call to the SMC.
    * @throws runtime_error    Throws a runtime error if something went wrong reading the key.
    * @param givenStruct       The given struct which is used to call the SMC.
    * @param smcSelector       The given smc selector.
    * @return  Returns the resulting struct of the SMC call
    */
    static SMCParamStruct callSMC(SMCParamStruct givenStruct, SMCSelector smcSelector = kSMCHandleYPCEvent);

    /**
    * Reads the data of a SMC-Key
    * @throws runtime_error    Throws a runtime error if something went wrong while reading the key.
    * @param smcKey            The given struct containing the key.
    * @param result            Reference in which the result will be stored.
    */
    static void readKey(smc_key_t smcKey, SMCBytes &result);

    /**
    * Reads the data of the key code from the SMC.
    * @param keyCode   The given four-figure key code.
    * @param typeInfo  The type info for reading the key.
    * @param resultArray   The array in which the read data is written.
    */
    static void readKey(const std::string &keyCode, DataType typeInfo, SMCBytes &resultArray);


    /***********************MEMORY************************/
public:
    inline static void fetchMemoryInfo(){
      s_memInfo.memoryUsage = fetchMemoryUsage();
    }

private:
    /**
    * Returns the total amount of ram memory of this machine in gigabyte.
    */
    static float fetchTotalMemory();
    /**
    * Reads the current ram usage and returns it in a vector.
    * The vector contains 5 values: [free, active, inactive, wired, compressed]
    * The values are in GB
    */
    static std::vector<float> fetchMemoryUsage();


    /***********************CPU************************/
public:
    inline static void fetchCpuInfo(){
      s_cpuInfo.tempature = fetchCpuTemp();
      s_cpuInfo.usage = fetchCpuUsage();
      s_cpuInfo.loadInfo = fetchCpuLoadInfo();
    }
private:
    /**
    * Reads the cpu temperature of the CPU_0_PROXIMITY sensor.
    * @return  The temperature of the cpu in degrees celcius.
    */
    static int fetchCpuTemp();

    /**
    * Returns the cpu usage of the user, system, idle and nice in the given float array in this order.
    */
    static std::vector<float> fetchCpuUsage();

    /**
    * Returns the cpu ticks of the user, system idle and nice in a struct.
    */
    static cpu_tick_t fetchCpuLoadInfo();
  };
}// namespace Plugin::SysInfo
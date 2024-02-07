//
// Created by Kasper de Bruin on 07/02/2024.
//
#include "SystemInfo.h"

//C++
#include <map>
#include <sstream>

//Core Foundation
#include <CoreFoundation/CoreFoundation.h>

//System Configuration
#include <SystemConfiguration/SystemConfiguration.h>

//IOKit
#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>

//disk
#include <sys/mount.h>

//network
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>

namespace Plugin::SysInfo {
  void SysInfo::open() {
    // get the smc service
    io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("AppleSMC"));
    if (service == 0) {
      throw std::runtime_error("Unable to find the SMC service.");
    }

    kern_return_t openingResult = IOServiceOpen(service, mach_task_self_, 0, &s_connectionHandle);
    if (openingResult != kIOReturnSuccess) {
      throw std::runtime_error("Unable to connect to the SMC.");
    }

    IOObjectRelease(service);
  }

  void SysInfo::close() {
    kern_return_t closeResult = IOServiceClose(s_connectionHandle);
    if (closeResult != kIOReturnSuccess) {
      throw std::runtime_error("Failed to close the connection to the SMC!");
    }

    s_connectionHandle = 0;
  }

  DataType SysInfo::getKeyInfo(std::string keyString) {
    // create a new smc struct and set the key and data fields
    SMCParamStruct inputStruct = SMCParamStruct();
    inputStruct.key = Utils::stringToFourCharCode(keyString);
    inputStruct.data8 = kSMCGetKeyInfo;

    SMCParamStruct readResult = callSMC(inputStruct);

    return {readResult.keyInfo.dataType, static_cast<UInt32>(readResult.keyInfo.dataSize)};
  }

  SMCParamStruct SysInfo::callSMC(SMCParamStruct givenStruct,
                                  SMCSelector smcSelector) {
    // create an output struct to save the result
    SMCParamStruct outputStruct = SMCParamStruct();
    size_t outputStructSize = sizeof(outputStruct);
    size_t inputStructSize = sizeof(givenStruct);

    // make the call to the SMC
    kern_return_t smcCallResult = IOConnectCallStructMethod(s_connectionHandle,
                                                            smcSelector,
                                                            &givenStruct,
                                                            inputStructSize,
                                                            &outputStruct,
                                                            &outputStructSize);

    if (smcCallResult == kIOReturnSuccess && outputStruct.result == kSMCSuccess) {
      return outputStruct;
    } else if (smcCallResult == kIOReturnSuccess && outputStruct.result == kSMCKeyNotFound) {
      throw std::runtime_error("Given SMC-Key was not found!");
    } else if (smcCallResult == kIOReturnNotPrivileged) {
      throw std::runtime_error("Reading this key requires root privileges!");
    } else {
      throw std::runtime_error("An unknown error occurred while reading the SMC-Key!");
    }
  }

  void SysInfo::readKey(smc_key_t smcKey, SMCBytes &result) {
    SMCParamStruct inputStruct = SMCParamStruct();

    inputStruct.key = smcKey.code;
    inputStruct.keyInfo.dataSize = (UInt32) smcKey.info.size;
    inputStruct.data8 = kSMCReadKey;

    SMCParamStruct resultStruct = callSMC(inputStruct);
    std::copy(std::begin(resultStruct.bytes), std::end(resultStruct.bytes), std::begin(result));
  }

  void SysInfo::readKey(const std::string &keyCode, DataType typeInfo, SMCBytes &resultArray) {
    SMCKey key = SMCKey(keyCode, typeInfo);
    readKey(key, resultArray);
  }


  /*******Memory*******/
  std::vector<float> SysInfo::fetchMemoryUsage() {
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    mach_port_t host = mach_host_self();
    vm_statistics64_data_t stats;

    kern_return_t result = host_statistics64(host, HOST_VM_INFO64, (host_info64_t) &stats, &count);

    if (result != KERN_SUCCESS) {
      throw std::runtime_error("An error occured while getting the memory usage.");
    }

    vm_size_t page_size = vm_kernel_page_size;
    // divide by 1_073_741_824 to get gigabyte
    float freeMem = (double) (stats.free_count * page_size) / (double) 1073741824;
    float activeMem = (double) (stats.active_count * page_size) / (double) 1073741824;
    float inactiveMem = (double) (stats.inactive_count * page_size) / (double) 1073741824;
    float wiredMem = (double) (stats.wire_count * page_size) / (double) 1073741824;
    float compressedMem = (double) (stats.compressor_page_count * page_size) / (double) 1073741824;

    std::vector<float> resultVec;
    resultVec.push_back(freeMem);
    resultVec.push_back(activeMem);
    resultVec.push_back(inactiveMem);
    resultVec.push_back(wiredMem);
    resultVec.push_back(compressedMem);

    return resultVec;
  }

  float SysInfo::fetchTotalMemory() {
    mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;
    mach_port_t host = mach_host_self();
    host_basic_info_data_t info;

    kern_return_t result = host_info(host, HOST_BASIC_INFO, (host_info_t) &info, &count);

    if (result != KERN_SUCCESS) {
      throw std::runtime_error("An error occured while getting the total memory of the machine.");
    }

    float totalMemory = (double) info.max_mem / (double) 1073741824;

    return totalMemory;
  }

  /*******CPU*******/
  int  SysInfo::fetchCpuTemp(){
    SMCBytes resultArray;
    readKey("TC0P", getKeyInfo("TC0P"), resultArray);

    return resultArray[0];
  }

  std::vector<float> SysInfo::fetchCpuUsage(){
    cpu_tick_t curCpuTicks = fetchCpuLoadInfo();

    float userDiff = curCpuTicks.userTicks   - s_cpuInfo.prevCpuTicks.userTicks;
    float sysDiff  = curCpuTicks.systemTicks - s_cpuInfo.prevCpuTicks.systemTicks;
    float idleDiff = curCpuTicks.idleTicks   - s_cpuInfo.prevCpuTicks.idleTicks;
    float niceDiff = curCpuTicks.niceTicks   - s_cpuInfo.prevCpuTicks.niceTicks;

    float curTotalTicks = userDiff + sysDiff + idleDiff + niceDiff;

    std::vector<float> resultVec;
    resultVec.push_back(userDiff / curTotalTicks * 100);
    resultVec.push_back(sysDiff / curTotalTicks * 100);
    resultVec.push_back(idleDiff / curTotalTicks * 100);
    resultVec.push_back(niceDiff / curTotalTicks * 100);

    s_cpuInfo.prevCpuTicks = curCpuTicks;

    return resultVec;
  }

  cpu_tick_t SysInfo::fetchCpuLoadInfo(){
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    mach_port_t host = mach_host_self();
    host_cpu_load_info_data_t load_info;

    kern_return_t result = host_statistics(host, HOST_CPU_LOAD_INFO, (host_info_t) &load_info, &count);

    if (result != KERN_SUCCESS) {
      throw std::runtime_error("An error occured while getting the cpu usage.");
    }

    cpu_tick_t cpuTickStruct;

    cpuTickStruct.userTicks = load_info.cpu_ticks[CPU_STATE_USER];
    cpuTickStruct.systemTicks = load_info.cpu_ticks[CPU_STATE_SYSTEM];
    cpuTickStruct.idleTicks = load_info.cpu_ticks[CPU_STATE_IDLE];
    cpuTickStruct.niceTicks = load_info.cpu_ticks[CPU_STATE_NICE];

    return cpuTickStruct;
  }
}// namespace Plugin::SysInfo
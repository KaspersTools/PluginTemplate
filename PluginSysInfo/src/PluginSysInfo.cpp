//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginSysInfo.h"
#include "SystemInfo.h"
#include "UI/PluginSysInfoWindow.h"

namespace Plugin::SysInfo {
  void EntryPoint::initialize() {
    std::cout << "Plugin SysInfo Initialized" << std::endl;
    HummingBirdCore::UI::WindowManager::getInstance()->addWindow("SysInfo", 0, std::make_shared<Window>());
    SysInfo::open();
    SysInfo::init();
  }

  void EntryPoint::update() {
    s_currentTime = ImGui::GetTime();
    if (s_currentTime - s_lastTime >= 1.0f) {
      s_lastTime = s_currentTime;
      SysInfo::fetchMemoryInfo();
      SysInfo::fetchCpuInfo();
    }
  }

  void EntryPoint::cleanup() {
    std::cout << "SysInfo plugin cleaned up" << std::endl;
    SysInfo::close();
  }
}// namespace Plugin::SysInfo
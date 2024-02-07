//
// Created by Kasper de Bruin on 07/02/2024.
//

#include "PluginSysInfoWindow.h"
#include "../SystemInfo.h"

namespace Plugin::SysInfo {
  void Window::render() {
    showMemoryInfo();
    showCpuInfo();
  }

  void Window::showMemoryInfo(){
    const MemoryInfo& memInfo = SysInfo::getMemoryInfo();
    const float totalMem = memInfo.totalMemory;
    const std::vector<float> memUsage = memInfo.memoryUsage;

    std::string freeMem       = std::to_string(memUsage[0]);
    std::string activeMem     = std::to_string(memUsage[1]);
    std::string inactiveMem   = std::to_string(memUsage[2]);
    std::string wiredMem      = std::to_string(memUsage[3]);
    std::string compressedMem = std::to_string(memUsage[4]);

    ImGui::SeparatorText("Memory");
    ImGui::Text("Free: %s GB", freeMem.c_str());
    ImGui::ProgressBar(memUsage[0] / totalMem, ImVec2(-1.0f, 0.0f), "Free");
    ImGui::Text("Active: %s GB", activeMem.c_str());
    ImGui::ProgressBar(memUsage[1] / totalMem, ImVec2(-1.0f, 0.0f), "Active");
    ImGui::Text("Inactive: %s GB", inactiveMem.c_str());
    ImGui::ProgressBar(memUsage[2] / totalMem, ImVec2(-1.0f, 0.0f), "Inactive");
    ImGui::Text("Wired: %s GB", wiredMem.c_str());
    ImGui::ProgressBar(memUsage[3] / totalMem, ImVec2(-1.0f, 0.0f), "Wired");
    ImGui::Text("Compressed: %s GB", compressedMem.c_str());
    ImGui::ProgressBar(memUsage[4] / totalMem, ImVec2(-1.0f, 0.0f), "Compressed");
  }

  void Window::showCpuInfo(){
    const CpuInfo& cpuInfo = SysInfo::getCpuInfo();
    const int m_cpuTemp = cpuInfo.tempature;
    const std::vector<float> m_cpuUsage = cpuInfo.usage;

    ImGui::SeparatorText("CPU");
    ImGui::Text("CPU Temperature: %d°C", m_cpuTemp);

    ImGui::Text("CPU Usage");
    ImGui::Text("User: %.2f%%", m_cpuUsage[0]);
    ImGui::ProgressBar(m_cpuUsage[0] / 100, ImVec2(-1.0f, 0.0f), "User");
    ImGui::Text("System: %.2f%%", m_cpuUsage[1]);
    ImGui::ProgressBar(m_cpuUsage[1] / 100, ImVec2(-1.0f, 0.0f), "System");
    ImGui::Text("Idle: %.2f%%", m_cpuUsage[2]);
    ImGui::ProgressBar(m_cpuUsage[2] / 100, ImVec2(-1.0f, 0.0f), "Idle");
    ImGui::Text("Nice: %.2f%%", m_cpuUsage[3]);
    ImGui::ProgressBar(m_cpuUsage[3] / 100, ImVec2(-1.0f, 0.0f), "Nice");
  }
}// namespace Plugin::SysInfo
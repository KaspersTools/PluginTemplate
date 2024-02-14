//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once

#include "HBUI/HBUI.h"
#include "HBUI/UIWindow.h"
#include "HBUI/WindowManager.h"

class PluginTemplateWindow : public HummingBirdCore::UIWindow {
  public:
  PluginTemplateWindow() : UIWindow("Plugin Example") {
  }

  void render() override {
    ImGui::Text("This is the plugin example window");

    ImGui::InputText("Plugin Name", inputTest, IM_ARRAYSIZE(inputTest));
    ImGui::ColorPicker4("Color Picker", &clear_color.x);

    ImGui::Button("test button1");
    ImGui::Button("test button3");
  }

  private:
  char inputTest[128] = "Hello, world!";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

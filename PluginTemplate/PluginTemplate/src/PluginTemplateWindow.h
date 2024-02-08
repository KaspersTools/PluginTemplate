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
    ImGui::Text("This is the plugin exsmple window");
	//todo: add stdlib
//    ImGui::InputText("Plugin Name", &inputTest)
	ImGui::InputText("Plugin Name", inputTest, IM_ARRAYSIZE(inputTest));
    ImGui::Button("test button");
  }

  private:
	char inputTest[128] = "Hello, world!";
};


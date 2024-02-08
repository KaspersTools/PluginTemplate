//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginTemplate.h"
#include "PluginTemplateWindow.h"

void PluginTemplate::initialize() {
	std::cout << "PluginTemplate initialized" << std::endl;
	HummingBirdCore::UI::WindowManager::getInstance()->addWindow("Plugin Template", 0, std::make_shared<PluginTemplateWindow>());
}

void PluginTemplate::cleanup() {
	std::cout << "v cleaned up" << std::endl;
}

void PluginTemplate::update() {
}
//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once

#include "../include/IPlugin.h"
#include <HBUI/HBUI.h>

class PluginTemplate : public HummingBird::Plugins::IPlugin {
public:
public:
	PluginTemplate(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
			ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) : IPlugin(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData) {
	}
	void initialize() override;
	void update() override;
	void cleanup() override;

private:
	char* inputTest = "test";

};

extern "C" HummingBird::Plugins::IPlugin *create_plugin(
		HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
		ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) {
	return new PluginTemplate(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData);
}
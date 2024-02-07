//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once

#include "../include/IPlugin.h"
#include <HBUI/HBUI.h>

namespace Plugin::SysInfo {
	class EntryPoint : public HummingBird::Plugins::IPlugin {
	public:
	public:
		EntryPoint(HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
				   ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) : IPlugin(
				windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData) {
		}

		void initialize() override;

		void update() override;

		void cleanup() override;

	};
}


extern "C" HummingBird::Plugins::IPlugin *create_plugin(
		HummingBirdCore::UI::WindowManager *windowManagerPtr, ImGuiContext *imGuiContext,
		ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void *userData) {
	return new Plugin::SysInfo::EntryPoint(windowManagerPtr, imGuiContext, allocFunc, freeFunc, userData);
}
//
// Created by Kasper de Bruin on 06/02/2024.
//

#include "PluginSysInfo.h"
#include "UI/PluginSysInfoWindow.h"

namespace Plugin::SysInfo {
	void EntryPoint::initialize() {
		std::cout << "Plugin SysInfo Initialized" << std::endl;
		HummingBirdCore::UI::WindowManager::getInstance()->addWindow("SysInfo", 0,
																	 std::make_shared<Window>());
	}

	void EntryPoint::cleanup() {
		std::cout << "SysInfo plugin cleaned up" << std::endl;
	}

	void EntryPoint::update() {
	}
}
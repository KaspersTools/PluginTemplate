//
// Created by Kasper de Bruin on 07/02/2024.
//

#include "PluginSysInfoWindow.h"

namespace Plugin::SysInfo {
	void Window::render() {
		ImGui::Text("This is the plugin templatse window");
		ImGui::Button("test button");
	}
}
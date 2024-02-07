//
// Created by Kasper de Bruin on 07/02/2024.
//

#pragma once

#include "HBUI/HBUI.h"
#include "HBUI/UIWindow.h"
#include "HBUI/WindowManager.h"

namespace Plugin::SysInfo {
	class Window : public HummingBirdCore::UIWindow {
	public:
		Window() : UIWindow("Plugin SysInfo"){}
		void render() override;
	private:
	};
}


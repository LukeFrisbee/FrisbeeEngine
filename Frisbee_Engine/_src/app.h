#pragma once

#include <vector>

#include "device.h"
#include "window.h"
#include "renderer.h"
#include "game_object.h"
#include "descriptors.h"
#include "render_system.h"

namespace fengine { 
	class App {

		public:
		static constexpr int WIDTH = 1600;
		static constexpr int HEIGHT = 1200;

		App();
		~App();
		void run();

		App(const App &) = delete;
		App& operator=(const App&) = delete;

		private:
		void loadGameObjects();

		FWindow m_window {WIDTH, HEIGHT, "Frisbee Engine"};
		Device m_device{ m_window };
		Renderer m_renderer{ m_window, m_device };
		RenderSystem m_renderSystem{ m_device, m_renderer.getRenderPass() };

		std::vector<GameObject> m_gameObjects;
	};
}
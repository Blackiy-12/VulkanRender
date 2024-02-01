#pragma once

#include "Descriptors.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Window.hpp"


#include <memory>
#include <vector>


class FirstApp 
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	FirstApp();
	~FirstApp();

	FirstApp(const FirstApp &) = delete;
	FirstApp &operator=(const FirstApp &) = delete;

	void run();

private:
	void loadGameObjects();

	void loadShaders();

	Window lveWindow{WIDTH, HEIGHT, "Vulkan"};
	Device device{lveWindow};
	Renderer lveRenderer{lveWindow, device};

	std::unique_ptr<DescriptorPool> globalPool{};
	GameObject::Map gameObjects;
};


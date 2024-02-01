#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include <string>


class Window {
public:
	Window(int w, int h, std::string name);
	~Window();

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	
	VkExtent2D getExtent()			const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
	SDL_Window *getSDLwindow()		const { return window; }

private:

	int width;
	int height;
	bool framebufferResized = false;

	std::string windowName;
	SDL_Window* window;
};


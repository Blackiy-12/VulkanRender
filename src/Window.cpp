#include "Window.hpp"

#include <SDL2/SDL_vulkan.h>
#include <stdexcept>

Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} 
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
}

Window::~Window() 
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) 
{
    if (SDL_Vulkan_CreateSurface(window, instance, surface) == SDL_FALSE) 
    {
      throw std::runtime_error("failed to craete window surface");
    }
}



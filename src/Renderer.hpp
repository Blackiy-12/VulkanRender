#pragma once

#include "Device.hpp"
#include "Swapchain.hpp"
#include "Window.hpp"


#include <cassert>
#include <memory>
#include <vector>


class Renderer 
{
public:
    Renderer(Window &window, Device &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass getSwapChainRenderPass()   const { return lveSwapChain->getRenderPass(); }
    float getAspectRatio()                  const { return lveSwapChain->extentAspectRatio(); }
    bool isFrameInProgress()                const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const 
    {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const 
    {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void createSwapChain();

    Window &lveWindow;
    Device &device;
    std::unique_ptr<SwapChain> lveSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex{0};
    bool isFrameStarted{false};
};


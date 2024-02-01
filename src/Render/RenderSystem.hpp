#pragma once

#include <Camera.hpp>
#include <Device.hpp>
#include <FrameInfo.hpp>
#include <GameObject.hpp>
#include <Pipeline.hpp>


#include <memory>
#include <vector>


class SimpleRenderSystem 
{
public:
    SimpleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo);

 private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Device &device;

    std::unique_ptr<Pipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
};


#pragma once

#include <Camera.hpp>
#include <Device.hpp>
#include <FrameInfo.hpp>
#include <GameObject.hpp>
#include <Pipeline.hpp>


#include <memory>
#include <vector>


class PointLightSystem 
{
public:
    PointLightSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~PointLightSystem();

    PointLightSystem(const PointLightSystem &) = delete;
    PointLightSystem &operator=(const PointLightSystem &) = delete;

    void update(FrameInfo &frameInfo, GlobalUbo &ubo);
    void render(FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Device &device;

    std::unique_ptr<Pipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
};


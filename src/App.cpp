#include "App.hpp"

#include "MovementController.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Render/LightSystem.hpp"
#include "Render/RenderSystem.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <Shader/ShaderBuilder.hpp>


FirstApp::FirstApp() 
{
    globalPool = DescriptorPool::Builder(device).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT).build();
    loadShaders();
    loadGameObjects();
}

FirstApp::~FirstApp() 
{
}

void FirstApp::run() 
{
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) 
    {
        uboBuffers[i] = std::make_unique<Buffer>(
            device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout =DescriptorSetLayout::Builder(device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();

    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) 
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem
    {
        device,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()
    };
    
    PointLightSystem pointLightSystem
    {
        device,
        lveRenderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout()
    };
    Camera camera{};

    auto viewerObject = GameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
    InputHandler* Input = InputHandler::getInput();

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!Input->wasQuitEvetn()) 
    {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        Input->processInput(frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = lveRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = lveRenderer.beginFrame()) 
        {
            int frameIndex = lveRenderer.getFrameIndex();
            FrameInfo frameInfo
            {
                frameIndex,
                frameTime,
                commandBuffer,
                camera,
                globalDescriptorSets[frameIndex],
                gameObjects
            };

            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            lveRenderer.beginSwapChainRenderPass(commandBuffer);

            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);

            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(device.device());
}

void FirstApp::loadGameObjects() 
{
    std::shared_ptr<Model> lveModel = Model::createModelFromFile(device, "models/flat_vase.obj");
    auto flatVase = GameObject::createGameObject();
    flatVase.model = lveModel;
    flatVase.transform.translation = {-.5f, .5f, 0.f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(flatVase.getId(), std::move(flatVase));

    lveModel = Model::createModelFromFile(device, "models/smooth_vase.obj");
    auto smoothVase = GameObject::createGameObject();
    smoothVase.model = lveModel;
    smoothVase.transform.translation = {.5f, .5f, 0.f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

    lveModel = Model::createModelFromFile(device, "models/Monkey.obj");
    auto monkey = GameObject::createGameObject();
    monkey.model = lveModel;
    monkey.transform.translation = { .0f, -.5f, -1.f };
    monkey.transform.scale = { .5f, -.5f, -.5f };
    gameObjects.emplace(monkey.getId(), std::move(monkey));

    lveModel = Model::createModelFromFile(device, "models/quad.obj");
    auto floor = GameObject::createGameObject();
    floor.model = lveModel;
    floor.transform.translation = {0.f, .5f, 0.f};
    floor.transform.scale = {3.f, 1.f, 3.f};
    gameObjects.emplace(floor.getId(), std::move(floor));

    std::vector<glm::vec3> lightColors
    {
        {1.f, .1f, .1f},
        {.1f, .1f, 1.f},
        {.1f, 1.f, .1f},
        {1.f, 1.f, .1f},
        {.1f, 1.f, 1.f},
        {1.f, 1.f, 1.f} 
    };

    for (int i = 0; i < lightColors.size(); i++) 
    {
        auto pointLight = GameObject::makePointLight(0.2f);
        pointLight.color = lightColors[i];
        auto rotateLight = glm::rotate( glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), {0.f, -1.f, 0.f});
        pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}

void FirstApp::loadShaders()
{
    ShaderBuilder::compile("point_light.vert", shaderc_shader_kind::shaderc_vertex_shader, "./shaders/point_light.vert", true);
    ShaderBuilder::compile("point_light.frag", shaderc_shader_kind::shaderc_fragment_shader, "./shaders/point_light.frag", true);
    ShaderBuilder::compile("simple_shader.vert", shaderc_shader_kind::shaderc_vertex_shader, "./shaders/simple_shader.vert", true);
    ShaderBuilder::compile("simple_shader.frag", shaderc_shader_kind::shaderc_fragment_shader, "./shaders/simple_shader.frag", true);
}
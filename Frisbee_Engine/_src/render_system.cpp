#include "render_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"

#include <stdexcept>
#include <array>

#include "global_data.h"

namespace fengine {
	struct SimplePushConstantData {
		glm::mat4 transform{ 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
		//alignas(16) glm::vec3 Color{};
	};

	RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass) 
		: m_device { device } 
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}

	RenderSystem::~RenderSystem() {
		vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
	}

	void RenderSystem::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void RenderSystem::createPipeline(VkRenderPass renderPass) {
		assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		FPipeline::defaultPipeLineConfigInfo(
			pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;

		m_fPipeline = std::make_unique<FPipeline>(
			m_device,
			pipelineConfig,
			"shaders/vertex.vert.spv",
			"shaders/fragment.frag.spv");

		VkPhysicalDeviceFeatures  supportedFeatures;
		vkGetPhysicalDeviceFeatures(m_device.physicalDevice(), &supportedFeatures);

		// WIREFRAME
		pipelineConfig.rasterizationInfo.polygonMode = VK_POLYGON_MODE_LINE;
		m_fPipelineWireFrame = std::make_unique<FPipeline>(
			m_device,
			pipelineConfig,
			"shaders/vertex.vert.spv",
			"shaders/fragment.frag.spv");
	}

	void RenderSystem::renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects)
	{
		// WIREFRAME
		if (GlobalData::getInstance().isWireFrame)
			m_fPipelineWireFrame->bind(frameInfo.commandBuffer);
		else
			m_fPipeline->bind(frameInfo.commandBuffer);

		auto projectionView = frameInfo.camera.getProjection() * frameInfo.camera.getView();

		for (auto& obj : gameObjects)
		{
			//auto uiInputData = m_draw->GetUIInputData();

			SimplePushConstantData push{};
			push.transform = projectionView * obj.transform.modelMatrix();
			push.normalMatrix = obj.transform.normalMatrix();

			//push.offset = uiInputData.position; //obj.transform2D.translation;
			//push.Color = obj.Color;

			vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0, sizeof(SimplePushConstantData), &push);

			obj.model->bind(frameInfo.commandBuffer);
			obj.model->draw(frameInfo.commandBuffer);
		}
	}
}
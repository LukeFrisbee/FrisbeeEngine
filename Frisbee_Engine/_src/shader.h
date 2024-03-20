#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "pipeline.h"
#include "buffer.h"
#include "swap_chain.h"

namespace fengine {
	struct SimplePushConstantData {
		glm::mat4 modelMatrix{ 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
		//alignas(16) glm::vec3 Color{};
	};

	class Shader {
	public:
		Shader(Device& device, const VkRenderPass& renderPass, const char* vertexPath, const char* fragPath);
		~Shader();

		void bindPipeline(const VkCommandBuffer& commandBuffer);
		void bindDescriptorSets(const VkCommandBuffer& commandBuffer, int frameIndex);
		void updatePushConstants(const VkCommandBuffer& commandBuffer, const SimplePushConstantData& push);

		//Shader(const Shader&) = delete;
		//Shader& operator= (const Shader&) = delete;

	private:
		void _buildDescriptorSet();
		void _createPipelineLayout();
		void _createPipeline(const VkRenderPass& renderPass);

		Device& m_device;
		VkPipelineLayout m_pipelineLayout;
		std::unique_ptr<FPipeline> m_pipeline;
		std::vector<std::unique_ptr<Buffer>> m_uboBuffers = std::vector<std::unique_ptr<Buffer>>(SwapChain::MAX_FRAMES_IN_FLIGHT);

		std::vector<VkDescriptorSet> m_descriptorSets = std::vector<VkDescriptorSet>(SwapChain::MAX_FRAMES_IN_FLIGHT);
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayout;
	};
}
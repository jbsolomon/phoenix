#include <optional>

#include <id.hpp>

#include "phx_sdl/scene.hpp"

namespace phx_sdl {

    std::tuple<vk::DescriptorSetLayout, vk::PipelineLayout>
    Scene::create_pipeline_layout(
      const vk::Device& device, const vk::DispatchLoaderDynamic& loader,
      const std::optional<vk::DescriptorSetLayout> layout) const
      noexcept {

	// Pipeline layout includes information about uniform buffers
	// and push constants. etc.
	vk::DescriptorSetLayoutBinding uboBindings(
	  0, vk::DescriptorType::eUniformBuffer, 1,
	  vk::ShaderStageFlagBits::eAll);

	// The descriptor set layout is necessary for using push
	// constants and UBOs.
	auto descriptor_set_layout =
	  layout.has_value() ? layout.value()
	                     : device.createDescriptorSetLayout(
	                         vk::DescriptorSetLayoutCreateInfo(
	                           vk::DescriptorSetLayoutCreateFlags(),
	                           1, &uboBindings));

	const auto& pcrange = vk::PushConstantRange(
	  vk::ShaderStageFlagBits::eAll, 0,
	  sizeof(Bridge::PushConstants));

	auto pl_layout = device.createPipelineLayout(
	  vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(),
	                               1, &descriptor_set_layout, 1,
	                               &pcrange),
	  nullptr, loader);

	return std::make_tuple(descriptor_set_layout, pl_layout);
    }

    void Scene::encode_graphics_cmd(
      const vk::CommandBuffer& into, const vk::PipelineLayout& layout,
      const vk::Event& evt, const Bridge::PushConstants& pcs) const
      noexcept {

	into.pushConstants(layout, vk::ShaderStageFlagBits::eAll, 0,
	                   sizeof(Bridge::PushConstants), &pcs);

	// After completing the push constant command, set the
	// event status.
	into.setEvent(evt, vk::PipelineStageFlagBits::eAllCommands);
    }

}; // namespace phx_sdl

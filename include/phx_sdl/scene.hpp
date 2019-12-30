#ifndef PHX_SDL_SCENE_HPP
#define PHX_SDL_SCENE_HPP

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <glm/vec2.hpp>

#include <id.hpp>

namespace phx_sdl {

    class Scene {

    public:
	struct Surface {
	    std::vector<glm::vec2> verts;
	};

	struct Shader {
	    res::ID frag;
	    res::ID vert;
	};

	// Functionality for bridging GLSL / C++.
	struct Bridge {
	    Bridge() = delete;

	    // The interface to user intentions.
	    struct Intent {
		// The direction the user indicates.
		glm::vec2 m;
	    };

	    struct Aspect {
		// The ratio of screen width to screen height.
		float a;
	    };

	    struct PushConstants {
		// A container for all push constants.
		Intent i;
		Aspect a;
	    };

	    static_assert(sizeof(PushConstants) <= 128,
	                  "PushConstants is too big (max 128 bytes.)");
	};

	Surface surface;

	// TODO: Refactor to allow users to subclass this.
	virtual Shader shader(void) const noexcept = 0;

	virtual std::tuple<vk::DescriptorSetLayout, vk::PipelineLayout>
	create_pipeline_layout(
	  const vk::Device&, const vk::DispatchLoaderDynamic&,
	  const std::optional<vk::DescriptorSetLayout> =
	    std::nullopt) const noexcept;

	virtual void encode_renderpass(
	  const vk::CommandBuffer&         into,
	  const vk::DispatchLoaderDynamic& loader,
	  const vk::RenderPass& pass, const vk::Framebuffer& fb,
	  const vk::Extent2D& extent, const vk::Pipeline& pipeline,
	  const uint32_t device_mask) const noexcept = 0;

	virtual void
	encode_graphics_cmd(const vk::CommandBuffer&,
	                    const vk::PipelineLayout&, const vk::Event&,
	                    const Bridge::PushConstants&) const
	  noexcept;
    };

    class Triangle : public Scene {
    public:
	Shader shader(void) const noexcept override;

	void encode_renderpass(const vk::CommandBuffer&         into,
	                       const vk::DispatchLoaderDynamic& loader,
	                       const vk::RenderPass&            pass,
	                       const vk::Framebuffer&           fb,
	                       const vk::Extent2D&              extent,
	                       const vk::Pipeline& pipeline,
	                       const uint32_t      device_mask) const
	  noexcept override;
    };

    class PushConstants : public Scene {
    public:
	Shader shader(void) const noexcept override;

	void encode_renderpass(const vk::CommandBuffer&         into,
	                       const vk::DispatchLoaderDynamic& loader,
	                       const vk::RenderPass&            pass,
	                       const vk::Framebuffer&           fb,
	                       const vk::Extent2D&              extent,
	                       const vk::Pipeline& pipeline,
	                       const uint32_t      device_mask) const
	  noexcept override;
    };

    class Texture2d : public Scene {
    public:
	Shader shader(void) const noexcept override;

	void encode_renderpass(const vk::CommandBuffer&         into,
	                       const vk::DispatchLoaderDynamic& loader,
	                       const vk::RenderPass&            pass,
	                       const vk::Framebuffer&           fb,
	                       const vk::Extent2D&              extent,
	                       const vk::Pipeline& pipeline,
	                       const uint32_t      device_mask) const
	  noexcept override;
    };

    class DefaultScenes {
    public:
	DefaultScenes() = delete;

	static const Triangle      triangle;
	static const PushConstants pushconstants;
	static const Texture2d     texture2d;
    };

} // namespace phx_sdl

#endif // PHX_SDL_SCENE_HPP
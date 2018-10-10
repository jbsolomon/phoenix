#include <map>
#include <sstream>

#include "SDL.h"
#include "SDL_syswm.h"

#include "helper.hpp"
#include "init_error.hpp"

namespace sdl {
    int Helper::getGLAttr(const SDL_GLattr what) noexcept(false) {
	int into = 0;
	if (SDL_GL_GetAttribute(what, &into)) {
	    std::stringstream ss;
	    ss << "Failed to get attribute " << what << ": "
	       << SDL_GetError();
	    throw err::InitError(ss);
	}

	return into;
    }

    void Helper::glPostHooks() noexcept(false) {
	if (int glOk = SDL_GL_LoadLibrary(nullptr) != 0) {
	    std::stringstream ss;
	    ss << "SDL_GL_LoadLibrary failed with code " << glOk << ": "
	       << SDL_GetError();
	    throw err::InitError(ss);
	}

	// Request a core OpenGL 4.1 Core context.
	setSDLGLAttr(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	setSDLGLAttr(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	setSDLGLAttr(SDL_GL_CONTEXT_PROFILE_MASK,
	             SDL_GL_CONTEXT_PROFILE_CORE);

	// Require hardware acceleration.
	setSDLGLAttr(SDL_GL_ACCELERATED_VISUAL, 1);

	auto glVMaj = getGLAttr(SDL_GL_CONTEXT_MAJOR_VERSION);
	auto glVMin = getGLAttr(SDL_GL_CONTEXT_MINOR_VERSION);

	printf("SDL OpenGL API requested: %d.%d\n", glVMaj, glVMin);
	// ? // Also request a depth buffer
	// SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    }

    void Helper::sdlOnlyPostHooks() noexcept(false) {}

    std::map<SDL_GLattr, std::string> glAttrNames = {
	{
	  SDL_GL_ACCELERATED_VISUAL,
	  "SDL_GL_ACCELERATED_VISUAL",
	},

	{ SDL_GL_CONTEXT_MAJOR_VERSION,
	  "SDL_GL_CONTEXT_MAJOR_VERSION" },

	{ SDL_GL_CONTEXT_MINOR_VERSION,
	  "SDL_GL_CONTEXT_MINOR_VERSION" },

	{ SDL_GL_CONTEXT_PROFILE_MASK, "SDL_GL_CONTEXT_PROFILE_MASK" },
    };

    void Helper::setSDLGLAttr(const SDL_GLattr a, const int to,
                              const std::string& name) noexcept(false) {
	if (int ok = SDL_GL_SetAttribute(a, to) != 0) {
	    std::stringstream ss;
	    ss << "Setting GL attribute " << name << " to " << to
	       << " failed with code " << ok << ": " << SDL_GetError();

	    throw err::InitError(ss);
	}
    }

    void Helper::setSDLGLAttr(SDL_GLattr a, int to) noexcept(false) {
	setSDLGLAttr(a, to, glAttrNames[a]);
    }

    SDL_SysWMinfo foo;
    SDL_Event mouseEvt = SDL_Event{ .motion = { .x = 100, .y = 100 } };
    void      Helper::resetMouse() { SDL_PushEvent(&mouseEvt); }
}; // namespace sdl

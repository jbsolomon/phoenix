add_subdirectory(helper)
add_subdirectory(init_util)
add_subdirectory(init_video)
add_subdirectory(display)
add_subdirectory(window)

add_subdirectory(sdl_renderer)
add_subdirectory(scene)
add_subdirectory(vk_renderer)

add_subdirectory(texture)

add_subdirectory(sdl_input)

set(SDLUtil_SRCS
  sdl_util.cxx
)

add_library(SDLUtil STATIC ${SDLUtil_SRCS})

target_include_directories(SDLUtil PUBLIC
  ${Phoenix_INCLUDE_DIR}
)

target_link_libraries(SDLUtil
  Error
  SDLInitUtil
  SDLInitVideo
  SDLDisplay
  SDLWindow
)

set_property(TARGET SDLUtil PROPERTY POSITION_INDEPENDENT_CODE ON)

set_property(TARGET SDLUtil PROPERTY CXX_STANDARD 17)
set_property(TARGET SDLUtil PROPERTY CXX_STANDARD_REQUIRED ON)

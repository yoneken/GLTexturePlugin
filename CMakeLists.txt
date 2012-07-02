
option(BUILD_GL_TEXTURE_SAMPLE "Building a GL Texture sample plugin" OFF)
set(sources GLTexturePlugin.cpp GLTextureView.cpp GLTextureView.h)

if(BUILD_GL_TEXTURE_SAMPLE)
  set(target CnoidGLTexturePlugin)
  QT4_WRAP_CPP(sources GLTextureView.h)
  add_cnoid_plugin(${target} SHARED ${sources})
  target_link_libraries(${target} CnoidBase)
  apply_common_setting_for_plugin(${target})
endif()

install_sample_source(${sources})


set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

add_library(imgui STATIC)
target_sources(imgui PRIVATE
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui.cpp

  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  )

target_link_libraries(imgui glad glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

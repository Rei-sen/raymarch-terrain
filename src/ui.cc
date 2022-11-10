/* See the file "LICENSE" for the full license governing this code. */

#include "ui.hh"

#include <algorithm>
#include <numbers>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

UI::UI(const Window &w) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // ImGuiIO &io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(w, false);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

UI::~UI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UI::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void UI::renderFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::display(Config &c) {
  // ImGui::ShowDemoWindow();
  if (ImGui::Begin("Configuration")) {
    ImGui::BeginGroup();
    ImGui::BeginChild("Tab view",
                      ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    if (ImGui::BeginTabBar("Tabs")) {
      showCameraTab(c.camera);
      showGenerationSettings(c.generation);
      showSettingsTab(c.settings);
      ImGui::EndTabBar();
    }
    ImGui::EndChild();
    showUIHelp();
    ImGui::EndGroup();
  }
  ImGui::End();
}

void UI::showCameraTab(Camera &cam) {
  if (ImGui::BeginTabItem("Camera")) {

    ImGui::InputFloat3("Position", glm::value_ptr(cam.pos));
    ImGui::SliderFloat("FOV", &cam.fov, 60, 130, "%.3f",
                       ImGuiSliderFlags_AlwaysClamp);
    ImGui::Text("Orientation:");
    ImGui::InputFloat("Horizontal", &cam.hOrient);
    ImGui::InputFloat("Vertical", &cam.vOrient);
    // cam.fov = std::clamp(cam.fov, 60.f, 130.f);
    // cam.hOrient = fmodf(cam.hOrient, 2 * std::numbers::pi);
    // cam.vOrient = fmodf(cam.vOrient, 2 * M_PI);

    ImGui::EndTabItem();
  }
}

void UI::showSettingsTab(Settings &set) {
  if (ImGui::BeginTabItem("Settings")) {

    ImGui::InputFloat("Camera speed", &set.cameraSpeed);
    ImGui::InputFloat("Mouse sensitivity", &set.mouseSensitivity);

    ImGui::EndTabItem();
  }
}

void UI::showGenerationSettings(GenerationSettings &gen) {
  if (ImGui::BeginTabItem("Generation")) {

    ImGui::Combo("Interpolation method", &gen.interpolationMethod,
                 "Smooth\0Linear\0\0");
    ImGui::SliderFloat(
        "Coordinate scaling", &gen.coordinateScaling, 0.1, 1000.0, "%.3f",
        ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

    ImGui::SliderFloat("Amplitude", &gen.amplitude, 0.5, 1000.0, "%.3f",
                       ImGuiSliderFlags_AlwaysClamp |
                           ImGuiSliderFlags_Logarithmic);
    ImGui::SliderInt("Layers", &gen.layers, 1, 10, "%d",
                     ImGuiSliderFlags_AlwaysClamp);

    ImGui::EndTabItem();
  }
}

void UI::showUIHelp() {
  if (ImGui::Button("Show UI Help"))
    ImGui::OpenPopup("UI Help");

  if (ImGui::BeginPopup("UI Help")) {

    ImGui::ShowUserGuide();
    ImGui::EndPopup();
  }
}

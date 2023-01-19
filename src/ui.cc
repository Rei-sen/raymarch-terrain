/* See the file "LICENSE" for the full license governing this code. */

#include "ui.hh"

#include <algorithm>
#include <numbers>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

UI::UI(const Window &w) {
  lastTime = glfwGetTime();
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
      showRenderingSettings(c.rendering);
      showSettingsTab(c.settings);
      showStats();
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

    ImGui::Text("Terrain");
    ImGui::InputInt("Max terrain rendering steps", &gen.terrainMaxSteps);
    ImGui::Combo("Interpolation method", &gen.interpolationMethod,
                 "Smooth\0Linear\0\0");
    ImGui::SliderFloat(
        "Coordinate scaling", &gen.coordinateScaling, 0.1, 1000.0, "%.3f",
        ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

    ImGui::SliderFloat("Amplitude", &gen.amplitude, 0.5, 1000.0, "%.3f",
                       ImGuiSliderFlags_AlwaysClamp |
                           ImGuiSliderFlags_Logarithmic);
    ImGui::SliderInt("Layers", &gen.layers, 1, 25, "%d",
                     ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Layer rotation", &gen.layerRotation, 0.0,
                       std::numbers::pi, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Hurst exponent", &gen.hurstExp, 0, 1, "%.3f",
                       ImGuiSliderFlags_AlwaysClamp);

    ImGui::ColorEdit3("Ground color", glm::value_ptr(gen.groundColor));
    ImGui::ColorEdit3("Sky color", glm::value_ptr(gen.skyColor));
    ImGui::SliderFloat2("Sun direction", glm::value_ptr(gen.sunDir),
                        -std::numbers::pi, std::numbers::pi, "%.3f",
                        ImGuiSliderFlags_AlwaysClamp);

    ImGui::Text("Grass");
    ImGui::Checkbox("Enable grass", &gen.enableGrass);
    ImGui::SliderFloat("Grass transition start", &gen.grassStart, 0.0,
                       gen.grassEnd, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Grass transition end", &gen.grassEnd, gen.grassStart,
                       1.0, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::ColorEdit3("Grass color", glm::value_ptr(gen.grassColor));

    ImGui::Text("Trees");
    ImGui::Checkbox("Enable trees", &gen.enableTrees);
    ImGui::InputInt("Max tree rendering steps", &gen.treesMaxSteps);
    ImGui::SliderFloat("Max tree generation distance", &gen.maxTreeDist, 0.0,
                       255.0);
    ImGui::InputFloat("Tree spacing", &gen.treeSpacing);
    ImGui::InputFloat("Tree radius", &gen.treeRadius);
    ImGui::InputFloat("Tree height", &gen.treeHeight);
    ImGui::InputFloat("Tree height offset", &gen.treeHeightOffset);
    ImGui::SliderInt("Tree Layers", &gen.treeLayers, 1, 10, "%d",
                     ImGuiSliderFlags_AlwaysClamp);
    ImGui::InputFloat("Tree distortion coordinate scaling",
                      &gen.treeDistortionCoordScaling);
    ImGui::InputFloat("Tree distortion amplitude",
                      &gen.treeDistortionAmplitude);
    ImGui::ColorEdit3("Tree color", glm::value_ptr(gen.treeColor));
    ImGui::ColorEdit3("Tree color 2", glm::value_ptr(gen.treeColor2));
    ImGui::InputFloat("Tree sufrace flatness", &gen.treeSurfaceFlatness);

    ImGui::Text("Clouds");
    ImGui::Checkbox("Enable clouds", &gen.enableClouds);
    ImGui::InputInt("Max cloud rendering steps", &gen.cloudsMaxSteps);

    ImGui::SliderFloat("Cloud altitude", &gen.cloudAltitude, 0.0, 500.0);
    ImGui::SliderFloat("Cloud height", &gen.cloudHeight, 0.0, 50.0);
    ImGui::SliderFloat(
        "Cloud coordinate scaling", &gen.cloudCoordinateScaling, 0.5, 1000,
        "%.3f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat(
        "Cloud amplitude", &gen.cloudAmplitude, 0.5, 1000, "%.3f",
        ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Cloud density scaling", &gen.cloudDensityScaling,
                       0.0001, 5.0, "%.6f", ImGuiSliderFlags_Logarithmic);

    ImGui::SliderInt("Cloud layers", &gen.cloudLayers, 1, 8, "%d",
                     ImGuiSliderFlags_AlwaysClamp);

    ImGui::EndTabItem();
  }
}

void UI::showRenderingSettings(RenderingSettings &rend) {
  if (ImGui::BeginTabItem("Rendering")) {

    // ImGui::Checkbox("Linear interpolation of last step",
    //                 &rend.enableInterpolation);
    ImGui::Checkbox("Enable shadows", &rend.enableShadows);

    ImGui::BeginDisabled(!rend.enableShadows);
    ImGui::Checkbox("Soft shadows", &rend.softShadows);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!rend.enableShadows || !rend.softShadows);
    ImGui::SliderFloat("Soft shadow range", &rend.softShadowRange, 0.5, 100.0,
                       "%.3f", ImGuiSliderFlags_Logarithmic);
    ImGui::EndDisabled();

    ImGui::Checkbox("Numerical normals", &rend.numericalNormals);
    ImGui::Checkbox("Enable sun glare", &rend.enableSunGlare);
    ImGui::ColorEdit3("Sun glare color", glm::value_ptr(rend.sunGlareColor));

    ImGui::Checkbox("Render raymarching iterations",
                    &rend.enableRenderingIterations);
    ImGui::BeginDisabled(!rend.enableRenderingIterations);
    ImGui::InputInt("Max raymarching iterations", &rend.maxRenderIterations);
    ImGui::EndDisabled();

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

void UI::showStats() {
  double currentTime = glfwGetTime();
  double dif = currentTime - lastTime;
  frames++;
  if (dif > 1.0) {
    lastTime += 1.0;
    framerate = frames / dif;

    frames = 0;
  }

  if (ImGui::BeginTabItem("Statistics")) {

    ImGui::Text("Framerate: %f", framerate);
    ImGui::Text("Frame time: %f ms", 1000 / framerate);

    ImGui::EndTabItem();
  }
}

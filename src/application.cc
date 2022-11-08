/* See the file "LICENSE" for the full license governing this code. */

#include "application.hh"

#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

#include "config.hh"

static void onMouseButton(GLFWwindow *window, int button, int action, int mods);

void onMouseButton(GLFWwindow *window, int button, int action, int mods) {
  if (ImGui::GetIO().WantCaptureMouse)
    return;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }
}

Application::Application() : win(), ui(win) {

  glfwSetMouseButtonCallback(win, onMouseButton);

  ImGui_ImplGlfw_InstallCallbacks(win);
}

Application::~Application() {}

void Application::run() {
  while (!win.shouldClose()) {
    win.pollEvents();
    processInput();

    ui.newFrame();

    // ImGui::ShowDemoWindow();
    ui.display(config);

    renderer.draw(config);

    ui.renderFrame();
    win.swapBuffers();
  }
}

void Application::processInput() {
  if (!ImGui::GetIO().WantCaptureKeyboard) {
    const float cameraSpeed = config.settings.cameraSpeed;

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(win, GLFW_TRUE);

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
      config.camera.move(glm::vec4{0, 0, -1, 0} * cameraSpeed);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
      config.camera.move(glm::vec4{0, 0, 1, 0} * cameraSpeed);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
      config.camera.move(glm::vec4{-1, 0, 0, 0} * cameraSpeed);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
      config.camera.move(glm::vec4{1, 0, 0, 0} * cameraSpeed);
  }

  if (!ImGui::GetIO().WantCaptureMouse) {
    static double lastX = 0, lastY = 0; // previous mouse position
    double curX, curY;                  // current mouse position
    glfwGetCursorPos(win, &curX, &curY);
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      config.camera.hOrient +=
          (lastX - curX) * config.settings.mouseSensitivity;
      config.camera.vOrient +=
          (lastY - curY) * config.settings.mouseSensitivity;
    }
    lastX = curX;
    lastY = curY;
  }
}

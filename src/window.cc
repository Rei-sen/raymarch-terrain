/* See the file "LICENSE" for the full license governing this code. */

#include <glad/glad.h>
// glad include before glfw
#include "window.hh"

#include <iostream>

#include <GLFW/glfw3.h>

static void glfwErrorCallback(int e, const char *text);
static void glfwWindowCloseCallback(GLFWwindow *w);

void glfwErrorCallback(int e, const char *text) {
  std::cerr << "GLFW error: " << e << " " << text << std::endl;
}

void glfwWindowCloseCallback(GLFWwindow *w) { glfwSetWindowShouldClose(w, 1); }

Window::Window() : Window(800, 600) {}

Window::Window(unsigned width, unsigned height) {
  glfwSetErrorCallback(glfwErrorCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  w = glfwCreateWindow(width, height, "TEST", NULL, NULL);

  if (!w) {
    throw std::runtime_error("Could not create glfw window");
  }

  glfwMakeContextCurrent(w);
  glfwSetWindowCloseCallback(w, glfwWindowCloseCallback);
  // glfwSetInputMode(w, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }
}

Window::~Window() {
  glfwDestroyWindow(w);
  ;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(w); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(w); }

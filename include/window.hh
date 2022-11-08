/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
  Window();
  Window(unsigned width, unsigned height);
  ~Window();

  bool shouldClose() const;
  void pollEvents();
  void swapBuffers();

  operator GLFWwindow *() const { return w; }

private:
  GLFWwindow *w;
};


#include <glad/glad.h>
// glad needs to be included before glfw
#include <GLFW/glfw3.h>

#include "application.hh"

int main(int argc, char **argv) {

  glfwInit();

  {
    Application app;

    app.run();
  }
  glfwTerminate();
}

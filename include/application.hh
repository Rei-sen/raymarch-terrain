/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <memory>

#include "config.hh"
#include "renderer.hh"
#include "ui.hh"
#include "window.hh"

class Application {
public:
  Application();
  ~Application();

  void run();
  void processInput();

private:
  Window win;
  UI ui;
  Config config;
  Renderer renderer;
};

/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include "window.hh"

#include "config.hh"

class UI {
public:
  UI(const Window &w);
  ~UI();

  void newFrame();
  void renderFrame();

  void display(Config &c);

private:
  void showCameraTab(Camera &cam);
  void showSettingsTab(Settings &set);
  void showGenerationSettings(GenerationSettings &gen);
  void showRenderingSettings(RenderingSettings &rend);
  void showUIHelp();
};

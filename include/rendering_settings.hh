/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <glm/vec3.hpp>

struct RenderingSettings {
  bool enableInterpolation = true;
  bool enableShadows = true;
  bool softShadows = true;
  float softShadowRange = 10.0;
  bool numericalNormals = false;
  bool enableSunGlare = true;
  glm::vec3 sunGlareColor{0.25, 0.175, 0.075};
  bool enableRenderingIterations = false;
  int maxRenderIterations = 255;
};

/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct Camera {
public:
  /// Update camera's position based on its orientation
  void move(glm::vec3 v);
  glm::mat4 getOrientationMatrix() const;

  glm::vec3 pos{0, 10, 0};
  float hOrient = 0;
  float vOrient = 0;
  float fov = 90;
};

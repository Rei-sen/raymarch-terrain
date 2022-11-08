/* See the file "LICENSE" for the full license governing this code. */

#include "camera.hh"

#include <glm/gtx/euler_angles.hpp>
#include <glm/vec4.hpp>

void Camera::move(glm::vec3 v) {
  pos += glm::vec3(getOrientationMatrix() * glm::vec4(v, 1.0f));
}

glm::mat4 Camera::getOrientationMatrix() const {
  return glm::yawPitchRoll(hOrient, vOrient, 0.0f);
}

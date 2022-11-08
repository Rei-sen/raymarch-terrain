/* See the file "LICENSE" for the full license governing this code. */

#include "rendering/GL/quad.hh"

#include <string>

std::unique_ptr<GL::Shader> quadVertexShader() {
  const std::string src = R"glsl(
#version 330 core

out vec2 fragPos;

const vec2 quadVertices[4] = vec2[4](
  vec2(-1.0, -1.0), vec2(-1.0, 1.0),
  vec2(1.0, 1.0), vec2(1.0, -1.0)
);

void main(void) {
  fragPos = quadVertices[gl_VertexID];
  gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}

)glsl";

  auto quad = std::make_unique<GL::Shader>(GL_VERTEX_SHADER);
  quad->loadSource(src);
  quad->compile();

  return quad;
}

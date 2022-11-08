/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <memory>

#include "config.hh"
#include "rendering/GL/program.hh"
#include "rendering/GL/shader.hh"
#include "rendering/GL/vao.hh"

class Renderer {
public:
  Renderer();
  ~Renderer();

  void update();
  void draw(const Config &c);

private:
  void updateUniforms(const Config &c);

  std::unique_ptr<GL::Program> program;
  std::unique_ptr<GL::Shader> vertexShader;
  GL::VAO vao;
};

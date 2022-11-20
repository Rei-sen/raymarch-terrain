/* See the file "LICENSE" for the full license governing this code. */

#include "renderer.hh"

#include <glm/gtc/type_ptr.hpp>

#include "rendering/GL/quad.hh"

static std::unique_ptr<GL::Shader> shaderFromConfig(/*const Config &c*/);

std::unique_ptr<GL::Shader> shaderFromConfig(/*const Config &c*/) {

  const std::string src =
#include "../glsl/sh.frag"
      ;
  auto sh = std::make_unique<GL::Shader>(GL_FRAGMENT_SHADER);
  sh->loadSource(src);
  sh->compile();

  return sh;
}

Renderer::Renderer() {
  vertexShader = quadVertexShader();

  auto fs = shaderFromConfig(/*config*/);

  program = std::make_unique<GL::Program>();
  program->attach(*vertexShader);
  program->attach(*fs);
  program->link();
  program->detach(*vertexShader);
  program->detach(*fs);
}

Renderer::~Renderer() {}

void Renderer::update() {}

void Renderer::draw(const Config &c) {
  glUseProgram(*program);
  vao.bind();

  updateUniforms(c);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Renderer::updateUniforms(const Config &c) {
  int p;
  // Camera uniforms
  p = glGetUniformLocation(*program, "cameraPos");
  glUniform4fv(p, 1, glm::value_ptr(c.camera.pos));
  p = glGetUniformLocation(*program, "cameraOrientationMatrix");
  glUniformMatrix4fv(p, 1, GL_FALSE,
                     glm::value_ptr(c.camera.getOrientationMatrix()));

  p = glGetUniformLocation(*program, "fov");
  glUniform1f(p, c.camera.fov);

  // Generation

  p = glGetUniformLocation(*program, "coordinateScaling");
  glUniform1f(p, c.generation.coordinateScaling);
  p = glGetUniformLocation(*program, "amplitude");
  glUniform1f(p, c.generation.amplitude);
  p = glGetUniformLocation(*program, "interpolationMethod");
  glUniform1i(p, c.generation.interpolationMethod);
  p = glGetUniformLocation(*program, "layers");
  glUniform1i(p, c.generation.layers);

  // Rendering
  p = glGetUniformLocation(*program, "enableInterpolation");
  glUniform1i(p, c.rendering.enableInterpolation);
}

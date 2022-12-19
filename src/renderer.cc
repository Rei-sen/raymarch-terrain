/* See the file "LICENSE" for the full license governing this code. */

#include "renderer.hh"

#include <GL/glext.h>
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

  p = glGetUniformLocation(*program, "terrainMaxSteps");
  glUniform1i(p, c.generation.terrainMaxSteps);
  p = glGetUniformLocation(*program, "coordinateScaling");
  glUniform1f(p, c.generation.coordinateScaling);
  p = glGetUniformLocation(*program, "amplitude");
  glUniform1f(p, c.generation.amplitude);
  p = glGetUniformLocation(*program, "interpolationMethod");
  glUniform1i(p, c.generation.interpolationMethod);
  p = glGetUniformLocation(*program, "layers");
  glUniform1i(p, c.generation.layers);
  p = glGetUniformLocation(*program, "layerRotation");
  glUniform1f(p, c.generation.layerRotation);
  p = glGetUniformLocation(*program, "hurstExp");
  glUniform1f(p, c.generation.hurstExp);

  p = glGetUniformLocation(*program, "groundColor");
  glUniform3fv(p, 1, glm::value_ptr(c.generation.groundColor));
  p = glGetUniformLocation(*program, "skyColor");
  glUniform3fv(p, 1, glm::value_ptr(c.generation.skyColor));

  p = glGetUniformLocation(*program, "enableGrass");
  glUniform1i(p, c.generation.enableGrass);
  p = glGetUniformLocation(*program, "grassStart");
  glUniform1f(p, c.generation.grassStart);
  p = glGetUniformLocation(*program, "grassEnd");
  glUniform1f(p, c.generation.grassEnd);
  p = glGetUniformLocation(*program, "grassColor");
  glUniform3fv(p, 1, glm::value_ptr(c.generation.grassColor));

  p = glGetUniformLocation(*program, "sunDir");
  {
    auto t = c.generation.sunDir;
    glUniform3f(p, cos(t.x) * cos(t.y), sin(t.y), sin(-t.x) * cos(t.y));
  }

  p = glGetUniformLocation(*program, "enableTrees");
  glUniform1i(p, c.generation.enableTrees);
  p = glGetUniformLocation(*program, "treesMaxSteps");
  glUniform1i(p, c.generation.treesMaxSteps);
  p = glGetUniformLocation(*program, "treeSpacing");
  glUniform1f(p, c.generation.treeSpacing);
  p = glGetUniformLocation(*program, "treeRadius");
  glUniform1f(p, c.generation.treeRadius);
  p = glGetUniformLocation(*program, "treeHeight");
  glUniform1f(p, c.generation.treeHeight);
  p = glGetUniformLocation(*program, "treeHeightOffset");
  glUniform1f(p, c.generation.treeHeightOffset);
  p = glGetUniformLocation(*program, "treeLayers");
  glUniform1i(p, c.generation.treeLayers);
  p = glGetUniformLocation(*program, "treeDistortionAmplitude");
  glUniform1f(p, c.generation.treeDistortionAmplitude);
  p = glGetUniformLocation(*program, "treeDistortionCoordScaling");
  glUniform1f(p, c.generation.treeDistortionCoordScaling);
  p = glGetUniformLocation(*program, "treeColor");
  glUniform3fv(p, 1, glm::value_ptr(c.generation.treeColor));
  p = glGetUniformLocation(*program, "treeColor2");
  glUniform3fv(p, 1, glm::value_ptr(c.generation.treeColor2));

  p = glGetUniformLocation(*program, "enableClouds");
  glUniform1i(p, c.generation.enableClouds);
  p = glGetUniformLocation(*program, "cloudsMaxSteps");
  glUniform1i(p, c.generation.terrainMaxSteps);
  p = glGetUniformLocation(*program, "cloudAltitude");
  glUniform1f(p, c.generation.cloudAltitude);
  p = glGetUniformLocation(*program, "cloudHeight");
  glUniform1f(p, c.generation.cloudHeight);
  p = glGetUniformLocation(*program, "cloudLayers");
  glUniform1i(p, c.generation.cloudLayers);
  p = glGetUniformLocation(*program, "cloudCoordinateScaling");
  glUniform1f(p, c.generation.cloudCoordinateScaling);
  p = glGetUniformLocation(*program, "cloudAmplitude");
  glUniform1f(p, c.generation.cloudAmplitude);
  p = glGetUniformLocation(*program, "cloudDensityScaling");
  glUniform1f(p, c.generation.cloudDensityScaling);
  p = glGetUniformLocation(*program, "treeSurfaceFlatness");
  glUniform1f(p, c.generation.treeSurfaceFlatness);

  // Rendering
  p = glGetUniformLocation(*program, "enableInterpolation");
  glUniform1i(p, c.rendering.enableInterpolation);
  p = glGetUniformLocation(*program, "enableShadows");
  glUniform1i(p, c.rendering.enableShadows);
  p = glGetUniformLocation(*program, "enableSoftShadows");
  glUniform1i(p, c.rendering.softShadows);
  p = glGetUniformLocation(*program, "softShadowRange");
  glUniform1f(p, c.rendering.softShadowRange);

  p = glGetUniformLocation(*program, "numericalNormals");
  glUniform1i(p, c.rendering.numericalNormals);

  p = glGetUniformLocation(*program, "enableSunGlare");
  glUniform1i(p, c.rendering.enableSunGlare);
  p = glGetUniformLocation(*program, "sunGlareColor");
  glUniform3fv(p, 1, glm::value_ptr(c.rendering.sunGlareColor));
}

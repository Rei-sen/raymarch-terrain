/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

namespace GL {

class Shader {
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

public:
  Shader() = delete;

  Shader(GLenum type);
  ~Shader();

  void compile();
  void loadSource(const std::vector<GLchar> &data);
  void loadSource(const std::string &data);

  bool isValid() const;

  operator int() const { return shader; };

private:
  void del();

  GLuint shader = 0;
};

} // namespace GL

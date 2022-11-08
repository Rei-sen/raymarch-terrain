/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <glad/glad.h>

namespace GL {

class VAO {
  VAO(const VAO &) = delete;
  VAO &operator=(const VAO &) = delete;

public:
  VAO();
  ~VAO();

  void bind();

  operator int() const;

private:
  GLuint vao;
};

} // namespace GL

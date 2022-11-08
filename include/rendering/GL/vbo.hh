/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <glad/glad.h>

namespace GL {

class VBO {
  VBO(const VBO &) = delete;
  VBO &operator=(const VBO &) = delete;

public:
  VBO();
  ~VBO();

  void bind(GLenum target);
  void fill(GLenum target, GLsizeiptr size, const void *data,
            GLenum usage = GL_STATIC_DRAW);

  operator int() const { return vbo; }

private:
  unsigned int vbo;
};

} // namespace GL

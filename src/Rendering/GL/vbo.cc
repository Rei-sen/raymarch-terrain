/* See the file "LICENSE" for the full license governing this code. */

#include "rendering/GL/vbo.hh"

namespace GL {

VBO::VBO() { glGenBuffers(1, &vbo); }

VBO::~VBO() { glDeleteBuffers(1, &vbo); }

void VBO::bind(GLenum target) { glBindBuffer(target, vbo); }

void VBO::fill(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
  bind(target);
  glBufferData(target, size, data, usage);
}

} // namespace GL

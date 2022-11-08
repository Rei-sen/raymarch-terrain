/* See the file "LICENSE" for the full license governing this code. */

#include "rendering/GL/vao.hh"

namespace GL {

VAO::VAO() { glGenVertexArrays(1, &vao); }

VAO::~VAO() { glDeleteVertexArrays(1, &vao); }

void VAO::bind() { glBindVertexArray(vao); }

VAO::operator int() const { return vao; }

} // namespace GL

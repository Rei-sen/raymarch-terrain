/* See the file "LICENSE" for the full license governing this code. */

#include "rendering/GL/shader.hh"

#include <stdexcept>

namespace GL {

Shader::Shader(GLenum type) { shader = glCreateShader(type); }

Shader::~Shader() { glDeleteShader(shader); }

void Shader::compile() {
  glCompileShader(shader);

  if (!isValid()) {
    GLint maxLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
    std::string msg;
    msg.resize(size_t(maxLen) + 1);
    glGetShaderInfoLog(shader, maxLen, &maxLen, msg.data());
    msg.resize(size_t(maxLen));
    throw std::runtime_error("Error while compiling shader: " + msg);
  }
}

void Shader::loadSource(const std::vector<GLchar> &data) {
  const GLchar *d = data.data();
  GLint len = GLint(data.size());
  glShaderSource(shader, 1, &d, &len);
}

void Shader::loadSource(const std::string &data) {
  const GLchar *d = data.data();
  glShaderSource(shader, 1, &d, nullptr);
}

bool Shader::isValid() const {
  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  return isCompiled == GL_TRUE;
}

} // namespace GL

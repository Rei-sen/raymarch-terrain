/* See the file "LICENSE" for the full license governing this code. */

#include "rendering/GL/program.hh"

namespace GL {

Program::Program() { program = glCreateProgram(); }

Program::~Program() { glDeleteProgram(program); }

void Program::attach(const Shader &shader) { glAttachShader(program, shader); }

void Program::detach(const Shader &shader) { glDetachShader(program, shader); }

void Program::link() { glLinkProgram(program); }

void Program::use() { glUseProgram(program); }

bool Program::isValid() const { return program; }

} // namespace GL

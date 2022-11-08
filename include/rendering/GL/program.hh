/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include "rendering/GL/shader.hh"

#include <map>
#include <string>

namespace GL {

class Program {
  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;

public:
  Program();
  ~Program();

  void attach(const Shader &shader);
  void detach(const Shader &shader);

  void link();
  void use();

  bool isValid() const;

  operator int() const { return program; }

private:
  std::map<std::string, int> uniforms;
  int program;
};

} // namespace GL

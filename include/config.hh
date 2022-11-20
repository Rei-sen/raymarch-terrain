/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include "camera.hh"
#include "generation_settings.hh"
#include "rendering_settings.hh"
#include "settings.hh"

struct Config {
  Camera camera;
  Settings settings;
  GenerationSettings generation;
  RenderingSettings rendering;
};

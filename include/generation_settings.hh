/* See the file "LICENSE" for the full license governing this code. */

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct GenerationSettings {
  int terrainMaxSteps = 256;
  float coordinateScaling = 8.0;
  float amplitude = 4.0;
  int interpolationMethod = 0;
  int layers = 10;
  float layerRotation = 0.8;
  float hurstExp = 1.0;

  glm::vec3 groundColor{0.9, 0.7, 0.6};
  glm::vec3 skyColor{0.55, 0.80, 1.0};
  glm::vec2 sunDir{1.5, 0.4};

  bool enableGrass = true;
  float grassStart = 0.6;
  float grassEnd = 0.7;
  glm::vec3 grassColor{0.53, 0.53, 0.05};

  bool enableTrees = false;
  int treesMaxSteps = 100;
  float maxTreeDist = 50.0;
  float treeSpacing = 0.07;
  float treeRadius = 0.02;
  float treeHeight = 0.08;
  float treeHeightOffset = 0.04;
  int treeLayers = 3;
  float treeDistortionAmplitude = 0.04;
  float treeDistortionCoordScaling = 30.0;
  glm::vec3 treeColor{0.33, 0.39, 0.10};
  glm::vec3 treeColor2{0.33, 0.39, 0.10};
  float treeSurfaceFlatness = 0.7;

  bool enableClouds = true;
  int cloudsMaxSteps = 100;
  float cloudAltitude = 100.0;
  float cloudHeight = 5.0;
  int cloudLayers = 4;
  float cloudCoordinateScaling = 50.0;
  float cloudAmplitude = 5.0;
  float cloudDensityScaling = 0.15;
};

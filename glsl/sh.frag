R""(
#version 330 core

in vec2 fragPos;
out vec4 fragColor;


// Camera
uniform vec4 cameraPos;
uniform mat4 cameraOrientationMatrix;
uniform float fov;

// Generation
uniform float coordinateScaling;
uniform float amplitude;
uniform int interpolationMethod;
uniform int layers;

// Rendering
uniform bool enableInterpolation;

const int MAX_STEPS = 100;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

// https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 b) {
  return fract(sin(dot(b, vec2(12.9898, 78.233))) * 43758.5453);
}

// float noise(vec2 n) {
//   const vec2 d = vec2(0.0, 1.0);
//     vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
//   return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
// }


float bilinearInterpolation(vec2 p, float a, float b, float c, float d) {
    // return mix(mix(a, b, p.x), mix(c, d, p.x), p.y);
    return a +
        (b - a) * p.x +
        (c - a) * p.y +
        (a - b - c + d) * p.x * p.y;
}

float smoothInterpolation(vec2 p, float a, float b, float c, float d) {
    vec2 t =  p * p * (3.0 - 2.0 * p);
    return a +
        (b - a) * t.x +
        (c - a) * t.y +
        (a - b - c + d) * t.x * t.y;
}


float getTerrainHeightAtPoint(vec2 p) {
    const vec2 c = vec2(0.0, 1.0);
    vec2 offset = fract(p);
    p -= offset;

    if (interpolationMethod == 0)
        return smoothInterpolation(offset, rand(p), rand(p + c.yx),
                                   rand(p + c.xy), rand(p + c.yy));
    else
        return bilinearInterpolation(offset, rand(p), rand(p + c.yx),
                                     rand(p + c.xy), rand(p + c.yy));
}

float fbm(vec2 p) {

    // tmp
    float sum = 0.0;
    float a = amplitude;

    for (int i = 0; i < layers; i++) {
        sum += getTerrainHeightAtPoint(p) * a;
        mat2 m = mat2(
            4.0/5.0, -3.0/5.0,
            3.0/5.0, 4.0/5.0
        );
        p *= m;
        p *= 2.0;
        a /= 2.0;
    }
    return sum;
}

float terrain(vec2 p) {

    p /= coordinateScaling;

    return fbm(p);
}

float sphereSDF(vec3 p) {
  return length(p) - 1.0;
}

float groundSDF(vec3 p) {
  return p.y - terrain(p.xz);
}

float sceneSDF(vec3 samplePoint) {
  return min(sphereSDF(samplePoint), groundSDF(samplePoint));
}


vec3 gradient(vec3 p) {
  vec2 e = vec2(EPSILON, 0);
  float d = sceneSDF(p);
  return normalize(vec3(sceneSDF(p + e.xyy) - d,
                        sceneSDF(p + e.yxy) - d,
                        sceneSDF(p + e.yyx) - d));
}

float map(vec3 eye, vec3 rayDir, float start, float end) {
  float depth = start;
  float lh = 0.0, ly = 0.0;

  float e = EPSILON;

  for (int i = 0; i < MAX_STEPS; i++) {
    float dist = sceneSDF(eye + depth * rayDir);
    if (dist < e ) {
      return depth;
    }

    depth += dist;
    if (depth >= end) {
      return end;
    }
    e = EPSILON / sqrt(depth * 100);
  }
  return end;
}

vec3 rayDirection(vec2 pos) {
  vec2 xy = pos / 2.0;
  float z = 1.0 / tan(radians(fov) / 2.0);
  return normalize(vec3(xy, -z));
}


void main(void) {
  vec3 dir = (cameraOrientationMatrix * vec4(rayDirection(fragPos), 1.0)).xyz;

  float dist = map(cameraPos.xyz, dir, MIN_DIST, MAX_DIST);

  vec3 endColor;
  if (dist > MAX_DIST - EPSILON) {
    endColor = vec3(0.95, 0.98, 1.0);
    // endColor = vec3(1.0);
  } else {
    vec3 normal = gradient(cameraPos.xyz + dir * dist);
    endColor = vec3(max(dot(normal, normalize(vec3(1,2,3))), 0.0)) * vec3(0.86, 0.68, 0.68);
    vec3 grey = vec3(0.95);
    endColor = mix(grey, endColor, exp(-0.005 * dist * vec3(1,2,4)));
  }

  // endColor = pow(endColor, vec3(1.0/2.2));
  fragColor = vec4(endColor, 1.0);
}
)""

R""(
#version 330 core

in vec2 fragPos;
out vec4 fragColor;


// Camera
uniform vec4 cameraPos;
uniform mat4 cameraOrientationMatrix;
uniform float fov;

// Generation
uniform int terrainMaxSteps;
uniform float coordinateScaling;
uniform float amplitude;
uniform int interpolationMethod;
uniform int layers;
uniform float layerRotation;
uniform float hurstExp;

uniform vec3 groundColor;
uniform vec3 skyColor;
uniform vec3 sunDir;

// Grass
uniform int enableGrass;
uniform float grassStart;
uniform float grassEnd;
uniform vec3 grassColor;

// Tree
uniform int enableTrees;
uniform int treesMaxSteps;
uniform float treesMaxDistance;
uniform float treeSpacing;
uniform float treeRadius;
uniform float treeHeight;
uniform float treeHeightOffset;
uniform int treeLayers;
uniform vec3 treeColor;
uniform vec3 treeColor2;

uniform int enableClouds;
uniform int cloudsMaxSteps;
uniform float cloudAltitude;
uniform float cloudHeight;
uniform int cloudLayers;
uniform float cloudCoordinateScaling;
uniform float cloudAmplitude;
uniform float cloudDensityScaling;

uniform float treeDistortionAmplitude;
uniform float treeDistortionCoordScaling;
uniform float treeSurfaceFlatness;

// Rendering
uniform bool enableInterpolation;
uniform bool enableShadows;
uniform bool enableSoftShadows;
uniform float softShadowRange;
uniform int numericalNormals;
uniform int enableSunGlare;
uniform vec3 sunGlareColor;
uniform int renderIterations;
uniform int maxRenderIterations;

const int MAX_STEPS = 256;
const float MIN_DIST = 0.0;
const float MAX_DIST = 250.0;
const float EPSILON = 0.001;


struct Noise2D {
  float value;
  vec2 derivative;
};

struct Noise3D {
  float value;
  vec3 derivative;
};

// https://iquilezles.org/articles/distfunctions/
float sdEllipsoid( vec3 p, vec3 r) {
  float k0 = length(p / r);
  float k1 = length(p / (r * r));
  return k0 * (k0 - 1.0) / k1;
}

// tmp
float hash1( float n ) {
    return fract( n*17.0*fract( n*0.3183099 ) );
}

float rand(float b) {
    return hash1(b);
}
// https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 b) {
  return fract(sin(dot(b, vec2(12.9898, 78.233))) * 43758.5453);
}
float rand(vec3 p) {
  float f = p.x + 311.0 * p.y + 919.0 * p.z;
  return hash1(f);
  // return fract(sin(dot(b, vec3(12.9898, 47.5487, 78.233))) * 43758.5453);
}

// Noise3D noise_linear

Noise2D noise_linear(vec2 p) {

  vec2 r = floor(p);
  vec2 offset = fract(p);

  float a = rand(r);
  float b = rand(r + vec2(1,0));
  float c = rand(r + vec2(0,1));
  float d = rand(r + vec2(1,1));

  float w0 = a;
  float w1 = b - a;
  float w2 = c - a;
  float w3 = a - b - c + d;

  return Noise2D(w0 +
                 w1 * offset.x +
                 w2 * offset.y +
                 w3 * offset.x * offset.y,
                 vec2(w1 + w3 * offset.y, w2 + w3 * offset.x));
}

Noise2D noise_smooth(vec2 p) {

  vec2 r = floor(p);
  vec2 offset = fract(p);

  float a = rand(r);
  float b = rand(r + vec2(1,0));
  float c = rand(r + vec2(0,1));
  float d = rand(r + vec2(1,1));

  float w0 = a;
  float w1 = b - a;
  float w2 = c - a;
  float w3 = a - b - c + d;

  vec2 t =  offset * offset * (3.0 - 2.0 * offset);
  // S = 3p^2 - 2p^3
  // S' = 6p - 6p^2
  vec2 dt = 6.0 * offset * (1.0 - offset);

  return Noise2D(w0 +
                 w1 * t.x +
                 w2 * t.y +
                 w3 * t.x * t.y,
                 dt * vec2(w1 + w3 * t.y, w2 + w3 * t.x));
}

// stolen
vec4 noise_3d( in vec3 x )
 {
    vec3 p = floor(x);
    vec3 w = fract(x);

    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);

    float a = rand( p+vec3(0,0,0) );
    float b = rand( p+vec3(1,0,0) );
    float c = rand( p+vec3(0,1,0) );
    float d = rand( p+vec3(1,1,0) );
    float e = rand( p+vec3(0,0,1) );
    float f = rand( p+vec3(1,0,1) );
    float g = rand( p+vec3(0,1,1) );
    float h = rand( p+vec3(1,1,1) );

    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4( -1.0+2.0*(k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z),
                 2.0* du * vec3( k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                                 k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                                 k3 + k6*u.x + k5*u.y + k7*u.x*u.y ) );
}

// vec3 smooth_d(vec2 p, float a, float b, float c, float d) {
//   float w0 = a;
//   float w1 = b - a;
//   float w2 = c - a;
//   float w3 = a - b - c + d;

//   vec2 t =  p * p * (3.0 - 2.0 * p);
//   // S = 3p^2 - 2p^3
//   // S' = 6p - 6p^2
//   vec2 dt = 6.0 * p * (1.0 - p);

//   return vec3(w0 +
//               w1 * t.x +
//               w2 * t.y +
//               w3 * t.x * t.y,
//               dt * vec2(w1 + w3 * t.y,
//                         w2 + w3 * t.x));
// }

vec3 terrain_d(vec2 p) {

  if (interpolationMethod == 0) {

    // const vec2 c = vec2(0.0, 1.0);
    // vec2 offset = fract(p);
    // p -= offset;
    Noise2D r = noise_smooth(p);
    // return smooth_d(offset, rand(p), rand(p + c.yx),
    //                         rand(p + c.xy), rand(p + c.yy));
    return vec3(r.value, r.derivative);
  } else {
    Noise2D r = noise_linear(p);
    return vec3(r.value, r.derivative);
  }
      // return bilin_d(offset, rand(p), rand(p + c.yx),
      //                          rand(p + c.xy), rand(p + c.yy));

}

vec3 fbm_d(vec2 p, int l) {
    mat2 rot = mat2(
            cos(layerRotation), -sin(layerRotation),
            sin(layerRotation), cos(layerRotation)
        );

    mat2 rot_i = transpose(rot);

    float G = exp2(-hurstExp);
    float sum = 0.0;
    float a = 1.0;
    float f = 2.0;
    vec2 d = vec2(0.0, 0.0);

    mat2 m = mat2(1.0);

    for (int i = 0; i < l; i++) {
        vec3 n = terrain_d(p);
         sum += n.x * a;

        d += a * m * n.yz;
        m = f * rot_i * m;
        p = f * rot * p;

        a *= G;

    }
    return vec3(sum, d);
}

vec4 fbm_3d(vec3 p, int l) {
  float G = exp2(-hurstExp);
  float sum = 0.0;
  float a = 1.0;
  float f = 2.0;
  vec3 d = vec3(0.0, 0.0, 0.0);

  for (int i = 0; i < l; i++) {
    vec4 n = noise_3d(p);
    sum += n.x * a;

    d += a * n.yzw;
    p = f * p;

    a *= G;
  }

  return vec4(sum, d);
}

vec3 rayDirection(vec2 pos) {
  vec2 xy = pos / 2.0;
  float z = 1.0 / tan(radians(fov) / 2.0);
  return normalize(vec3(xy, -z));
}

// Terrain
vec4 terrainMap_d (vec2 p) {
  p /= coordinateScaling;
  vec3 res = fbm_d(p, layers);
  vec2 d = (res.yz / coordinateScaling) * amplitude;
  // vec2 d = res.yz;
  return vec4(res.x * amplitude, normalize(vec3(-d.x, 1.0, -d.y)));
}

float terrainShadow(vec3 ro) {

  float depth = 0.0; // total traveled distance
  float prec = EPSILON; // precision
  float minR = MAX_DIST;

  for (int i = 0; i < MAX_STEPS; i++) {
    vec3 pos = ro + sunDir * depth;
    float env = terrainMap_d(pos.xz).x;
    float h = pos.y - env;
    if (enableSoftShadows) {
      float r = h/depth;
      minR = min(r, minR);
    }
    float d = h * 0.7;
    depth += d;
    prec = max(EPSILON, depth * EPSILON);

    if (abs(d) < prec )
        return 0.0;

    if (d > MAX_DIST) break;
  }
  if (enableSoftShadows)
      return smoothstep(0.0, 1.0, softShadowRange * minR);
  else
      return 1.0;
}


vec4 raymarchTerrain_d(vec3 ro, vec3 rd, inout int iterations) {
  vec3 delta = vec3(0.0,0.0,0.0);
  float depth = 0.0; // total traveled distance
  float prec = EPSILON; // precision
  for (int i = 0; i < terrainMaxSteps; i++) {
    iterations += 1;
    vec3 pos = ro + rd * depth;
    vec4 env = terrainMap_d(pos.xz);
    float h = pos.y - env.x;
    delta = env.yzw;
    float d = h * 0.7;
    depth += d;
    prec = max(EPSILON, 2 * depth * EPSILON);

    if (abs(d) < prec)
      return vec4(depth, delta);
    if (depth > MAX_DIST) break;
  }
  return vec4(MAX_DIST, delta);

    // if (abs(d) < prec || depth > MAX_DIST) break;
  // }

  // return vec4(depth, delta);
}

vec3 gradient(vec3 p) {
  vec2 e = vec2(EPSILON, 0);
  float d = terrainMap_d(p.xz).x;
  return normalize(vec3(d - terrainMap_d(p.xz + e.xy).x,
                        e.x,
                        d - terrainMap_d(p.xz + e.yx).x));
}

// Trees
float treeMap(vec3 p) {
    vec2 local = floor(p.xz / treeSpacing);
    local = local * treeSpacing + vec2(treeSpacing * 0.5);
    vec2 distortion = vec2(rand(local), rand(local.yx));
    distortion =// smoothstep(0.0, 1.0, distortion) *
        distortion *
        (1.0 - 2.0 * treeRadius/treeSpacing);
    distortion -= distortion * 0.5;
    distortion *= treeSpacing;
    local += distortion;
    vec4 t = terrainMap_d(local);
    vec3 center = vec3(local.x, t.x, local.y);
    // vec3 center = vec3(local.x, 10.0, local.y);

    vec3 dif = p - center;

    dif.y -= treeHeightOffset;

    if (t.z < treeSurfaceFlatness) {
      float d = sdEllipsoid(dif, vec3(treeRadius, treeHeight, treeRadius));

      return max(treeSpacing, d);
    }
    // return vec4(min(length(dif) - r, t), normalize(dif));
    // fbm_3d(dif, 1);
    float s = fbm_3d(p * treeDistortionCoordScaling, treeLayers).x;
    // float s = 0.0;
    s = s*s;
    s *= treeDistortionAmplitude;
    return sdEllipsoid(dif, vec3(treeRadius, treeHeight, treeRadius)) + s;
}

float raymarchTrees(vec3 ro, vec3 rd, inout int iterations) {
  float depth = 0.0; // total traveled distance
  float prec = EPSILON; // precision
  for (int i = 0; i < treesMaxSteps; i++) {
    iterations += 1;
    vec3 pos = ro + rd * depth;
    float d = treeMap(pos);
    depth += d * 0.7;
    prec = max(EPSILON, depth * EPSILON);

    if (abs(d) < prec) return depth;
    if (depth > treesMaxDistance) break;
  }
  return MAX_DIST;
}

vec3 treeNormal(vec3 p) {
  vec2 e = vec2(EPSILON, 0);
  float d = treeMap(p);
  return normalize(-vec3(d - treeMap(p + e.xyy),
                        d -treeMap(p + e.yxy),
                        d - treeMap(p + e.yyx)));
}

float treeShadow(vec3 ro) {

  float depth = 0.0; // total traveled distance
  float prec = EPSILON; // precision
  float minR = MAX_DIST;

  for (int i = 0; i < MAX_STEPS; i++) {
    vec3 pos = ro + sunDir * depth;
    float d = treeMap(pos);
    if (enableSoftShadows) {
      float r = d/depth;
      minR = min(r, minR);
    }
    depth += d;
    prec = max(EPSILON, depth * EPSILON);

    if (abs(d) < prec )
        return 0.0;

    if (d > treesMaxDistance) break;
  }
  if (enableSoftShadows)
      return smoothstep(0.0, 1.0, softShadowRange * minR);
  else
      return 1.0;

}

// Clouds
float cloudMap(vec3 p) {
  float d = abs(p.y - cloudAltitude) - cloudHeight;
  // vec3 f = fbm_d(p.xz / coordinateScaling) * amplitude;
  float f = fbm_d(p.xz / cloudCoordinateScaling, cloudLayers).x
      * cloudAmplitude;
  // return vec4(d + f.x, normalize(vec3(-f.y, 1.0, -f.z)));
  return d + f;
}

vec4 raymarchClouds(vec3 ro, vec3 rd, inout int iterations) {
  // vec3 delta = vec3(0.0,0.0,0.0);
  float depth = 0.0; // total traveled distance
  float prec = EPSILON; // precision

  float density = 0.0;
  int densitySamples = 0;
  float stepSize = max(0.2, 0.011 * depth);


  float absorption = 100.0;
  float T = 1.0;
  vec3 color = vec3(0.0);
  float hit = 0.0;
  for (int i = 0; i < cloudsMaxSteps; i++) {
    iterations += 1;
    vec3 pos = ro + rd * depth;
    if (rd.y > 0 && pos.y > (cloudAltitude + cloudHeight)) break;
    if (rd.y < 0 && pos.y < (cloudAltitude - cloudHeight)) break;

    float d = cloudMap(pos);
    if (d < 0) {
      hit = 1.0;
        // float a = abs(d) / float(MAX_STEPS);
        // T *= 1.0 - (a * absorption);
      density += cloudDensityScaling * abs(d);
      densitySamples += 1;

      if (T <= 0.01) break;

        // color += vec3(1.0) * 500 * a * T;

      depth += stepSize;
    } else {
      if (d < 3 * stepSize)
        depth += stepSize;
      else
        depth += abs(d) * 0.7;
    }
    // delta = env.yzw;
  }
  if (densitySamples > 0)
    // color = vec3(1.0) * density/densitySamples;
    color = vec3(1.0) * smoothstep(0.0, 1.0, density/densitySamples);
  // return vec4(depth, delta);
  return vec4(hit, color);
}

float shadow(vec3 ro) {
  float s = 1.0;
  s = min(s, terrainShadow(ro));
  if (enableTrees == 1) {
    s = min(s, treeShadow(ro));
  }

  return s;
}

// ro - ray origin
// rd - ray direction
vec4 render(vec3 ro, vec3 rd) {

  int iterations = 0;
  vec4 d = vec4(MAX_DIST);
  int obj = 0; // object hit
  vec3 col = clamp(skyColor * mix(1.0, 0.6, rd.y), 0.0, 1.0);
  // vec3 col = skyColor - 0.4 * rd.y;
  vec4 t;

  // terrain
  t = raymarchTerrain_d(ro, rd, iterations);
  iterations += iterations;
  d = t;
  // vec4 t = vec4(MAX_DIST);
  if (t.x < MAX_DIST) {
    d = t;
    obj = 1;
  }

  if (enableTrees == 1) {
    float tt = raymarchTrees(ro, rd, iterations);
    iterations += iterations;
    if (tt < d.x && tt < MAX_DIST) {
      d.x = tt;
      d.yzw = treeNormal(ro + rd * tt);
      obj = 2;
    }
  }

  if (enableClouds == 1) {
    vec4 cd = raymarchClouds(ro, rd, iterations);
    iterations += iterations;
    if (cd.x > 0.0) {
      // col = vec3(1.0) * clamp(dot(cd.yzw, sunDir), 0.0, 1.0);
      col += cd.yzw;
      // obj = 0
    }
  }


  if (obj > 0) {
    vec3 material;
    vec3 norm;
    if (obj == 1) {

      if (numericalNormals == 0)
        norm = normalize(d.yzw);
      else
        norm = normalize(gradient(ro + rd*d.x));

      material = groundColor;
      if (enableGrass == 1) {
           material = mix(material, grassColor,
                          smoothstep(grassStart, grassEnd, norm.y));
      }

    //   // vec3 sunDir = normalize(vec3(0,1.0, 0.5));

    //   col = max(dot(norm, sunDir), 0.0) * material * sh
    //       + (0.5 + 0.5 * norm.y) * skyColor / 10.0
    //       + max(dot(norm, -sunDir), 0.0) * groundColor/ 10.0
    //       ;

    } else if (obj == 2) {
        norm = d.yzw;
    //     float sh = 1.0;
    //     if (enableShadows) sh = shadow(ro + rd * d.x + norm * 0.01);
    //     col = max(dot(norm, sunDir), 0.0) * treeColor * sh;
        material = mix(treeColor, treeColor2,
                        fbm_d((ro + rd * d.x).xz, 1).x);
    }
      float sh = 1.0;
      if (enableShadows) {
        sh = shadow(ro + rd * d.x + sunDir * 0.01);
      }


      col = max(dot(norm, sunDir), 0.0) * material * sh
          + (0.5 + 0.5 * norm.y) * (skyColor / 10.0)
          + max(dot(norm, -sunDir), 0.0) * groundColor / 10.0
          ;

  } else {

  }

  if (enableSunGlare == 1) {
    col += pow(max(dot(rd, sunDir), 0.0), 4) * sunGlareColor;
  }

  if (renderIterations == 1) {
    int m = maxRenderIterations;
    if (iterations < m) {
      col = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 0.0), smoothstep(0.0, 1.0, float(iterations) / m));
    } else {
      col = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), smoothstep(0.0, 1.0, float(iterations - m) / m));
    }
  }
  col = pow(col, vec3(1.0/2.2));

  return vec4(col, 1.0);
}

void main(void) {
  vec3 dir = (cameraOrientationMatrix * vec4(rayDirection(fragPos), 0.0)).xyz;

  /*
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
  fragColor = vec4(endColor, 1.0);*/

  fragColor = render(cameraPos.xyz, dir);
}
)""

#pragma once
#include "scene.h"
#include "noise.h"

namespace zv {

class ProceduralMesh {
public:
    // Basic primitives
    static Mesh cube(float size = 1.0f);
    static Mesh sphere(float radius = 1.0f, int segments = 16);
    static Mesh cylinder(float radius = 1.0f, float height = 2.0f, int segments = 16);
    static Mesh capsule(float radius = 1.0f, float height = 2.0f, int segments = 16);
    static Mesh plane(float width = 1.0f, float depth = 1.0f, int subdivs = 1);
    static Mesh torus(float majorR = 1.0f, float minorR = 0.3f, int segs = 16);

    // Deformed primitives
    static Mesh noiseSphere(float radius, int segments, const NoiseGenerator& ng, float amplitude = 0.1f);
    static Mesh terrain(float width, float depth, int resolution, const NoiseGenerator& ng, float heightScale = 10.0f);
    static Mesh crystal(float size, int faces, const NoiseGenerator& ng);

    // Mesh operations
    static void subdivide(Mesh& mesh);
    static void displace(Mesh& mesh, const NoiseGenerator& ng, float amplitude);
    static void smooth(Mesh& mesh, int iterations = 1);
    static void weldVertices(Mesh& mesh, float threshold = 0.001f);

private:
    static Vec3 sphericalToCartesian(float r, float theta, float phi);
};

} // namespace zv

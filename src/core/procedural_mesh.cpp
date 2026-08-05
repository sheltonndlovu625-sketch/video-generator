#include "zingvid/procedural_mesh.h"
#include <cmath>
#include <unordered_map>

namespace zv {

Mesh ProceduralMesh::cube(float size) {
    Mesh m;
    float h = size * 0.5f;
    m.vertices = {
        {{-h,-h,-h},{-1,-1,-1},{0,0},{1,1,1,1}}, {{h,-h,-h},{1,-1,-1},{1,0},{1,1,1,1}},
        {{h,h,-h},{1,1,-1},{1,1},{1,1,1,1}}, {{-h,h,-h},{-1,1,-1},{0,1},{1,1,1,1}},
        {{-h,-h,h},{-1,-1,1},{0,0},{1,1,1,1}}, {{h,-h,h},{1,-1,1},{1,0},{1,1,1,1}},
        {{h,h,h},{1,1,1},{1,1},{1,1,1,1}}, {{-h,h,h},{-1,1,1},{0,1},{1,1,1,1}}
    };
    m.indices = {
        0,1,2, 0,2,3, 1,5,6, 1,6,2, 5,4,7, 5,7,6,
        4,0,3, 4,3,7, 3,2,6, 3,6,7, 4,5,1, 4,1,0
    };
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::sphere(float radius, int segments) {
    Mesh m;
    for(int lat = 0; lat <= segments; lat++) {
        float theta = lat * 3.14159265f / segments;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);
        for(int lon = 0; lon <= segments; lon++) {
            float phi = lon * 2.0f * 3.14159265f / segments;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            Vec3 pos = {radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi};
            Vec3 n = {sinTheta * cosPhi, cosTheta, sinTheta * sinPhi};
            m.vertices.push_back({pos, n, {(float)lon/segments, (float)lat/segments}, {1,1,1,1}});
        }
    }
    for(int lat = 0; lat < segments; lat++) {
        for(int lon = 0; lon < segments; lon++) {
            int first = lat * (segments + 1) + lon;
            int second = first + segments + 1;
            m.indices.push_back(first); m.indices.push_back(second); m.indices.push_back(first+1);
            m.indices.push_back(second); m.indices.push_back(second+1); m.indices.push_back(first+1);
        }
    }
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::cylinder(float radius, float height, int segments) {
    Mesh m;
    float h = height * 0.5f;
    // Bottom center
    m.vertices.push_back({{0,-h,0},{0,-1,0},{0.5f,0.5f},{1,1,1,1}});
    // Top center
    m.vertices.push_back({{0,h,0},{0,1,0},{0.5f,0.5f},{1,1,1,1}});

    for(int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * 3.14159265f / segments;
        float c = std::cos(angle), s = std::sin(angle);
        Vec3 p = {radius * c, -h, radius * s};
        m.vertices.push_back({p, {0,-1,0}, {c*0.5f+0.5f, s*0.5f+0.5f}, {1,1,1,1}});
        p.y = h;
        m.vertices.push_back({p, {0,1,0}, {c*0.5f+0.5f, s*0.5f+0.5f}, {1,1,1,1}});
        Vec3 side = {c, 0, s};
        m.vertices.push_back({{radius*c, -h, radius*s}, side, {(float)i/segments, 0}, {1,1,1,1}});
        m.vertices.push_back({{radius*c, h, radius*s}, side, {(float)i/segments, 1}, {1,1,1,1}});
    }

    int baseIdx = 2;
    int topIdx = 3;
    int sideBase = 4;
    for(int i = 0; i < segments; i++) {
        int next = (i + 1);
        // Bottom cap
        m.indices.push_back(0); m.indices.push_back(baseIdx + i*4); m.indices.push_back(baseIdx + next*4);
        // Top cap
        m.indices.push_back(1); m.indices.push_back(topIdx + next*4); m.indices.push_back(topIdx + i*4);
        // Side
        int s1 = sideBase + i*4, s2 = sideBase + next*4;
        int s3 = s1 + 2, s4 = s2 + 2;
        m.indices.push_back(s3); m.indices.push_back(s4); m.indices.push_back(s1+2);
        m.indices.push_back(s4); m.indices.push_back(s2+2); m.indices.push_back(s1+2);
    }
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::plane(float width, float depth, int subdivs) {
    Mesh m;
    float hw = width * 0.5f, hd = depth * 0.5f;
    for(int z = 0; z <= subdivs; z++) {
        for(int x = 0; x <= subdivs; x++) {
            float u = (float)x / subdivs, v = (float)z / subdivs;
            Vec3 p = {u * width - hw, 0, v * depth - hd};
            m.vertices.push_back({p, {0,1,0}, {u, v}, {1,1,1,1}});
        }
    }
    for(int z = 0; z < subdivs; z++) {
        for(int x = 0; x < subdivs; x++) {
            int i = z * (subdivs + 1) + x;
            m.indices.push_back(i); m.indices.push_back(i+subdivs+1); m.indices.push_back(i+1);
            m.indices.push_back(i+1); m.indices.push_back(i+subdivs+1); m.indices.push_back(i+subdivs+2);
        }
    }
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::noiseSphere(float radius, int segments, const NoiseGenerator& ng, float amplitude) {
    Mesh m = sphere(radius, segments);
    for(auto& v : m.vertices) {
        float n = ng.fbm(v.position.x * 0.5f, v.position.y * 0.5f, v.position.z * 0.5f, 3);
        float displacement = 1.0f + n * amplitude;
        v.position.x *= displacement;
        v.position.y *= displacement;
        v.position.z *= displacement;
    }
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::terrain(float width, float depth, int resolution, const NoiseGenerator& ng, float heightScale) {
    Mesh m = plane(width, depth, resolution);
    for(auto& v : m.vertices) {
        float h = ng.fbm(v.position.x * 0.05f, v.position.z * 0.05f, 6);
        float ridged = ng.ridged(v.position.x * 0.03f, v.position.z * 0.03f, 4);
        v.position.y = h * heightScale * 0.5f + ridged * heightScale * 0.5f;
    }
    // Recompute normals
    for(auto& v : m.vertices) v.normal = {0,0,0};
    for(size_t i = 0; i < m.indices.size(); i += 3) {
        auto& v0 = m.vertices[m.indices[i]];
        auto& v1 = m.vertices[m.indices[i+1]];
        auto& v2 = m.vertices[m.indices[i+2]];
        Vec3 e1 = {v1.position.x-v0.position.x, v1.position.y-v0.position.y, v1.position.z-v0.position.z};
        Vec3 e2 = {v2.position.x-v0.position.x, v2.position.y-v0.position.y, v2.position.z-v0.position.z};
        Vec3 n = {e1.y*e2.z - e1.z*e2.y, e1.z*e2.x - e1.x*e2.z, e1.x*e2.y - e1.y*e2.x};
        v0.normal.x += n.x; v0.normal.y += n.y; v0.normal.z += n.z;
        v1.normal.x += n.x; v1.normal.y += n.y; v1.normal.z += n.z;
        v2.normal.x += n.x; v2.normal.y += n.y; v2.normal.z += n.z;
    }
    for(auto& v : m.vertices) {
        float len = std::sqrt(v.normal.x*v.normal.x + v.normal.y*v.normal.y + v.normal.z*v.normal.z);
        if(len > 0) { v.normal.x/=len; v.normal.y/=len; v.normal.z/=len; }
    }
    m.computeBounds();
    return m;
}

Mesh ProceduralMesh::crystal(float size, int faces, const NoiseGenerator& ng) {
    Mesh m;
    // Generate random crystal points on sphere
    std::vector<Vec3> points;
    for(int i = 0; i < faces; i++) {
        float theta = ng.hash(i, 0) * 3.14159265f;
        float phi = ng.hash(i, 127) * 2.0f * 3.14159265f;
        float r = size * (0.7f + ng.hash(i, 255) * 0.6f);
        points.push_back({r*std::sin(theta)*std::cos(phi), r*std::cos(theta), r*std::sin(theta)*std::sin(phi)});
    }
    // Simple convex hull approximation - center to each point
    Vec3 center = {0,0,0};
    for(const auto& p : points) {
        center.x += p.x; center.y += p.y; center.z += p.z;
    }
    center.x /= points.size(); center.y /= points.size(); center.z /= points.size();

    m.vertices.push_back({center, {0,1,0}, {0.5f, 0.5f}, {1,1,1,1}});
    for(const auto& p : points) {
        Vec3 n = {p.x-center.x, p.y-center.y, p.z-center.z};
        float len = std::sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        n.x/=len; n.y/=len; n.z/=len;
        m.vertices.push_back({p, n, {0,0}, {1,1,1,1}});
    }
    for(size_t i = 1; i < points.size(); i++) {
        m.indices.push_back(0); m.indices.push_back(i); m.indices.push_back(i+1);
    }
    m.indices.push_back(0); m.indices.push_back((uint32_t)points.size()); m.indices.push_back(1);
    m.computeBounds();
    return m;
}

void ProceduralMesh::subdivide(Mesh& mesh) {
    std::vector<Vertex> newVerts = mesh.vertices;
    std::vector<uint32_t> newIdx;

    auto getMid = [&](uint32_t a, uint32_t b) -> uint32_t {
        Vertex v;
        v.position = lerp(mesh.vertices[a].position, mesh.vertices[b].position, 0.5f);
        v.normal = lerp(mesh.vertices[a].normal, mesh.vertices[b].normal, 0.5f);
        v.uv = {(mesh.vertices[a].uv.x + mesh.vertices[b].uv.x) * 0.5f,
                (mesh.vertices[a].uv.y + mesh.vertices[b].uv.y) * 0.5f};
        v.color = mesh.vertices[a].color;
        newVerts.push_back(v);
        return (uint32_t)newVerts.size() - 1;
    };

    for(size_t i = 0; i < mesh.indices.size(); i += 3) {
        uint32_t a = mesh.indices[i], b = mesh.indices[i+1], c = mesh.indices[i+2];
        uint32_t ab = getMid(a, b), bc = getMid(b, c), ca = getMid(c, a);
        newIdx.push_back(a); newIdx.push_back(ab); newIdx.push_back(ca);
        newIdx.push_back(ab); newIdx.push_back(b); newIdx.push_back(bc);
        newIdx.push_back(ca); newIdx.push_back(bc); newIdx.push_back(c);
        newIdx.push_back(ab); newIdx.push_back(bc); newIdx.push_back(ca);
    }
    mesh.vertices = std::move(newVerts);
    mesh.indices = std::move(newIdx);
    mesh.computeBounds();
}

} // namespace zv

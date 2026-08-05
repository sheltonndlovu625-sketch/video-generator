#pragma once
#include "scene.h"
#include "noise.h"
#include <string>

namespace zv {

// Procedural Human Generator
// Uses mathematical body proportions and noise for variation
class HumanGenerator {
public:
    struct Params {
        float height = 1.75f;      // meters
        float build = 0.5f;        // 0=slender, 1=stocky
        uint32_t seed = 0;
        int detail = 3;            // 1-5
        Vec3 skinTone{0.8f, 0.6f, 0.5f};
        Vec3 hairColor{0.2f, 0.15f, 0.1f};
        bool hasHair = true;
        bool isChild = false;
    };

    static Entity generate(const Params& params, const std::string& name);

private:
    static void generateHead(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateTorso(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateLimbs(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateHair(Entity& e, const Params& p, NoiseGenerator& ng);
};

// Procedural Animal Generator
class AnimalGenerator {
public:
    enum class Species {
        Quadruped,  // Generic 4-legged
        Bird,
        Fish,
        Reptile,
        Insect
    };

    struct Params {
        Species species = Species::Quadruped;
        float size = 1.0f;
        uint32_t seed = 0;
        int detail = 3;
        Vec3 primaryColor{0.5f, 0.4f, 0.3f};
        Vec3 secondaryColor{0.3f, 0.3f, 0.3f};
        bool hasPattern = true;
    };

    static Entity generate(const Params& params, const std::string& name);

private:
    static void generateQuadruped(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateBird(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateFish(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateReptile(Entity& e, const Params& p, NoiseGenerator& ng);
};

// Procedural Place/Environment Generator
class PlaceGenerator {
public:
    enum class Type {
        Forest,
        Mountain,
        Desert,
        Cave,
        FloatingIsland,
        CrystalCavern,
        AlienLandscape
    };

    struct Params {
        Type type = Type::Forest;
        uint32_t seed = 0;
        int detail = 3;
        float scale = 100.0f;
        Vec3 skyColor{0.4f, 0.6f, 0.9f};
        Vec3 groundColor{0.2f, 0.5f, 0.1f};
        float waterLevel = -5.0f;
        bool hasWater = true;
        bool hasVegetation = true;
    };

    static Entity generate(const Params& params, const std::string& name);
    static std::vector<Entity> generateEnvironment(const Params& params);

private:
    static void generateTerrain(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateVegetation(std::vector<Entity>& entities, const Params& p, NoiseGenerator& ng);
    static void generateWater(Entity& e, const Params& p, NoiseGenerator& ng);
    static void generateRocks(std::vector<Entity>& entities, const Params& p, NoiseGenerator& ng);
    static void generateCrystals(std::vector<Entity>& entities, const Params& p, NoiseGenerator& ng);
};

} // namespace zv

#pragma once

#include "Engine/SoftBody.hpp"

#include <cstdint>

struct SoftBody3 : Engine::SoftBody {
    SoftBody3(const glm::vec2 &center, float radius, float particleMass, float particleRadius, std::size_t numParticles);
};

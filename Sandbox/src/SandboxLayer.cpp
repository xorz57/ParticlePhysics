#include "Sandbox/SandboxLayer.hpp"
#include "Engine/Particle.hpp"
#include "Engine/Spring.hpp"
#include "Sandbox/DoubleSpringPendulum.hpp"
#include "Sandbox/SoftBody1.hpp"
#include "Sandbox/SoftBody2.hpp"
#include "Sandbox/SoftBody3.hpp"
#include "Sandbox/SpringPendulum.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui-SFML.h>

#include <implot.h>

SandboxLayer::SandboxLayer(std::string name) : Engine::Layer(std::move(name)) {}

void SandboxLayer::OnAttach() {
    mSoftBodies.emplace_back(std::make_unique<SoftBody1>(glm::vec2(100.0f, 100.0f), 50.0f, 2.0f, 4.0f));
    mSoftBodies.emplace_back(std::make_unique<SoftBody2>(glm::vec2(300.0f, 200.0f), 50.0f, 2.0f, 4.0f));
    mSoftBodies.emplace_back(std::make_unique<SoftBody3>(glm::vec2(500.0f, 300.0f), 50.0f, 2.0f, 4.0f, 8));
    mSoftBodies.emplace_back(std::make_unique<SpringPendulum>(glm::vec2(400.0f, 0.0f), 100.0f, 2.0f, 4.0f));
    mSoftBodies.emplace_back(std::make_unique<DoubleSpringPendulum>(glm::vec2(200.0f, 0.0f), 50.0f, 2.0f, 4.0f));
}

void SandboxLayer::OnDetach() {
    mSoftBodies.clear();
}

void SandboxLayer::OnFixedUpdate(float dt) {
    for (const std::unique_ptr<Engine::SoftBody> &softBody: mSoftBodies) {
        for (Engine::Spring &spring: softBody->springs) {
            const glm::vec2 dPosition = spring.mParticle2.position - spring.mParticle1.position;
            const glm::vec2 dVelocity = spring.mParticle2.velocity - spring.mParticle1.velocity;
            if (glm::length(dPosition) != 0) {
                const glm::vec2 force = spring.mSpringConstant * (glm::length(dPosition) - spring.mRestLength) * glm::normalize(dPosition) + spring.mDampingConstant * dVelocity;
                spring.mParticle1.force += force;
                spring.mParticle2.force -= force;
            }
        }
        for (Engine::Particle &particle: softBody->particles) {
            const glm::vec2 gravitationalForce = particle.mass * mGravitationalAcceleration;
            particle.force += gravitationalForce;
            if (!particle.pinned) {
                particle.HandleBoundaryCollisions();
                const glm::vec2 acceleration = particle.force / particle.mass;
                particle.velocity += acceleration * dt;
                particle.position += particle.velocity * dt;
            }
            particle.force = glm::vec2(0.0f, 0.0f);
        }
    }
}

void SandboxLayer::OnUpdate(float dt) {
    // ...
}

void SandboxLayer::OnLateUpdate(float dt) {
    // ...
}

void SandboxLayer::OnRender(sf::RenderWindow &window) {
    for (const std::unique_ptr<Engine::SoftBody> &softBody: mSoftBodies) {
        for (const Engine::Spring &spring: softBody->springs) {
            sf::VertexArray line(sf::Lines, 2);
            line[0].position.x = spring.mParticle1.position.x;
            line[0].position.y = spring.mParticle1.position.y;
            line[1].position.x = spring.mParticle2.position.x;
            line[1].position.y = spring.mParticle2.position.y;
            line[0].color = sf::Color(206, 208, 206);
            line[1].color = sf::Color(206, 208, 206);
            window.draw(line);
        }
        for (const Engine::Particle &particle: softBody->particles) {
            sf::CircleShape shape;
            shape.setRadius(particle.radius);
            shape.setOrigin(particle.radius, particle.radius);
            shape.setPosition(particle.position.x, particle.position.y);
            shape.setFillColor(sf::Color(230, 232, 230));
            window.draw(shape);
        }
    }
}

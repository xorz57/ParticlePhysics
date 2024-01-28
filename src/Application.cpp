#include "Application.hpp"
#include "Cloth.hpp"
#include "Particle.hpp"
#include "Spring.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui-SFML.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

Application::Application() {
    mWindow.setFramerateLimit(120u);

    unsigned int rows = 10u;
    unsigned int cols = 10u;
    float padding = 20.0f;
    glm::vec2 center = glm::vec2(mMode.width, mMode.height) * 0.5f;
    glm::vec2 position = center - glm::vec2(cols - 1, rows - 1) * padding * 0.5f;
    mSoftBodies.emplace_back(Cloth(position, rows, cols, padding));
}

void Application::FixedUpdate(const sf::Time &fixedDeltaTime) {
    const float dt = fixedDeltaTime.asSeconds();
    for (SoftBody &softBody: mSoftBodies) {
        for (Spring &spring: softBody.springs) {
            const glm::vec2 dPosition = spring.mParticle2.position - spring.mParticle1.position;
            const glm::vec2 dVelocity = spring.mParticle2.velocity - spring.mParticle1.velocity;
            if (glm::length(dPosition) != 0) {
                const glm::vec2 force = spring.mSpringConstant * (glm::length(dPosition) - spring.mRestLength) * glm::normalize(dPosition) + spring.mDampingConstant * dVelocity;
                spring.mParticle1.force += force;
                spring.mParticle2.force -= force;
            }
        }
        for (Particle &particle: softBody.particles) {
            const glm::vec2 gravitationalForce = particle.mass * mGravitationalAcceleration;
            particle.force += gravitationalForce;
            if (!particle.pinned) {
                const glm::vec2 acceleration = particle.force * (1.0f / particle.mass);
                particle.velocity += acceleration * dt;
                particle.position += particle.velocity * dt;
            }
            particle.force = glm::vec2(0.0f, 0.0f);
        }
    }
}

void Application::Run() {
    (void) ImGui::SFML::Init(mWindow);

    const sf::Time fixedDeltaTime = sf::seconds(1.0f / 64.0f);
    float timeScale = 5.0f;
    sf::Time accumulator = sf::Time::Zero;
    sf::Clock deltaClock;
    while (mWindow.isOpen()) {
        sf::Event event{};
        while (mWindow.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(mWindow, event);
            switch (event.type) {
                case sf::Event::Closed:
                    mWindow.close();
                    break;

                case sf::Event::Resized:
                    mView.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                    mWindow.setView(mView);
                    break;

                case sf::Event::MouseWheelScrolled:
                    if (event.mouseWheelScroll.delta > 0) {
                        mView.zoom(1/1.2f);
                    }
                    if (event.mouseWheelScroll.delta < 0) {
                        mView.zoom(1.2f);
                    }
                    mWindow.setView(mView);
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (size_t softBodyIndex = 0; softBodyIndex < mSoftBodies.size(); ++softBodyIndex) {
                            const SoftBody &softBody = mSoftBodies[softBodyIndex];
                            for (size_t particleIndex = 0; particleIndex < softBody.particles.size(); ++particleIndex) {
                                const Particle &particle = softBody.particles[particleIndex];
                                const sf::Vector2f worldMousePosition = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                                const float distance = glm::length(glm::vec2(worldMousePosition.x - particle.position.x, worldMousePosition.y - particle.position.y));
                                if (distance <= particle.radius) {
                                    mSelectedSoftBody = true;
                                    mSelectedParticle = true;
                                    mSelectedSoftBodyIndex = softBodyIndex;
                                    mSelectedParticleIndex = particleIndex;
                                    break;
                                }
                            }
                        }
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        mSelectedSoftBody = false;
                        mSelectedParticle = false;
                    }
                    break;

                default:
                    break;
            }
        }

        const sf::Time deltaTime = deltaClock.restart();
        const sf::Time scaledDeltaTime = timeScale * deltaTime;

        accumulator += scaledDeltaTime;
        while (accumulator > fixedDeltaTime) {
            FixedUpdate(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        if (mSelectedSoftBody && mSelectedParticle) {
            const sf::Vector2f worldMousePosition = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
            Particle &SelectedParticle = mSoftBodies[mSelectedSoftBodyIndex].particles[mSelectedParticleIndex];
            SelectedParticle.force = glm::vec2(0.0f, 0.0f);
            SelectedParticle.velocity = glm::vec2(0.0f, 0.0f);
            SelectedParticle.position = glm::vec2(worldMousePosition.x, worldMousePosition.y);
        }

        ImGui::SFML::Update(mWindow, deltaTime);

        mWindow.clear(sf::Color(25, 25, 25));

        for (const SoftBody &softBody: mSoftBodies) {
            for (const Spring &spring: softBody.springs) {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position.x = spring.mParticle1.position.x;
                line[0].position.y = spring.mParticle1.position.y;
                line[1].position.x = spring.mParticle2.position.x;
                line[1].position.y = spring.mParticle2.position.y;
                line[0].color = sf::Color(150, 150, 150);
                line[1].color = sf::Color(150, 150, 150);
                mWindow.draw(line);
            }
            for (const Particle &particle: softBody.particles) {
                sf::CircleShape shape;
                shape.setRadius(particle.radius);
                shape.setOrigin(particle.radius, particle.radius);
                shape.setPosition(particle.position.x, particle.position.y);
                if (mSelectedSoftBody && mSelectedParticle && &particle == &mSoftBodies[mSelectedSoftBodyIndex].particles[mSelectedParticleIndex]) {
                    shape.setFillColor(sf::Color(200, 0, 0));
                } else {
                    shape.setFillColor(sf::Color(200, 200, 200));
                }
                mWindow.draw(shape);
            }
        }

        ImGui::Begin("Settings");
        ImGui::DragFloat("Time Scale", &timeScale, 0.1f, 0.0f, 10.0f);
        ImGui::DragFloat2("Gravity", glm::value_ptr(mGravitationalAcceleration), 0.1f, -9.8f, 9.8f);
        ImGui::End();

        ImGui::Begin("Statistics");
        ImGui::Text("FPS               : %3.f", 1.0f / deltaTime.asSeconds());
        ImGui::Text("Tick Rate         : %3.f", 1.0f / fixedDeltaTime.asSeconds());
        ImGui::Text("Fixed Delta Time  : %3d ms", fixedDeltaTime.asMilliseconds());
        ImGui::Text("Delta Time        : %3d ms", deltaTime.asMilliseconds());
        ImGui::Text("Scaled Delta Time : %3d ms", scaledDeltaTime.asMilliseconds());
        ImGui::End();

        ImGui::SFML::Render(mWindow);

        mWindow.display();
    }

    ImGui::SFML::Shutdown();
}

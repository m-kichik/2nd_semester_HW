#include <array>

#include <SFML/Graphics.hpp>

#include "Functions.h"
#include "Molecule.hpp"
#include "Visualizer.h"

void Visualizer::updateField() {
    std::array <std::array<std::size_t, fieldWid>, fieldLen> fieldParticles{};

    for (auto x : fieldParticles)
        for (auto y : x)
            y = 0;

    for (auto particle : particles)
        fieldParticles[
                static_cast <std::size_t> (particle.get_x() / cellSize)][
                static_cast <std::size_t> (particle.get_y() / cellSize)] ++;

    fieldColor fc;
    for (auto i = 0u; i < fieldLen; ++i)
        for (auto j = 0u; j < fieldWid; ++j) {
            switch (fieldParticles[i][j]) {
                case 0:
                    field[i][j] = fc.c_0;
                    break;
                case 1:
                    field[i][j] = fc.c_1;
                    break;
                case 2:
                    field[i][j] = fc.c_2;
                    break;
                case 3:
                    field[i][j] = fc.c_3;
                    break;
                case 4:
                    field[i][j] = fc.c_4;
                    break;
                case 5:
                    field[i][j] = fc.c_5;
                    break;
                case 6:
                    field[i][j] = fc.c_6;
                    break;
                case 7:
                    field[i][j] = fc.c_7;
                    break;
                case 8:
                    field[i][j] = fc.c_8;
                    break;
                case 9:
                    field[i][j] = fc.c_9;
                    break;
                default:
                    field[i][j] = fc.c_10;
                    break;
            }
        }
}

void Visualizer::runGas() {
    double timer = 0.0;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                ended = true;
                for (auto &thread : threads)
                    thread.join();
            }
        }

        window.clear(sf::Color::White);

        double time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
        for (auto i = 0U; i < fieldLen; ++i)
            for (auto j = 0U; j < fieldWid; ++j) {
                rect.setPosition(static_cast <float> (/*cts::length +*/ i * cellSize), static_cast <float> (j * cellSize));
                rect.setFillColor(field[i][j]);
                window.draw(rect);
            }

        for (auto i = 0u; i < cts::numberParticles; ++i) {
            sf::CircleShape molecule(cts::moleculeDiameter);
            molecule.setPosition(particles[i].get_x(), particles[i].get_y());
            molecule.setFillColor(sf::Color::Black);
            window.draw(molecule);
        }

        if (timer > cts::delay) {
            //changeParticlesSpeed(particles);

            for (auto i = step * numThreads; i < std::size(particles); ++i)
                particles[i].moleculeRun();

            /*for (auto& p : particles)
                p.moleculeRun();*/

            updateField();

            timer = 0.0;
        }

        window.display();
    }
}


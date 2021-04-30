#ifndef CHRONO_VISUALIZER_H
#define CHRONO_VISUALIZER_H

#include <array>
#include <thread>

#include <SFML/Graphics.hpp>

#include "Functions.h"
#include "Molecule.hpp"

#define ever (;;)

class Visualizer {
private:
    struct fieldColor {
        sf::Color c_10{255, 111, 0};
        sf::Color c_9{255, 125, 25};
        sf::Color c_8 = {255, 140, 51};
        sf::Color c_7 = {255, 154, 76};
        sf::Color c_6 = {255, 169, 102};
        sf::Color c_5 = {255, 183, 127};
        sf::Color c_4 = {255, 197, 153};
        sf::Color c_3 = {255, 212, 178};
        sf::Color c_2 = {255, 226, 204};
        sf::Color c_1 = {255, 241, 229};
        sf::Color c_0 = {255, 255, 255};
    };

public:
    Visualizer() : window(sf::VideoMode(cts::length /** 2*/, cts::width), "Gas"),
                   particles(createParticles(numParticles)) {
        for (auto i = 0u; i < numThreads; ++i) {
            threads.emplace_back(&Visualizer::waitWork<decltype(std::begin(particles))>, this,
                                 std::next(std::begin(particles), i * step),
                                 std::next(std::begin(particles), (i + 1u) * step));
        }
    }

private:
    void updateField();

    template <typename Iterator>
    void runMolecules(Iterator begin, Iterator end) {
        for(auto it = begin; it != end; ++it)
            it -> moleculeRun();
    }

    template <typename Iterator>
    void waitWork(Iterator begin, Iterator end) {
        for ever {
            std::this_thread::sleep_for(std::chrono::milliseconds (20));

            runMolecules(begin, end);

            if (ended)
                break;
        }
    }

public:
    void runGas();

private:
    sf::RenderWindow window;
    static const std::size_t cellSize = 20;
    static const std::size_t fieldLen = cts::length / cellSize;
    static const std::size_t fieldWid = cts::width / cellSize;
    std::array <std::array<sf::Color, fieldWid>, fieldLen> field;
    bool ended = false;

private:
    std::size_t numParticles = cts::numberParticles;
    std::vector <Molecule> particles;

private:
    const std::size_t numThreads = std::thread::hardware_concurrency() - 1u;
    const std::size_t step = cts::numberParticles / (numThreads + 1u);
    std::vector <std::thread> threads;
};

#endif
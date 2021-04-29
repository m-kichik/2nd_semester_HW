#include <array>
#include <thread>

#include "Header.hpp"
#include <SFML/Graphics.hpp>

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
    /*sf::Color c_5{255, 111, 0};
    sf::Color c_4 = {255, 140, 51};
    sf::Color c_3 = {255, 169, 102};
    sf::Color c_2 = {255, 197, 153};
    sf::Color c_1 = {255, 226, 204};
    sf::Color c_0 = {255, 255, 255};*/
};

const auto cellSize = 40u;
const auto fieldLen = cts::length / cellSize;
const auto fieldWid = cts::width / cellSize;

void updateField(std::array <std::array<sf::Color, fieldWid>, fieldLen>& field,
                 std::vector <Molecule>& particles) {
    std::array <std::array<std::size_t, fieldWid>, fieldLen> numParticles{};

    for (auto x : numParticles)
        for (auto y : x)
            y = 0;

    for (auto particle : particles)
        numParticles[
                static_cast <std::size_t> (particle.get_x() / cellSize)][
                        static_cast <std::size_t> (particle.get_y() / cellSize)] ++;

    fieldColor fc;
    for (auto i = 0u; i < fieldLen; ++i)
        for (auto j = 0u; j < fieldWid; ++j) {
            if (numParticles[i][j] >= 10)
                field[i][j] = fc.c_10;
            else if (numParticles[i][j] == 9)
                field[i][j] = fc.c_9;
            else if (numParticles[i][j] == 8)
                field[i][j] = fc.c_8;
            else if (numParticles[i][j] == 7)
                field[i][j] = fc.c_7;
            else if (numParticles[i][j] == 6)
                field[i][j] = fc.c_6;
            else
            if (numParticles[i][j] == 5)
                field[i][j] = fc.c_5;
            else if (numParticles[i][j] == 4)
                field[i][j] = fc.c_4;
            else if (numParticles[i][j] == 3)
                field[i][j] = fc.c_3;
            else if (numParticles[i][j] == 2)
                field[i][j] = fc.c_2;
            else if (numParticles[i][j] == 1)
                field[i][j] = fc.c_1;
            else if (numParticles[i][j] == 0)
                field[i][j] = fc.c_0;
        }
}

template <typename Iterator>
void runMolecules(Iterator begin, Iterator end) {
    for(auto it = begin; it != end; ++it)
        it -> moleculeRun();
}

void runGas() {
    std::vector <Molecule> particles;

    particles.reserve(cts::numberParticles);
    for (auto i = 0u; i < cts::numberParticles; ++i)
        particles.push_back(create());

    std::array <std::array<sf::Color, fieldWid>, fieldLen> field;

    double timer = 0.0;
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(cts::length /** 2*/, cts::width), "Gas");

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
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

            const auto numThreads = std::thread::hardware_concurrency() - 1u;
            const auto step = cts::numberParticles / (numThreads + 1u);
            std::vector <std::thread> threads;

            for (auto i = 0u; i < numThreads; ++i) {
                threads.emplace_back(runMolecules<decltype(std::begin(particles))>,
                                     std::next(std::begin(particles), i * step),
                                     std::next(std::begin(particles), (i + 1u) * step));
            }

            for (auto i = step * numThreads; i < std::size(particles); ++i)
                particles[i].moleculeRun();

            for (auto &thread : threads)
                thread.join();

            /*for (auto& p : particles)
                p.moleculeRun();*/

            updateField(field, particles);

            timer = 0.0;
        }

        window.display();
    }
}

int main()
{
	runGas();

	return 0;
}
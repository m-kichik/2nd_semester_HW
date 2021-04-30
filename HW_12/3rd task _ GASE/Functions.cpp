#include <cmath>
#include <random>

#include "Molecule.hpp"
#include "Functions.h"

std::vector <Molecule> createParticles(std::size_t size) {
    std::vector <Molecule> particles;

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution urdL(0., static_cast <double> (cts::length + 1));
    std::uniform_real_distribution urdW(0., static_cast <double> (cts::width + 1));
    std::poisson_distribution pdS(cts::speed);
    std::bernoulli_distribution bd(0.5);

    for (auto i = 0u; i < size; ++i) {
        auto x = static_cast <float> (urdL(mt));
        auto y = static_cast <float> (urdW(mt));

        auto speed = static_cast <double> (pdS(mt));

        std::uniform_real_distribution urdS(- speed, speed);
        auto speed_x = static_cast <float> (urdS(mt));
        auto speed_y = static_cast <float> (sqrt(pow(cts::speed, 2) - pow(speed_x, 2)));

        speed_x /= 1;
        speed_y /= 1;

        particles.emplace_back(x, y, speed_x, speed_y);
    }

	return particles;
}

void changeParticlesSpeed(std::vector <Molecule>& particles) {
    for (auto i = 0u; i < particles.size() - 1u; ++i) {
        for (auto j = i + 1; j < particles.size(); ++j) {
            float x = particles[j].get_x() - particles[i].get_x();
            float y = particles[j].get_y() - particles[i].get_y();
            auto r = static_cast <float> (sqrt(pow(x, 2) + pow(y, 2)));

            float dt = cts::delay * cts::constTime;

            auto F = static_cast <float> (cts::multiplier * (12 * (pow(cts::moleculeDiameter, 12) / pow(r, 13))
                                                             - 6 * (pow(cts::moleculeDiameter, 6) / pow(r, 7))));

            float dv_x = (x / r) * F * dt;
            float dv_y = (y / r) * F * dt;

            float v_j_x = particles[j].getSpeed_x();
            float v_j_y = particles[j].getSpeed_y();

            particles[j].setSpeed_x(v_j_x + dv_x);
            particles[j].setSpeed_y(v_j_y + dv_y);

            float v_i_x = particles[i].getSpeed_x();
            float v_i_y = particles[i].getSpeed_y();

            particles[i].setSpeed_x(v_i_x - dv_x);
            particles[i].setSpeed_y(v_i_y - dv_y);
        }
    }
}
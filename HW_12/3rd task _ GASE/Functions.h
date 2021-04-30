#ifndef CHRONO_FUNCTIONS_H
#define CHRONO_FUNCTIONS_H

#include "Molecule.hpp"

std::vector <Molecule> createParticles(std::size_t size);

void changeParticlesSpeed(std::vector <Molecule>& particles);

#endif
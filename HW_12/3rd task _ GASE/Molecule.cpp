#include <cmath>
#include <iostream>
#include <random>

#include "Molecule.hpp"

std::ostream& operator << (std::ostream& stream, Molecule& m) {
    stream << "\nPoint (" << m.get_x() << ", " << m.get_y() << ") speed (" << m.getSpeed_x() << ", " << m.getSpeed_y() << ")\n";

    return stream;
}

void Molecule::moleculeRun() {
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::bernoulli_distribution bd(0.15);

    if (bd(mt))
        m_speed_x *= -1;

    if (bd(mt))
        m_speed_y *= -1;

    float dx = m_speed_x * cts::timeConstSpeed;
    float dy = m_speed_y * cts::timeConstSpeed;

	if (m_x + dx > 0 && m_x + dx < cts::length)
		m_x += dx;
	else {
		if (m_x + dx <= 0)
			m_x = std::abs(dx) - m_x;
		else
		if (m_x + dx >= cts::length)
			m_x = cts::length - std::abs(m_x + dx - cts::length);
	m_speed_x *= -1;
	}

	if (m_y + dy > 0 && m_y + dy < cts::width)
		m_y += dy;
	else {
		if (m_y + dy <= 0)
			m_y = std::abs(dy) - m_y;
		else
		if (m_y + dy >= cts::width)
			m_y = cts::width - std::abs(m_y + dy - cts::width);
	m_speed_y *= -1;
	}
}
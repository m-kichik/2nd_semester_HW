#include <cmath>
#include <random>

#include "Header.hpp"

Molecule create() {
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution urdL(0., static_cast <double> (cts::length + 1));
    std::uniform_real_distribution urdW(0., static_cast <double> (cts::width + 1));
    std::poisson_distribution pdS(cts::speed);
    std::bernoulli_distribution bd(0.5);

	auto x = static_cast <float> (urdL(mt));
	auto y = static_cast <float> (urdW(mt));

	auto speed = pdS(mt);

    std::uniform_real_distribution urdS(0., static_cast <double> (speed));
	auto speed_x = static_cast <float> (urdS(mt));
	int k1 = bd(mt);
	if (!k1)
		speed_x *= (-1);

	auto speed_y = static_cast <float> (sqrt(pow(cts::speed, 2) - pow(speed_x, 2)));
	int k2 = bd(mt);
	if (!k2)
		speed_y *= (-1);

	speed_x /= 1;
	speed_y /= 1;

	Molecule m(x, y, speed_x, speed_y);

	return m;
}

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
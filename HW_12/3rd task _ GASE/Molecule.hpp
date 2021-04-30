#ifndef CHRONO_MOLECULE_H
#define CHRONO_MOLECULE_H

// 1 пиксель = 0,1 нанометра

// средняя скорость молекул в газе = 400 м/с, у нас - 400 пикс/с = 40 нм/с, время в 10^10 затормозили
// (поправка на это в подсчёте силы - constTime)
// --- при timeConstSpeed = 0.02, соответственно смотрим на const_of_speed для перевода

// 0,3 нм = диаметр кислорода, у нас частицы в 10 раз больше (отн. масштаба), потому что мы можем!
#include <vector>

namespace cts { // consts
	const std::size_t length = 600;
	const std::size_t width = 800;

	const std::size_t numberParticles = 2500;

	const float moleculeDiameter = 3.0;

	const float delay = 0.02;

	const float constTime = 1e-10; // во сколько тормозим время в выражении для силы

	const float timeConstSpeed = 0.02;

	const double speed = 400.0;

	const float multiplier = 4 * 1.38e-23 * 120 * 1e9 * 1e0; // 4 * kб * Theta - коэфф перед скобкой в потенциале Леннарда-Джонсона,
															   // последний множитель после е выбирался по принципу "чтоб эффект видно было"
}

class Molecule {
public:
	Molecule(float x, float y, float s_x, float s_y)
		: m_x(x), m_y(y), m_speed_x(s_x), m_speed_y(s_y)
	{}

	~Molecule() = default;

public:
	friend std::ostream& operator << (std::ostream& stream, Molecule& m);
	auto get_x() const { return m_x; }
	auto get_y() const { return m_y; }
	auto getSpeed_x() const { return m_speed_x; }
	auto getSpeed_y() const { return m_speed_y; }
	void setSpeed_x(float s_x) { m_speed_x = s_x; }
	void setSpeed_y(float s_y) { m_speed_y = s_y; }

	void moleculeRun();

private:
	float m_x;
	float m_y;

	float m_speed_x;
	float m_speed_y;
};

#endif
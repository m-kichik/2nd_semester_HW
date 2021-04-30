#ifndef CHRONO_MOLECULE_H
#define CHRONO_MOLECULE_H

// 1 ������� = 0,1 ���������

// ������� �������� ������� � ���� = 400 �/�, � ��� - 400 ����/� = 40 ��/�, ����� � 10^10 �����������
// (�������� �� ��� � �������� ���� - constTime)
// --- ��� timeConstSpeed = 0.02, �������������� ������� �� const_of_speed ��� ��������

// 0,3 �� = ������� ���������, � ��� ������� � 10 ��� ������ (���. ��������), ������ ��� �� �����!
#include <vector>

namespace cts { // consts
	const std::size_t length = 600;
	const std::size_t width = 800;

	const std::size_t numberParticles = 2500;

	const float moleculeDiameter = 3.0;

	const float delay = 0.02;

	const float constTime = 1e-10; // �� ������� �������� ����� � ��������� ��� ����

	const float timeConstSpeed = 0.02;

	const double speed = 400.0;

	const float multiplier = 4 * 1.38e-23 * 120 * 1e9 * 1e0; // 4 * k� * Theta - ����� ����� ������� � ���������� ��������-��������,
															   // ��������� ��������� ����� � ��������� �� �������� "���� ������ ����� ����"
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
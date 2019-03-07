#pragma once

#include <glm/glm.hpp>

#define BOIDSIZE 100
#define QUARTERBOIDSIZE 0.25*BOIDSIZE
#define HALFBOIDSIZE 0.5*BOIDSIZE

using namespace glm;
static int boidId = 1;

class CBoid
{
public:
	CBoid() :m_ptPos(100.0, 100.0), m_velocity(1.0, 0.0), m_OrientationAngle(0.0), m_Id(boidId)
	{
	}
	CBoid(float x, float y) :m_ptPos(x, y), m_Id(boidId++)
	{
		float min = -3.0;
		float max = 3.0;
		m_velocity.x = static_cast <float> (min + (rand() % static_cast<int>(max - min + 1)));
		m_velocity.y = static_cast <float> (min + (rand() % static_cast<int>(max - min + 1)));
		m_OrientationAngle = glm::degrees(atan2(m_velocity.y, m_velocity.x));
	}
	CBoid(CBoid&& other) = default;
	CBoid(const CBoid& other) = default;
	~CBoid()
	{
	}
	vec2 GetPos() { return m_ptPos; }
	vec2 GetVelocity() { return m_velocity; }
	float GetOrientation() { return m_OrientationAngle; }
	int GetId() { return m_Id; }
	void UpdateLocation();
	void Seek(vec2 target);
	void Draw();

private:
	vec2 m_ptPos;
	vec2 m_velocity;
	float m_OrientationAngle;
	int m_Id;

};
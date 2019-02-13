#pragma once

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;

class CLine
{
public:
	CLine() :m_ptStart(0.0, 0.0), m_ptEnd(0.0, 0.0) {}
	CLine(vec2 pts, vec2 pte) :m_ptStart(pts), m_ptEnd(pte)
	{
		CalcLength();
	}	
	~CLine();

	void Update(vec2 mouseLocation);
	void Draw();
	void CalcLength();
	vec2 PointOnLineAtDistance(double distance);

	vec2 m_ptStart;
	vec2 m_ptEnd;
	float m_Length;
};


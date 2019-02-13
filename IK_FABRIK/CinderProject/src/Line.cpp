#include "Line.h"



CLine::~CLine()
{
}

void CLine::Draw()
{
	gl::color(0, 0, 1); // set color to blue
	gl::drawLine(m_ptStart, m_ptEnd);
}

void CLine::Update(vec2 mouseLocation)
{

}

void CLine::CalcLength()
{
	double xD = m_ptEnd.x - m_ptStart.x;
	double yD = m_ptEnd.y - m_ptStart.y;
	m_Length = sqrtf((xD*xD) + (yD*yD));
}

vec2 CLine::PointOnLineAtDistance(double distance)
{
	vec2 ptReturn;
	double t = distance / m_Length;
	//xt = (1 - t)*x0 + t*x1  where(x0, y0) is the start and (x1, y1) is the end
	//yt = (1 - t)*y0 + t*y1
	ptReturn.x = (1 - t)*m_ptStart.x + t*m_ptEnd.x;
	ptReturn.y = (1 - t)*m_ptStart.y + t*m_ptEnd.y;
	return ptReturn;
}
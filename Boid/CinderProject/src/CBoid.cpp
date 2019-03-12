#pragma once

#include <iostream>
#include <fstream>
#include "CBoid.h"
#include "cinder/gl/gl.h"


using namespace ci;
extern bool double_equals(double a, double b);
extern void Logtofile(std::string name, float value);

void CBoid::UpdateLocation()
{
	if (m_bSeekTarget)
		Seek();

	m_ptPos.x += m_velocity.x;
	m_ptPos.y += m_velocity.y;
	if (m_ptPos.x<0)
		m_ptPos.x += 800;
	if (m_ptPos.x>800)
		m_ptPos.x -= 800;
	if (m_ptPos.y<0)
		m_ptPos.y += 600;
	if (m_ptPos.y>600)
		m_ptPos.y -= 600;
}

void CBoid::SetSeekLocal(vec2 target)
{
	m_pLocaltarget = target;
	m_bSeekTarget = true;
}

void CBoid::Seek()
{
	if (!double_equals(m_pLocaltarget.x, m_ptPos.x) || !double_equals(m_pLocaltarget.y, m_ptPos.y))
	{
		vec2 targetDirection = glm::normalize(m_pLocaltarget - m_ptPos);
		float targetAngle = glm::degrees(atan2(targetDirection.y, targetDirection.x));		
		if (targetAngle < 0)
			targetAngle += 360.0f;

		Logtofile("target :", targetAngle);

		//target crossing 360.0 degree line either way. 
		//Adjust angles so that boid does not suddenly rotate other way.
		float angleAdjustment = 0.05f;
		if ((targetAngle > 0.0 && targetAngle < 90.0) && (m_OrientationAngle > 270.0 && m_OrientationAngle < 360.0))
			targetAngle += 360.0;
		else if ((targetAngle > 270.0 && targetAngle < 360.0) && (m_OrientationAngle > 0.0 && m_OrientationAngle < 90.0))
			targetAngle -= 360.0;
		
		float angleDiff = targetAngle - m_OrientationAngle;		
		m_OrientationAngle += angleAdjustment *(angleDiff);
		if (m_OrientationAngle > 360.)
			m_OrientationAngle -= 360.;
		if (m_OrientationAngle < 0.)
			m_OrientationAngle += 360.;
		///
		float len = (float)(m_velocity.length());
		m_velocity = glm::normalize(m_pLocaltarget - m_ptPos);
		m_velocity *= len;
	}
	else
	{
		//assert(!m_bSeekTarget);
		m_bSeekTarget = false;//stop seek target
	}
}

void CBoid::Draw()
{
	Logtofile("Orientation: ", m_OrientationAngle);
	vec2 pos = m_ptPos;
	vec2 pos1((pos.x + HALFBOIDSIZE * glm::cos(glm::radians(m_OrientationAngle))), (pos.y + HALFBOIDSIZE * glm::sin(glm::radians(m_OrientationAngle))));
	vec2 pos2((pos.x - HALFBOIDSIZE * glm::cos(glm::radians(m_OrientationAngle))), (pos.y - HALFBOIDSIZE * glm::sin(glm::radians(m_OrientationAngle))));
	vec2 dir = pos1 - pos;
	vec2 dirPerp1 = glm::normalize(vec2(dir.y, -dir.x));
	vec2 dirPerp2 = glm::normalize(vec2(-dir.y, dir.x));
	vec2 pos3(pos2 + vec2((QUARTERBOIDSIZE*dirPerp1.x), (QUARTERBOIDSIZE*dirPerp1.y)));
	vec2 pos4(pos2 + vec2((QUARTERBOIDSIZE*dirPerp2.x), (QUARTERBOIDSIZE*dirPerp2.y)));
	gl::drawLine(pos1, pos3);
	gl::drawLine(pos1, pos4);
	gl::drawLine(pos3, pos4);
}


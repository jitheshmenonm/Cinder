#include <vector>
#include "CBoid.h"
#include "CQuad.h"

CQuad::CQuad():m_bRoot(false),
			   m_ptTopLeft(0.0,0.0),
			   m_ptBottomRight(0.0,0.0),
			   m_ptrTopLeftChild(nullptr),
			   m_ptrTopRightChild(nullptr),
			   m_ptrBottomLeftChild(nullptr),
			   m_ptrBottomRightChild(nullptr)
{
}
CQuad::CQuad(vec2 ptTl, vec2 ptBr):m_bRoot(false),
								   m_ptTopLeft(ptTl),
								   m_ptBottomRight(ptBr),
								   m_ptrTopLeftChild(nullptr),
								   m_ptrTopRightChild(nullptr),
								   m_ptrBottomLeftChild(nullptr),
								   m_ptrBottomRightChild(nullptr)
{

}

CQuad::~CQuad()
{
}

void CQuad::SetRoot()
{
	m_bRoot = true;
}

bool CQuad::IsRoot() const
{
	return m_bRoot;
}

void CQuad::AddLeaf(CBoid* ptr)
{
	m_pLeaves.push_back(ptr);
}

int CQuad::NoOfLeaves() const
{
	return (int(m_pLeaves.size()));
}

bool CQuad::HasLeaves() const
{
	return !m_pLeaves.empty();
}

bool CQuad::IsPtInsideQuad(vec2 pt)
{
	if ((pt.x > m_ptTopLeft.x && pt.x < m_ptBottomRight.x) && (pt.y > m_ptTopLeft.y && pt.y < m_ptBottomRight.y))
		return true;

	return false;
}

void CQuad::SetTopLeft(CQuad* ptrTopLeftChild)
{
	m_ptrTopLeftChild = ptrTopLeftChild;
}
void CQuad::SetTopRight(CQuad* ptrTopRightChild)
{
	m_ptrTopRightChild = ptrTopRightChild;
}
void CQuad::SetBottomLeft(CQuad* ptrBottomLeftChild)
{
	m_ptrBottomLeftChild = ptrBottomLeftChild;
}
void CQuad::SetBottomRight(CQuad* ptrBottomRightChild)
{
	m_ptrBottomRightChild = ptrBottomRightChild;
}

bool CQuad::HasNoChildQuad()
{
	if (m_ptrTopLeftChild || m_ptrTopRightChild || m_ptrBottomLeftChild || m_ptrBottomRightChild)
		return false;
	return true;
}

bool CQuad::CanSubDivideFurther()
{
	float QuadWidth = (m_ptBottomRight.x - m_ptTopLeft.x);
	if ((QuadWidth / 2.0) < (2 * BOIDSIZE))
		return false;

	return true;
}

void CQuad::RemoveLeaves()
{
	m_pLeaves.clear();//no deletion here, as these are just reference pointers
}

CBoid* CQuad::GetTopLeaf()
{
	if (!m_pLeaves.empty())
		return m_pLeaves.back();

	return nullptr;
}

void CQuad::AddBoid(CBoid* ptrBoidToAdd)
{
	//This is called recursively till the boid become a leaf in one of the Quad nodes
	glm::vec2 pt = ptrBoidToAdd->GetPos();
	if (IsPtInsideQuad(pt))
	{
		float QuadWidth = (m_ptBottomRight.x - m_ptTopLeft.x);
		float QuadHeight = (m_ptBottomRight.x - m_ptTopLeft.x);

		//calculate the releavent points for the sub quads 		
		glm::vec2 middlePt ((m_ptTopLeft.x + (QuadWidth / 2.0)), (m_ptTopLeft.y + (QuadHeight / 2.0)));
		glm::vec2 middleUpPt ((m_ptTopLeft.x + (QuadWidth / 2.0)), m_ptTopLeft.y);
		glm::vec2 middleDownPt ((m_ptTopLeft.x + (QuadWidth / 2.0)), (m_ptTopLeft.y + QuadHeight));
		glm::vec2 middleLeftPt (m_ptTopLeft.x, (m_ptTopLeft.y + (QuadHeight / 2.0)));
		glm::vec2 middleRightPt ((m_ptTopLeft.x + QuadWidth), (m_ptTopLeft.y + (QuadHeight / 2.0)));

		//top Left sub Quad- uses m_ptTopLeft and middlePt
		if ((pt.x > m_ptTopLeft.x && pt.x < middlePt.x) && (pt.y > m_ptTopLeft.y && pt.y < middlePt.y))
		{
			if (m_ptrTopLeftChild)
			{
				if (m_ptrTopLeftChild->HasLeaves() && m_ptrTopLeftChild->CanSubDivideFurther())
				{
					assert(m_ptrTopLeftChild->NoOfLeaves() == 1);
					
					//move the current leaf downstream by splitting further 
					m_ptrTopLeftChild->AddBoid(m_ptrTopLeftChild->GetTopLeaf());//add leaf to sub tree
					m_ptrTopLeftChild->RemoveLeaves();//reset current leaves					

					//now add the current one as leaf
					m_ptrTopLeftChild->AddBoid(ptrBoidToAdd);
				}
				else//cant sub divide further
					m_ptrTopLeftChild->AddLeaf(ptrBoidToAdd);//add leaf to sub tree				
			}
			else
			{
				m_ptrTopLeftChild = new CQuad(m_ptTopLeft, middlePt);
				m_ptrTopLeftChild->AddLeaf(ptrBoidToAdd);
			}
		}
		//top Right sub Quad- uses  middleUpPt and middleRightPt
		if ((pt.x > middleUpPt.x && pt.x < middleRightPt.x) && (pt.y > middleUpPt.y && pt.y < middleRightPt.y))
		{
			if (m_ptrTopRightChild)
			{
			}
			else
			{
				m_ptrTopRightChild = new CQuad(middleUpPt, middleRightPt);
				m_ptrTopRightChild->AddLeaf(ptrBoidToAdd);
			}
		}
		//bottom Left  sub Quad- uses middleLeftPt and middleDownPt
		if ((pt.x > middleLeftPt.x && pt.x < middleDownPt.x) && (pt.y > middleLeftPt.y && pt.y < middleDownPt.y))
		{
			if (m_ptrBottomLeftChild)
			{
			}
			else
			{
				m_ptrBottomLeftChild = new CQuad(middleLeftPt, middleDownPt);
				m_ptrBottomLeftChild->AddLeaf(ptrBoidToAdd);
			}
		}
		//bottom Right sub Quad- uses middlePt and m_ptBottomRight
		if ((pt.x > middlePt.x && pt.x < m_ptBottomRight.x) && (pt.y > middlePt.y && pt.y < m_ptBottomRight.y))
		{
			if (m_ptrBottomRightChild)
			{
			}
			else
			{
				m_ptrBottomRightChild = new CQuad(middlePt, m_ptBottomRight);
				m_ptrBottomRightChild->AddLeaf(ptrBoidToAdd);
			}
		}

	}
}

void DestroyQuadTree(CBoid* ptrRoot)
{

}

void UpdateQuadTree(CQuad * ptrRoot, std::vector<CBoid>& boids)
{
	if (ptrRoot)
	{
		for (CBoid& b : boids)
			ptrRoot->AddBoid(&b);
	}
}
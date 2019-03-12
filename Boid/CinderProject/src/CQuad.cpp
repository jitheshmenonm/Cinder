#include <vector>
#include <algorithm>
#include "CBoid.h"
#include "CQuad.h"
#include "cinder/gl/gl.h"


using namespace ci;

CQuad::CQuad() :
	m_ptTopLeft(0.0, 0.0),
	m_ptBottomRight(0.0, 0.0),
	m_ptrTopLeftChild(nullptr),
	m_ptrTopRightChild(nullptr),
	m_ptrBottomLeftChild(nullptr),
	m_ptrBottomRightChild(nullptr),
	m_pPtrParent(nullptr)
{
}
CQuad::CQuad(vec2 ptTl, vec2 ptBr) :
	m_ptTopLeft(ptTl),
	m_ptBottomRight(ptBr),
	m_ptrTopLeftChild(nullptr),
	m_ptrTopRightChild(nullptr),
	m_ptrBottomLeftChild(nullptr),
	m_ptrBottomRightChild(nullptr)
{

}
void CQuad::SetParent(CQuad* pParent)
{
	m_pPtrParent = pParent;
}

CQuad* CQuad::GetParent()
{
	return m_pPtrParent;
}

CQuad::~CQuad()
{

}

void CQuad::DestroyChildQuads()
{
	if (m_ptrTopLeftChild)
	{
		m_ptrTopLeftChild->DestroyChildQuads();
		delete m_ptrTopLeftChild;
		m_ptrTopLeftChild = nullptr;
	}
	if (m_ptrTopRightChild)
	{
		m_ptrTopRightChild->DestroyChildQuads();
		delete m_ptrTopRightChild;
		m_ptrTopRightChild = nullptr;
	}
	if (m_ptrBottomLeftChild)
	{
		m_ptrBottomLeftChild->DestroyChildQuads();
		delete m_ptrBottomLeftChild;
		m_ptrBottomLeftChild = nullptr;
	}
	if (m_ptrBottomRightChild)
	{
		m_ptrBottomRightChild->DestroyChildQuads();
		delete m_ptrBottomRightChild;
		m_ptrBottomRightChild = nullptr;
	}
}

void CQuad::SetRoot()
{
	m_pPtrParent = nullptr;
}

bool CQuad::IsRoot() const
{
	return (!m_pPtrParent) ? true : false;
}

void CQuad::AddLeaf(CBoid* ptr)
{
	for (auto p : m_pLeaves)
	{
		if (p->GetId() == ptr->GetId())
		{
			bool p = false;
			assert(p);
		}
	}
	ptr->SetQuad(this);
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
	if ((pt.x >= m_ptTopLeft.x && pt.x <= m_ptBottomRight.x) && (pt.y >= m_ptTopLeft.y && pt.y <= m_ptBottomRight.y))
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

void CQuad::Draw()
{
	vec2 ptTopRight(m_ptTopLeft.x, m_ptBottomRight.y);
	vec2 ptBottomLeft(m_ptBottomRight.x, m_ptTopLeft.y);

	gl::drawLine(m_ptTopLeft, ptTopRight);
	gl::drawLine(ptTopRight, m_ptBottomRight);
	gl::drawLine(m_ptBottomRight, ptBottomLeft);
	gl::drawLine(ptBottomLeft, m_ptTopLeft);

	if (m_ptrTopLeftChild)
		m_ptrTopLeftChild->Draw();
	if (m_ptrTopRightChild)
		m_ptrTopRightChild->Draw();
	if (m_ptrBottomLeftChild)
		m_ptrBottomLeftChild->Draw();
	if (m_ptrBottomRightChild)
		m_ptrBottomRightChild->Draw();
}

bool CQuad::HasLeafWithId(int nId)
{
	auto it = std::find_if(m_pLeaves.begin(), m_pLeaves.end(),
		[&](CBoid* p) { return p->GetId() == nId; });
	if (it != m_pLeaves.end())
		return true;

	return false;
}

bool CQuad::AddBoid(CBoid* ptrBoidToAdd)
{
	bool returnVal = false;
	//This is called recursively till the boid become a leaf in one of the Quad nodes
	glm::vec2 pt = ptrBoidToAdd->GetPos();
	if (IsPtInsideQuad(pt))
	{
		float QuadWidth = (m_ptBottomRight.x - m_ptTopLeft.x);
		float QuadHeight = (m_ptBottomRight.y - m_ptTopLeft.y);

		//calculate the releavent points for the sub quads 		
		glm::vec2 middlePt((m_ptTopLeft.x + (QuadWidth / 2.0)), (m_ptTopLeft.y + (QuadHeight / 2.0)));
		glm::vec2 middleUpPt((m_ptTopLeft.x + (QuadWidth / 2.0)), m_ptTopLeft.y);
		glm::vec2 middleDownPt((m_ptTopLeft.x + (QuadWidth / 2.0)), (m_ptTopLeft.y + QuadHeight));
		glm::vec2 middleLeftPt(m_ptTopLeft.x, (m_ptTopLeft.y + (QuadHeight / 2.0)));
		glm::vec2 middleRightPt((m_ptTopLeft.x + QuadWidth), (m_ptTopLeft.y + (QuadHeight / 2.0)));


		auto funcAddLeafRecursively = [&returnVal](CQuad*& pQuad, glm::vec2 tlPt, glm::vec2 brPt, CBoid* ptrBoid) mutable
		{
			if (!pQuad)
				pQuad = new CQuad(tlPt, brPt);//Use object pools so as to allocate memory in chunk firsthand:TODO

			if (pQuad->HasLeaves())
			{
				if (pQuad->CanSubDivideFurther())
				{
					//Any Quad should not have more than one leaf if it can subdivide further
					assert(pQuad->NoOfLeaves() == 1);

					//move the current leaf downstream by splitting further 
					pQuad->AddBoid(pQuad->GetTopLeaf());///RECURSION..........
					pQuad->RemoveLeaves();//reset current leaves					

										  //now add the current one as leaf
					returnVal = pQuad->AddBoid(ptrBoid);///RECURSION..........
				}
				else
				{
					if (!pQuad->HasLeafWithId(ptrBoid->GetId()))//cant sub divide further
					{
						pQuad->AddLeaf(ptrBoid);//add leaf to sub tree				
						returnVal = true;
					}
				}
			}
			else
			{
				pQuad->AddLeaf(ptrBoid);//add leaf to sub tree				
				returnVal = true;
			}
		};

		//top Left sub Quad- uses m_ptTopLeft and middlePt
		if ((pt.x >= m_ptTopLeft.x && pt.x <= middlePt.x) && (pt.y >= m_ptTopLeft.y && pt.y <= middlePt.y))
		{
			funcAddLeafRecursively(m_ptrTopLeftChild, m_ptTopLeft, middlePt, ptrBoidToAdd);
			m_ptrTopLeftChild->SetParent(this);
		}
		//top Right sub Quad- uses  middleUpPt and middleRightPt
		if (!returnVal && (pt.x >= middleUpPt.x && pt.x <= middleRightPt.x) && (pt.y >= middleUpPt.y && pt.y <= middleRightPt.y))
		{
			funcAddLeafRecursively(m_ptrTopRightChild, middleUpPt, middleRightPt, ptrBoidToAdd);
			m_ptrTopRightChild->SetParent(this);
		}
		//bottom Left  sub Quad- uses middleLeftPt and middleDownPt
		if (!returnVal && (pt.x >= middleLeftPt.x && pt.x <= middleDownPt.x) && (pt.y >= middleLeftPt.y && pt.y <= middleDownPt.y))
		{
			funcAddLeafRecursively(m_ptrBottomLeftChild, middleLeftPt, middleDownPt, ptrBoidToAdd);
			m_ptrBottomLeftChild->SetParent(this);
		}
		//bottom Right sub Quad- uses middlePt and m_ptBottomRight
		if (!returnVal && (pt.x >= middlePt.x && pt.x <= m_ptBottomRight.x) && (pt.y >= middlePt.y && pt.y <= m_ptBottomRight.y))
		{
			funcAddLeafRecursively(m_ptrBottomRightChild, middlePt, m_ptBottomRight, ptrBoidToAdd);
			m_ptrBottomRightChild->SetParent(this);
		}
	}
	else
	{
		if (this->GetParent())
			returnVal = this->GetParent()->AddBoid(ptrBoidToAdd);
	}
	return returnVal;
}

bool CQuad::UpdateBoid(CBoid* ptrBoidToUpdate)
{
	bool returnVal = false;
	if (!IsPtInsideQuad(ptrBoidToUpdate->GetPos()))
	{
		//find the boid traversing the tree and see if it still belongs to the quad
		//or look up in the tree to see if it belongs to that. continue until root(in case boid gets outside window)
		auto it = std::find_if(m_pLeaves.begin(), m_pLeaves.end(),
			[&](CBoid* p) { return p->GetId() == ptrBoidToUpdate->GetId(); });
		if (it != m_pLeaves.end())
		{
			ptrBoidToUpdate->SetQuad(nullptr);//reset quad ptr
			m_pLeaves.erase(it);//remove leaf from this quad
		}
		if (this->GetParent())//look up in the parent if it has parent
			returnVal = this->GetParent()->AddBoid(ptrBoidToUpdate);//Add to hopefully another sibling(hopefully)
	}
	else
		returnVal = true;//inside same quad still

	return returnVal;
}

std::vector<int> collidedIDs;
void CQuad::CheckForCollisions()
{
	//starts from root
	if (!HasLeaves())
	{
		//look in children for this boid-> recursive calls made
		if (m_ptrTopLeftChild)
			m_ptrTopLeftChild->CheckForCollisions();
		if (m_ptrTopRightChild)
			m_ptrTopRightChild->CheckForCollisions();
		if (m_ptrBottomLeftChild)
			m_ptrBottomLeftChild->CheckForCollisions();
		if (m_ptrBottomRightChild)
			m_ptrBottomRightChild->CheckForCollisions();
	}
	else
	{
		if (NoOfLeaves() > 1)
		{
			/*To determine which side of the line from A = (x1, y1) to B = (x2, y2) a point P = (x, y) falls on you need to compute the value
			d = (x-x1)(y2-y1)-(y-y1)(x2-x1)
			If d<0 then the point lies on one side of the line, and if d>0 then it lies on the other side.If d = 0 then the point lies exactly line.
			To see whether points on the left side of the line are those with positive or negative values compute the value for d for a point you know
			is to the left of the line, such as(x1?1, y1) and then compare the sign with the point you are interested in.*/

			bool p = false;
			//assert(p);
			for (auto pB : m_pLeaves)
				collidedIDs.push_back(pB->GetId());
		}
	}
}


void DestroyQuadTree(CQuad* ptrRoot)
{
	ptrRoot->DestroyChildQuads();
}

void UpdateQuadTree(CQuad * ptrRoot, std::vector<CBoid>& boids)
{
	if (ptrRoot)
	{
		for (CBoid& b : boids)
		{
			CQuad* pQuad = b.GetQuad();
			bool returnVal = pQuad->UpdateBoid(&b);
			assert(returnVal);			
		}
	}
}


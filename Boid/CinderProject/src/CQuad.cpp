#include <vector>
#include "CBoid.h"
#include "CQuad.h"

CQuad::CQuad():m_bRoot(false),
			   m_ptrLeaf(nullptr),
			   m_ptTopLeft(0.0,0.0),
			   m_ptBottomRight(0.0,0.0),
			   m_ptrTopLeftChild(nullptr),
			   m_ptrTopRightChild(nullptr),
			   m_ptrBottomLeftChild(nullptr),
			   m_ptrBottomRightChild(nullptr)
{

}
CQuad::CQuad(vec2 ptTl, vec2 ptBr):m_bRoot(false),
								   m_ptrLeaf(nullptr),
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

void CQuad::SetLeaf(CBoid* ptr)
{
	m_ptrLeaf = ptr;
}

CBoid* CQuad::GetLeaf() const
{
	return m_ptrLeaf;
}

bool CQuad::IsPtInsideQuad(vec2 pt)
{
	if ((pt.x > m_ptTopLeft.x && pt.x < m_ptBottomRight.x) &&
		(pt.y > m_ptTopLeft.y && pt.y < m_ptBottomRight.y))
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

void BuildQuadTree(CBoid* ptrRoot)
{


}

void DestroyQuadTree(CBoid* ptrRoot)
{

}

void UpdateQuadTree(std::vector<CBoid>& boids)
{

}
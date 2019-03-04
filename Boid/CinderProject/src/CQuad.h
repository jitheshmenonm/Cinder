#pragma once

#include <glm/glm.hpp>
#include "CBoid.h"

using namespace glm;

class CQuad
{
public:
	CQuad();
	CQuad(vec2 ptTl, vec2 ptBr);

	~CQuad();
	bool IsPtInsideQuad(vec2 pt);
	void SetRoot();
	bool IsRoot() const;
	void AddLeaf(CBoid* ptr);
	int NoOfLeaves() const;
	bool HasLeaves() const;
	void RemoveLeaves();
	CBoid* GetTopLeaf();

	void SetTopLeft(CQuad* ptrTopLeftChild);
	void SetTopRight(CQuad* ptrTopRightChild);
	void SetBottomLeft(CQuad* ptrBottomLeftChild);
	void SetBottomRight(CQuad* ptrBottomRightChild);

	bool HasNoChildQuad();

	void AddBoid(CBoid* ptrBoidToAdd);
	bool CanSubDivideFurther();

private:
	vec2 m_ptTopLeft;
	vec2 m_ptBottomRight;

	CQuad* m_ptrTopLeftChild;
	CQuad* m_ptrTopRightChild;
	CQuad* m_ptrBottomLeftChild;
	CQuad* m_ptrBottomRightChild;
	
	bool m_bRoot;
	std::vector<CBoid*> m_pLeaves;//convert to shared ptr!!!!!!!!
};


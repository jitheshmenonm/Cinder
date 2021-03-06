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
	void AddLeaf(CBoid* ptr);//Simply add this boid to leaves
	int NoOfLeaves() const;
	bool HasLeaves() const;
	void RemoveLeaves();
	CBoid* GetTopLeaf();
	void SetParent(CQuad* pParent);
	CQuad* GetParent();
	void CheckForCollisions();
	void Draw();

	void SetTopLeft(CQuad* ptrTopLeftChild);
	void SetTopRight(CQuad* ptrTopRightChild);
	void SetBottomLeft(CQuad* ptrBottomLeftChild);
	void SetBottomRight(CQuad* ptrBottomRightChild);

	bool HasNoChildQuad();

	bool AddBoid(CBoid* ptrBoidToAdd);//This can be recursive and ends up calling AddLeaf
	bool UpdateBoid(CBoid* ptrBoidToUpdate);
	bool CanSubDivideFurther();
	bool HasLeafWithId(int nId);

	void DestroyChildQuads();

private:
	vec2 m_ptTopLeft;
	vec2 m_ptBottomRight;

	CQuad* m_ptrTopLeftChild;
	CQuad* m_ptrTopRightChild;
	CQuad* m_ptrBottomLeftChild;
	CQuad* m_ptrBottomRightChild;

	CQuad* m_pPtrParent;//so as to traverse up the tree

	std::vector<CBoid*> m_pLeaves;//convert to shared ptr:TODO
};
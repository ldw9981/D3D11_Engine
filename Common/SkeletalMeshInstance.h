#pragma once
#include "D3DRenderManager.h"

class SkeletalMeshResource;
class Material;
class Node;
struct CB_MatrixPalette;
struct SkeletonResource;
class SkeletalMeshInstance : public IRenderable
{
public:
	SkeletalMeshInstance();
	~SkeletalMeshInstance();
public:

	SkeletalMeshResource* m_pMeshResource;
	Material* m_pMaterial = nullptr;
	std::vector<Math::Matrix*>		m_BoneReferences;	// 본에 해당되는 노드MATRIX의 포인터를 저장한다.
	Math::Matrix* m_pNodeWorldTransform = nullptr;

	void Create(SkeletalMeshResource* pMeshResource, SkeletonResource* skeleton, Node* pRootNode, Material* pMaterial);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, SkeletonResource* skeleton);


	virtual void Render(ID3D11DeviceContext* deviceContext);
};
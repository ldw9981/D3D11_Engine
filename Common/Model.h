#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Node.h"
#include "Skeleton.h"

class Model: public Node
{
public:
	Model() {}
	~Model() {}
	
	// 공유 가능한 데이터
	std::vector<Mesh> m_Meshes;	
	std::vector<shared_ptr<Material>> m_Materials;
	std::vector<shared_ptr<Animation>> m_Animations;

	shared_ptr<Skeleton> m_pSkeleton = nullptr;
	
	// 인스턴스 데이터
	float m_AnimationProressTime = 0.0f;
    

	bool ReadFile(ID3D11Device* device,const char* filePath);
		
	Material* GetMaterial(UINT index);

	void Update(float deltaTime);	

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
};



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
	
	// ���� ������ ������
	std::vector<Mesh> m_Meshes;	
	std::vector<Material> m_Materials;
	std::vector<Animation> m_Animations;	
	Skeleton m_Skeleton;
	
	// �ν��Ͻ� ������
	float m_AnimationProressTime = 0.0f;
    

	bool ReadFile(ID3D11Device* device,const char* filePath);
		
	Material* GetMaterial(UINT index);

	void Update(float deltaTime);	

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
};



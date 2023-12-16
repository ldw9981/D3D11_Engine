#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Helper.h"
#include "Skeleton.h"


Node::~Node()
{
	LOG_MESSAGEA("~Node() %s", m_Name.c_str());	
}

void Node::CreateHierachy(Model* model,aiNode* node)
{
	m_Name = node->mName.C_Str();	
	LOG_MESSAGEA(m_Name.c_str());

	// d3d�� �� �켱(column-major) assimp�� ����� �� �켱(row-major)  ����̴�.
	m_Local = Math::Matrix(&node->mTransformation.a1).Transpose();
		
	m_MeshIndices.resize(node->mNumMeshes);
	UINT meshIndex=0;
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{	
		meshIndex = node->mMeshes[i];
		m_MeshIndices[i] = meshIndex;

		// Mesh�� Node�� WorldMatrix�� �����Ѵ�.
		model->m_Meshes[meshIndex].m_pNodeWorldTransform = &m_World;
	}

	UINT numChild = node->mNumChildren;	
	m_Children.resize(numChild);
	for (UINT i = 0; i < numChild; ++i)
	{
		m_Children[i].m_pParent = this;
		m_Children[i].CreateHierachy(model,node->mChildren[i]);
	}
}

void Node::CreateHierachy(Skeleton* skeleton)
{
	UINT count = skeleton->GetBoneCount();

	Bone* pBone = skeleton->GetBone(0);
	m_Name = pBone->Name;
	m_Children.reserve(pBone->NumChildren);	
		
	for (UINT i = 1; i < count; i++)
	{
		Bone* pBone = skeleton->GetBone(i);
		assert(pBone != nullptr);
		assert(pBone->ParentBoneIndex != -1);

		Node* pParentNode = FindNode(skeleton->GetBoneName(pBone->ParentBoneIndex));
		assert(pParentNode != nullptr);

		auto& node = pParentNode->m_Children.emplace_back();
		node.m_Name = pBone->Name;
		node.m_Local = pBone->RelativeTransform;
		node.m_Children.reserve(pBone->NumChildren);
		node.m_pParent = pParentNode;
	}
}

void Node::UpdateAnimation(float progressTime)
{
	// ����� �ִϸ��̼��� �ִٸ� �ִϸ��̼��� ������Ʈ�Ѵ�.
	if (m_pNodeAnimation != nullptr)
	{
		Math::Vector3 position,scaling;
		Math::Quaternion rotation;
		m_pNodeAnimation->Evaluate(progressTime, position,rotation,scaling);
		m_Local = Math::Matrix::CreateScale(scaling) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);	
	}

	// �θ� ��尡 �ִٸ� �θ� ����� WorldMatrix�� ���ؼ� �ڽ��� WorldMatrix�� �����.
	if (m_pParent != nullptr)
		m_World = m_Local * m_pParent->m_World;
	else
		m_World = m_Local;
	
	for (auto& child : m_Children)
	{	// �ڽ� ������ Update()�� ȣ���Ѵ�.
		child.UpdateAnimation(progressTime);
	}
}

Node* Node::FindNode(const std::string& name)
{
	if (m_Name == name)
		return this;

	for (auto& child : m_Children)
	{
		Node* found = child.FindNode(name);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}


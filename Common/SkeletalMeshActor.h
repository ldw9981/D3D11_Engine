#pragma once
#include "Actor.h"
/*
	�ִϸ��̼��� �ִ� �⺻ ��� ������Ʈ�� �ǹ��ϴ� ����
	Box ������ �ڽ� ������ �浹ó���� �Ѵ�.
*/
class SkeletalMeshComponent;
class BoxComponent;
class SkeletalMeshActor: public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor() {};

	BoxComponent* m_pBoxComponent = nullptr;
	SkeletalMeshComponent* m_pSkeletalMeshComponent = nullptr;

	std::string m_SceneFilePath;	// BeginPlay���� �ε�
	virtual void OnBeginPlay();
};


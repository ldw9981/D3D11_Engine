#pragma once
#include "Actor.h"

/*
	�⺻ ��� ������Ʈ�� �ǹ��ϴ� ����
	Block ������ �ڽ� ������ �浹ó���� �Ѵ�.
*/
class BoxComponent;
class StaticMeshComponent;
class StaticMeshActor :
    public Actor
{
public:
	StaticMeshActor();
	~StaticMeshActor();


	BoxComponent* m_pBoxComponent = nullptr;
	StaticMeshComponent* m_pStaticMeshComponent=nullptr;

	std::string m_SceneFilePath;
	virtual void OnBeginPlay();
};


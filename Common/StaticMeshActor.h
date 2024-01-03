#pragma once
#include "Actor.h"

/*
	기본 배경 오브젝트를 의미하는 액터
	Block 설정한 박스 형태의 충돌처리를 한다.
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


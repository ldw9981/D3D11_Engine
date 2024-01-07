#pragma once
#include "Actor.h"
/*
	애니메이션이 있는 기본 배경 오브젝트를 의미하는 액터
	Box 설정한 박스 형태의 충돌처리를 한다.
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

	
	virtual void OnBeginPlay();
};


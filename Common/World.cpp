#include "pch.h"
#include "World.h"
#include "Actor.h"
#include "D3DRenderManager.h"

World::World()
{
	
}

World::~World()
{
}


void World::DestroyGameObject(Actor* pObject)
{
	pObject->OnEndPlay();
	m_actors.remove_if([pObject](std::shared_ptr<Actor> pActor)	
		{
		return pActor.get() == pObject;		// 일단 만들고 너무 느리면 iterator로 바꾸자
	});
}

void World::Update(float DeltaTime)
{
	for (auto& actor : m_actors)
	{
		actor->Update(DeltaTime);
	}
}

void World::OnBeginPlay()
{	
	for (auto& actor : m_actors)
	{
		actor->OnBeginPlay();
	}
	m_bIsPlaying = true;
}

void World::OnEndPlay()
{
	m_bIsPlaying = false;
	for (auto& actor : m_actors)
	{
		actor->OnEndPlay();
	}
	m_actors.clear();
}

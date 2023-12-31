#include "pch.h"
#include "World.h"
#include "Actor.h"
#include "D3DRenderManager.h"

World::World()
{
	
}

World::World(std::string Name)
	:m_Name(Name)
{

}

World::~World()
{
}


void World::SetWorldEvent(IWorldEvent* pWorldEvent)
{
	m_pWorldEvent = pWorldEvent;
}

void World::DestroyGameObject(Actor* pObject)
{
	pObject->OnEndPlay();	
	m_Actors.erase(pObject->m_iteratorInWorld);
	if (m_pWorldEvent)
		m_pWorldEvent->OnEndPlay(this);
}

void World::Update(float DeltaTime)
{
	for (auto& actor : m_Actors)
	{
		actor->Update(DeltaTime);
	}
}

void World::OnBeginPlay()
{	
	if (m_pWorldEvent)
		m_pWorldEvent->OnBeginPlay(this);

	for (auto& actor : m_Actors)
	{
		actor->OnBeginPlay();
	}
	m_bIsPlaying = true;
}

void World::OnEndPlay()
{
	m_bIsPlaying = false;
	for (auto& actor : m_Actors)
	{
		actor->OnEndPlay();
	}
	m_Actors.clear();

	if (m_pWorldEvent)
		m_pWorldEvent->OnEndPlay(this);
}

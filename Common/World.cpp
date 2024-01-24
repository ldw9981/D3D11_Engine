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

void World::DestroyGameObject(Actor* pObject)
{
	pObject->OnEndPlay();	
	m_Actors.erase(pObject->m_iteratorInWorld);
	
	m_OnEndPlay.Invoke(this);
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
	m_OnBeginPlay.Invoke(this);

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

	m_OnEndPlay.Invoke(this);
}

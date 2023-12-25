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

void World::Render()
{
/*
	for (auto& actor : m_actors)
	{
		for (auto& component : actor->m_OwnedComponents)
		{
			
		}
	}*/
}

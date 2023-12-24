#include "pch.h"
#include "World.h"
#include "Actor.h"

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
}

void World::OnEndPlay()
{
	for (auto& actor : m_actors)
	{
		actor->OnEndPlay();
	}
	m_actors.clear();
}

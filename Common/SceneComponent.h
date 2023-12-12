#pragma once
class SceneComponent
{
public:
	SceneComponent();
	~SceneComponent();

	std::string m_Name;
	Math::Matrix m_Local;
	Math::Matrix m_World;
};


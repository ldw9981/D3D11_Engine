#pragma once
class SceneComponent
{
public:
	SceneComponent();
	~SceneComponent();

	std::string m_Name;
	Math::Matrix m_Local;
	Math::Matrix m_World;
	Math::Vector3 m_Position;
	Math::Vector3 m_Rotation;
	Math::Vector3 m_Scale;

	virtual void Update(float DeltaTime);
};


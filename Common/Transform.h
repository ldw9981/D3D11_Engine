#pragma once
class Transform
{
public:
	Transform* m_pParent = nullptr;
	std::string m_Name;
	Math::Matrix m_Local;
	Math::Matrix m_World;

	void SetParent(Transform* pParent) { m_pParent = pParent; };
	virtual void Update(float DeltaTime);
};
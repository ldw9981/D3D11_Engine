#pragma once
class Transform
{
public:
	Transform();
	~Transform();
	Transform* m_pParent = nullptr;

	Math::Matrix m_Local;
	Math::Matrix m_World;

	void SetParent(Transform* pParent) { m_pParent = pParent; };
	virtual void Update(float DeltaTime);
		
	
	Math::Vector3 GetWorldPosition();

};
#pragma once


class ITransform
{
public:
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position)=0;
	virtual void GetPosition(DirectX::SimpleMath::Vector3& position)=0;
};
class Transform : virtual public ITransform
{
public:
	Transform() {};
	~Transform() {};


protected:
	DirectX::SimpleMath::Vector3 m_position;
	float m_SameData;
public:
	void UpdateTransform(float DeltaTime);
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position);
	virtual void GetPosition(DirectX::SimpleMath::Vector3& position);
};


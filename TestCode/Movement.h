#pragma once
#include "Transform.h"

class IMovement
{
public:
	virtual DirectX::SimpleMath::Vector3 GetDirection() const = 0 ;
	virtual void SetDirection(DirectX::SimpleMath::Vector3 val) = 0;
};

class Movement : virtual public ITransform, virtual public IMovement
{
public:
	Movement() {};
	~Movement() {};
	
	virtual DirectX::SimpleMath::Vector3 GetDirection() const { return m_Direction; }
	virtual void SetDirection(DirectX::SimpleMath::Vector3 val) { m_Direction = val; }
private:
	DirectX::SimpleMath::Vector3 m_Direction;
	float m_Speed;
public:		
    virtual void UpdateMovement(float DeltaTime);
};


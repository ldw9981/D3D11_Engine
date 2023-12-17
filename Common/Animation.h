#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

struct aiNodeAnim;

//Transform
struct AnimationKey
{
	float Time = 0.0f;		// Ű�� �ð�
	Vector3 Position;
	Quaternion Rotation;
	Vector3 Scaling;
};

// Ư�� ����� �ð������� Transform
struct NodeAnimation
{	
	string NodeName;		// �ִϸ��̼��� ����� ���� ���������� ��� �̸�
	vector<AnimationKey> AnimationKeys;
	// aiNodeAnim���� AnimationKey�� ����
	void Create(aiNodeAnim* nodeAnimation,double tickPerSecond);
	// �ð��� ���� ������ Key���� ����ϴ� �Լ�
	void Evaluate(float time, Vector3& position, Quaternion& rotation, Vector3& scaling);
};

// ��� ����� �� �ð������� Transform
struct Animation
{
	string FilePath;
	string Name;
	float Duration = 0.0f;	// ��ü ����
	vector<NodeAnimation> NodeAnimations;

	void Create(const std::string strFBXFilePath, const aiAnimation* pAiAnimation);
	void Create(const std::string strFBXFilePath);
};

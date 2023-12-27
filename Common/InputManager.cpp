#include "pch.h"
#include "InputManager.h"

constexpr float ROTATION_GAIN = 0.004f;
constexpr float MOVEMENT_GAIN = 0.07f;

using namespace DirectX;
using namespace DirectX::SimpleMath;

InputManager::InputManager()
	:m_MouseState(), m_KeyboardState()
{
	assert(Instance == nullptr);
	Instance = this;
}

InputManager* InputManager::Instance = nullptr;

void InputManager::Update(float DeltaTime)
{
	m_MouseState = m_Mouse->GetState();
	m_MouseStateTracker.Update(m_MouseState);

	m_KeyboardState = m_Keyboard->GetState();
	m_KeyboardStateTracker.Update(m_KeyboardState);

	auto KeyboardLastState = m_KeyboardStateTracker.GetLastState();
	auto MouseLastState = m_MouseStateTracker.GetLastState();

	for (auto& it : m_InputProcessers)
	{
		(it)->OnInputProcess(m_KeyboardState, KeyboardLastState, m_MouseState, MouseLastState);
	}	
}

bool InputManager::Initialize(HWND hWnd)
{
	m_Keyboard = std::make_unique<Keyboard>();
	m_Mouse = std::make_unique<Mouse>();
	m_Mouse->SetWindow(hWnd);
	return true;
}

void InputManager::AddInputProcesser(InputProcesser* inputProcesser)
{
	m_InputProcessers.push_back(inputProcesser);
}

void InputManager::RemoveInputProcesser(InputProcesser* inputProcesser)
{
	m_InputProcessers.remove(inputProcesser);
}

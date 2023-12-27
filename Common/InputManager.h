#pragma once

#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>

class InputProcesser
{
public:
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyCurr, const DirectX::Keyboard::State& KeyLast,
		const DirectX::Mouse::State& MouseCurr, const DirectX::Mouse::State& MouseLast)=0;
};

class InputManager
{
public:
	InputManager();
	~InputManager() {};

	static InputManager* Instance;

	std::list<InputProcesser*> m_InputProcessers;

	// input
	std::unique_ptr<DirectX::Keyboard>              m_Keyboard;
	std::unique_ptr<DirectX::Mouse>                 m_Mouse;
	DirectX::Keyboard::KeyboardStateTracker         m_KeyboardStateTracker;
	DirectX::Mouse::ButtonStateTracker              m_MouseStateTracker;

	DirectX::Mouse::State                           m_MouseState;
	DirectX::Keyboard::State                        m_KeyboardState;

	void Update(float DeltaTime);
	bool Initialize(HWND hWnd);

	void AddInputProcesser(InputProcesser* inputProcesser);
	void RemoveInputProcesser(InputProcesser* inputProcesser);
};


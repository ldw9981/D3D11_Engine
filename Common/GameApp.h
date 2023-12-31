#pragma once
#include <windows.h>
#include "GameApp.h"
#include "TimeSystem.h"
#include "D3DRenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "CollisionManager.h"



#define MAX_LOADSTRING 100

class World;
class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp();
	
	static HWND m_hWnd;		//�����ʿ��ϴ� ������ ���������� ���ϱ����� ��������� �������.
	static GameApp* m_pInstance;			// �����ڿ��� �ν��Ͻ� �����͸� �����Ѵ�.
	
public:
	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;                                // ���� �ν��Ͻ��Դϴ�.
	WCHAR m_szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
	WNDCLASSEXW m_wcex;
	int  m_nCmdShow;
	GameTimer m_Timer;
	UINT m_ClientWidth;
	UINT m_ClientHeight;
	bool m_bUseConsole=false;
	D3DRenderManager m_Renderer;
	ResourceManager m_Resource;
	World* m_pCurrentWorld=nullptr;
	InputManager m_InputManager;
	CollisionManager m_CollisionManager;
public:
	// ������ ���� ���,����,���̱� �Ѵ�.
	virtual bool Initialize(UINT Width, UINT Height);
	virtual bool Run();
	virtual void Update(); // ��� ���� Ŭ�������� ����
	virtual void Render(); // ��� ���� Ŭ�������� ����

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	

	void ChangeWorld(World* pNewWorld);
};


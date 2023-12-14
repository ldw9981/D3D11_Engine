#pragma once
#include "../Common/GameApp.h"
#include "../Common/Mesh.h"
#include "../Common/Material.h"
#include "../Common/Animation.h"
#include "../Common/Node.h"
#include "../Common/Model.h"


using namespace DirectX::SimpleMath;
using namespace DirectX;


#define MAX_MODEL 3

class TutorialApp :
    public GameApp
{
public:
	TutorialApp(HINSTANCE hInstance);
	~TutorialApp();



	
	Model m_Model[MAX_MODEL];


	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Update();
	virtual void Render();

};


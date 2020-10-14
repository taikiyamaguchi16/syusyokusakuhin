#include "Title.h"

#include "Camera.h"
#include "game.h"
#include "CDirectInput.h"
#include "CObject2D.h"


CObject2D* cc;
CCamera* _camaa;

bool Title::Init()
{
	cc = new CObject2D();
	AddObjects();
	return true;
}

void Title::UnInit()
{
	for (auto item : m_obj_list) {
		delete item;
	}
	m_obj_list.clear();
	for (auto came : m_subCameras) {
		delete came;
	}
	m_subCameras.clear();
	delete cc;
}

void Title::MainLoop()
{
	Update();
	Draw();
}

void Title::Update()
{
	cc->Update();
	CDirectInput::GetInstance().GetKeyBuffer();			// �L�[���͎擾
	for (auto item = m_obj_list.begin(); item != m_obj_list.end();) {
		(*item)->Update();
		if (!(*item)->GetLife()) {
			delete *item;
			item = m_obj_list.erase(item);
			continue;
		}
		item++;
	}

	for (auto item2 : m_subCameras) {
		item2->Update();
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		cc->c_fg = true;
	}
	if (cc->CountUp()) {
		SceneManager::GetInstance()->ChangeScene(new CGame());
	}
	CDirectInput::GetInstance().GetMouseState();
}

void Title::Draw()
{
	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_X;
	vp.Height = (FLOAT)SCREEN_Y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	GetDX11DeviceContext()->RSSetViewports(1, &vp);
	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha


	// �����_�����O�O����
	DX11BeforeRender(ClearColor);

	// �C�~�B�f�B�G�C�g�R���e�L�X�g
	ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

	
	//==========================================================================================================
	cc->Update();
	cc->m_cameraMtx = _camaa->GetCameraMatrix();
	cc->Draw();
	for (int i = 0; i < 1; i++) {
		for (auto item : m_obj_list) {
			item->Draw();
		}
		m_subCameras[i]->Draw();
	}
	// �����_�����O�㏈��
	DX11AfterRender();
}

void Title::AddObjects()
{
	CObject* AirPlane;
	CObject* SkyDome;
	CObject* CameraObj;
	CObject* SubCamera;

	float _plane = 600.0f;
	float _color[4] = { 255.0f,255.0f,255.0f,255.0f };


	CCamera* _camera;

	CTransform* _trans;
	//==========================================================================================
	//AddComponent��Transform����I�I�I�I�I�I�I�I�I�I�I�I�I�I
	//���̎���Rigidbody
	//==========================================================================================


	CameraObj = new CObject;
	CameraObj->AddComponent<CTransform>();
	_camaa = _camera = CameraObj->AddComponent<CCamera>();
	CameraObj->SetName(std::string("Camera"));
	CameraObj->SetTag(std::string("Camera"));
	m_subCameras.emplace_back(CameraObj);
	_camera->SetMainCameraFg(true);


	AirPlane = new CObject;
	_trans = AirPlane->AddComponent<CTransform>();
	_trans->SetPos(XMFLOAT3(60.0f, 100.0f, 0));
	/*_rigid = AirPlane->AddComponent<CRigidbody>();
	_rigid->SetBoxSize(XMFLOAT3(3.0f, 3.0f, 3.0f));
	_rigid->SetMass(0.1f);*/
	AirPlane->SetMyFps(60);
	/*_render = AirPlane->AddComponent<CMeshRenderer>();
	_render->BoxInit(XMFLOAT3(3.0f, 3.0f, 3.0f));
	_rigid->InitDynamic();

	AirPlane->AddComponent<CSphereCollider>();*/
	AirPlane->SetName(std::string("AirPlane"));
	AirPlane->SetTag(std::string("Player"));
	m_obj_list.emplace_back(AirPlane);



	m_active_obj = AirPlane;
}

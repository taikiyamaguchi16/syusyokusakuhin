//*****************************************************************************
//!	@file	game.h
//!	@brief	�Q�[������
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include "Scene.h"
#include <iostream>
#include <vector>


class CGame final:public Scene
{
	
	//ID3D11Buffer*	m_vertexbuffer = nullptr;	// ���_�o�b�t�@
	//ID3D11Buffer*	m_idxbuffer = nullptr;		// �C���f�b�N�X�o�b�t�@	
	//ID3D11Buffer*	m_cbuffer = nullptr;		// �萔�o�b�t�@	

	//ID3D11VertexShader*	m_vs = nullptr;			// ���_�V�F�[�_�[
	//ID3D11PixelShader* m_ps = nullptr;			// �s�N�Z���V�F�[�_�[
	//ID3D11InputLayout* m_layout = nullptr;		// ���_���C�A�E�g

	//ID3D11ShaderResourceView* m_srv = nullptr;	// SRV
	//ID3D11Resource* m_texres = nullptr;			// texture resource


public:
	//�p�ɂɍ폜���s���邽��
	std::list<CComponent*> m_Components;
	CGame() { Init(); }
	~CGame() { UnInit(); }

	bool Init()override;
	void UnInit()override;
	void Update()override;
	void MainLoop()override;
	void Draw()override;

	
	inline void CreateObject(std::string str_) {
		CObject* obj = new CObject;
		obj->AddComponent<CTransform>();
		obj->SetName(str_);
		m_obj_list.emplace_back(obj);
	}

	template<typename T> 
	T * createInstance() { return new T; }


	void AddObjects();
};


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//******************************************************************************
//	End of file.
//******************************************************************************

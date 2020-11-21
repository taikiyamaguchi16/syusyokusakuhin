#pragma once
#include	<DirectXMath.h>
#include	"Shader.h"
#include	"Memory.h"

class DX11SetTransform {
	DX11SetTransform() {}
public:
	DX11SetTransform(const DX11SetTransform&) = delete;
	DX11SetTransform& operator=(const DX11SetTransform&) = delete;
	DX11SetTransform(DX11SetTransform&&) = delete;
	DX11SetTransform& operator=(DX11SetTransform&&) = delete;

	static DX11SetTransform* GetInstance() {
		static DX11SetTransform instance;
		return &instance;
	}
	// �萔�o�b�t�@��`(���[���h�ϊ��s��)
	ALIGN16 struct ConstantBufferWorld
	{
		XMMATRIX World;				// ���[���h�ϊ��s��
	};
	// �萔�o�b�t�@��`(�r���[�ϊ��s��)
	ALIGN16 struct ConstantBufferView
	{
		XMMATRIX View;				// �r���[�ϊ��s��
	};
	// �萔�o�b�t�@��`(���t���[���ݒ�)
	ALIGN16 struct ConstantBufferProjection
	{
		XMMATRIX Projection;		// �v���W�F�N�V�����ϊ��s��
	};

	enum class TYPE {
		WORLD,
		VIEW,
		PROJECTION
	};

	void Uninit() {
		if (m_pConstantBufferWorld) {
			m_pConstantBufferWorld->Release();
			m_pConstantBufferWorld = nullptr;
		}
		if (m_pConstantBufferView) {
			m_pConstantBufferView->Release();
			m_pConstantBufferView = nullptr;
		}
		if (m_pConstantBufferProjection) {
			m_pConstantBufferProjection->Release();
			m_pConstantBufferProjection = nullptr;
		}
	}

	bool Init();

	void SetTransform(TYPE type, DirectX::XMFLOAT4X4& matrix);
private:
	ID3D11Buffer * m_pConstantBufferWorld = nullptr;		// �R���X�^���g�o�b�t�@2
	ID3D11Buffer * m_pConstantBufferView = nullptr;			// �R���X�^���g�o�b�t�@2
	ID3D11Buffer * m_pConstantBufferProjection = nullptr;	// �R���X�^���g�o�b�t�@2
	ConstantBufferWorld m_CBWorld;				// b0 
	ConstantBufferView m_CBView;				// b1
	ConstantBufferProjection m_CBProjection;	// b2
};
#include "DX11Settransform.h"
#include "DirectX11Manager.h"

bool DX11SetTransform::Init()
{
	ID3D11Device* device = DirectX11Manager::m_pDevice.Get();

	// �R���X�^���g�o�b�t�@�쐬
	bool sts = CreateConstantBuffer(
		device,			// �f�o�C�X
		sizeof(ConstantBufferWorld),	// �T�C�Y
		&m_pConstantBufferWorld);			// �R���X�^���g�o�b�t�@�Q
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer world) error", "Error", MB_OK);
		return false;
	}

	// �R���X�^���g�o�b�t�@�쐬
	sts = CreateConstantBuffer(
		device,			// �f�o�C�X
		sizeof(ConstantBufferView),	// �T�C�Y
		&m_pConstantBufferView);			// �R���X�^���g�o�b�t�@�Q
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer view) error", "Error", MB_OK);
		return false;
	}

	// �R���X�^���g�o�b�t�@�쐬
	sts = CreateConstantBuffer(
		device,			// �f�o�C�X
		sizeof(ConstantBufferProjection),	// �T�C�Y
		&m_pConstantBufferProjection);			// �R���X�^���g�o�b�t�@�Q
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer projection) error", "Error", MB_OK);
		return false;
	}
	return true;
}

void DX11SetTransform::SetTransform(TYPE type, DirectX::XMFLOAT4X4 & matrix)
{
	ALIGN16 XMMATRIX mat;
	mat = DirectX::XMLoadFloat4x4(&matrix);

	switch (type) {
	case TYPE::WORLD:
		/*m_CBWorld.World = XMMatrixTranspose(mat);
		DirectX11Manager::m_pImContext->UpdateSubresource(m_pConstantBufferWorld, 0, nullptr, &m_CBWorld, 0, 0);
		DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, &m_pConstantBufferWorld);*/
		//DirectX11Manager::m_pImContext->PSSetConstantBuffers(0, 1, &m_pConstantBufferWorld);
		break;
	case TYPE::VIEW:
		/*m_CBView.View = XMMatrixTranspose(mat);
		DirectX11Manager::m_pImContext->UpdateSubresource(m_pConstantBufferView, 0, nullptr, &m_CBView, 0, 0);
		DirectX11Manager::m_pImContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferView);*/
		//DirectX11Manager::m_pImContext->PSSetConstantBuffers(1, 1, &m_pConstantBufferView);
		break;
	case TYPE::PROJECTION:
		/*m_CBProjection.Projection = XMMatrixTranspose(mat);
		DirectX11Manager::m_pImContext->UpdateSubresource(m_pConstantBufferProjection, 0, nullptr, &m_CBProjection, 0, 0);
		DirectX11Manager::m_pImContext->VSSetConstantBuffers(2, 1, &m_pConstantBufferProjection);*/
		//DirectX11Manager::m_pImContext->PSSetConstantBuffers(2, 1, &m_pConstantBufferProjection);
		break;
	}
}

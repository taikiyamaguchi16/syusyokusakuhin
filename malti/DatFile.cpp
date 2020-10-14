//*****************************************************************************
//!	@file	DatFile.cpp
//!	@brief	Dat�t�@�C���ǂݍ��݁A�`��N���X
//!	@note	
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"DatFile.h"
#include	"Shader.h"
#include	"WICTextureLoader.h"

//==============================================================================
//!	@fn		Load
//!	@brief	dat�t�@�C���ǂݍ���
//!	@param	dat�t�@�C����
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================
bool DatFile::Load(const char *filename, ID3D11Device* device11, ID3D11DeviceContext*	device11Context)
{
	bool sts;

	// dat�t�@�C���ǂݍ���
	m_datfile = new DatFileLoader();
	sts = m_datfile->Deserialize(filename);
	if (!sts) {
		return false;
	}

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBuffer(
		device11,
		sizeof(X::Vertex),				// �P���_������o�C�g��
		m_datfile->m_VertexSuu,			// ���_��
		m_datfile->m_Vertex,				// ���_�f�[�^�i�[�������擪�A�h���X
		&m_pVertexBuffer);				// ���_�o�b�t�@
	if (!sts){
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
		device11,
		m_datfile->m_IndexSuu,				// ���_��
		m_datfile->m_Index,				// ���_�f�[�^�i�[�������擪�A�h���X
		&m_pIndexBuffer);				// �C���f�b�N�X�o�b�t�@
	if (!sts){
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// �R���X�^���g�o�b�t�@�쐬
	sts = CreateConstantBuffer(
		device11,
		sizeof(X::ConstantBufferMaterial),
		&m_cb3);
	if (!sts){
		MessageBox(NULL, "CreateBuffer(constant buffer Material) error", "Error", MB_OK);
		return false;
	}

	// �V�F�[�_�[���\�[�X�r���[�쐬
	m_srv = new ID3D11ShaderResourceView*[m_datfile->m_MaterialSuu];//  m_xfile->m_MaterialSuu];
	m_texres = new ID3D11Resource*[m_datfile->m_MaterialSuu];


	for (int i = 0; i < m_datfile->m_MaterialSuu; i++){
		if (m_datfile->m_DecalTex[i] != "null" &&  m_datfile->m_DecalTex[i] != "none"){
			// �e�N�X�`���ǂݍ���
			bool sts = CreateSRVfromFile(m_datfile->m_DecalTex[i].c_str(), device11, device11Context, &m_texres[i], &m_srv[i]);
			if (!sts){
				std::string str;
				str = "CreateSRVfromFile " + m_datfile->m_DecalTex[i];
				MessageBox(NULL, str.c_str(), "Error", MB_OK);
			}
		}
		else{
			// �e�N�X�`���ǂݍ���
			bool sts = CreateSRVfromWICFile("assets/white.bmp",		// WIC�t�@�C������V�F�[�_�[���\�[�X�r���[���쐬����
				device11,
				device11Context,
				&m_texres[i],
				&m_srv[i]);
		}
	}

	return true;
}

//==============================================================================
//!	@fn		UnLoadXFile
//!	@brief	X�t�@�C���I�u�W�F�N�g�A�����[�h
//!	@param	
//!	@retval	�Ȃ�
//!	@note	
//==============================================================================
void DatFile::UnLoad()
{

	if (m_datfile != nullptr){
		for (int i = 0; i < m_datfile->m_MaterialSuu; i++){
			if (m_srv[i]){
				m_srv[i]->Release();
			}
			if (m_texres[i]) {
				m_texres[i]->Release();
			}
		}
		delete m_datfile;
	}

	if (m_srv != nullptr){
		delete[] m_srv;
	}

	if (m_texres != nullptr) {
		delete[] m_texres;
	}

	if (m_pIndexBuffer != nullptr){				// �C���f�b�N�X�o�b�t�@���
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer != nullptr){			// ���_�o�b�t�@���
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_cb3 != nullptr){						// �R���X�^���g�o�b�t�@���
		m_cb3->Release();
		m_cb3 = nullptr;
	}
}

//==============================================================================
//!	@fn		Draw
//!	@brief	X�t�@�C���`��
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//!	@note	
//==============================================================================
void DatFile::Draw(ID3D11DeviceContext* device)
{
	int indexnum=0;
	int indexstart = 0;

	X::ConstantBufferMaterial		cb;					// �R���X�^���g�o�b�t�@�R�p�\���́i�}�e���A���̒l�j

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(X::Vertex);
	unsigned  offset = 0;
	device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j

	indexstart = indexstart + indexnum;										// �C���f�b�N�X�̊J�n�ʒu

	for (int i = 0; i < m_datfile->m_MaterialSuu; i++){
		int subsetidx = -1;
		// �}�e���A�������Z�b�g
		device->PSSetShaderResources(0, 1, &m_srv[i]);

		// �}�e���A���ɊY������T�u�Z�b�g���擾����
		for (int j = 0; j < m_datfile->m_SubsetSuu; j++){
			if (i == m_datfile->m_Subset[j].m_MaterialIdx){

				cb.AmbientMaterial.x = m_datfile->m_Material[i].m_Ambient.x;					// �����̔��˗�
				cb.AmbientMaterial.y = m_datfile->m_Material[i].m_Ambient.y;					
				cb.AmbientMaterial.z = m_datfile->m_Material[i].m_Ambient.z;					
				cb.AmbientMaterial.w = 1.0f;												

				cb.DiffuseMaterial.x = m_datfile->m_Material[i].m_Diffuse.x;					// �f�B�t���[�Y���̔��˗�
				cb.DiffuseMaterial.y = m_datfile->m_Material[i].m_Diffuse.y;					
				cb.DiffuseMaterial.z = m_datfile->m_Material[i].m_Diffuse.z;					
				cb.DiffuseMaterial.w = 1.0f;												

//				cb.SpecularMaterial.x = m_xfile->m_Material[i].m_Specular.x;				// �X�y�L�������̔��˗�
//				cb.SpecularMaterial.y = m_xfile->m_Material[i].m_Specular.y;				
//				cb.SpecularMaterial.z = m_xfile->m_Material[i].m_Specular.z;				
				cb.SpecularMaterial.w = 1.0f;												

				cb.SpecularMaterial.x = m_datfile->m_Material[i].m_Diffuse.x;				// �X�y�L�������̔��˗�
				cb.SpecularMaterial.y = m_datfile->m_Material[i].m_Diffuse.y;
				cb.SpecularMaterial.z = m_datfile->m_Material[i].m_Diffuse.z;


				// �萔�o�b�t�@�X�V
				device->UpdateSubresource(
					m_cb3,			// �R���X�^���g�o�b�t�@
					0,				// �R�s�[��T�u���\�[�X
					NULL,			// �T�u���\�[�X���`����{�b�N�X�@�m�t�k�k�̏ꍇ�I�t�Z�b�g�Ȃ�
					&cb,			// �R�s�[���f�[�^
					0,				// 1�s�̃T�C�Y
					0);				// 1�[�x�X���C�X�̃T�C�Y

				// �萔�o�b�t�@3���s�N�Z���V�F�[�_�[�փZ�b�g
				device->VSSetConstantBuffers(
					3,				// �X�^�[�g�X���b�g
					1,				// ��
					&m_cb3);		// �R���X�^���g�o�b�t�@

				// �萔�o�b�t�@3���s�N�Z���V�F�[�_�[�փZ�b�g
				device->PSSetConstantBuffers(
					3,				// �X�^�[�g�X���b�g
					1,				// ��
					&m_cb3);		// �R���X�^���g�o�b�t�@

				subsetidx = j;
				device->DrawIndexed(m_datfile->m_Subset[subsetidx].m_VertexSuu,	// �`�悷��C���f�b�N�X��(Face*3)
					m_datfile->m_Subset[subsetidx].m_FaceStart*3,				// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
					0);															// ���_�o�b�t�@�̍ŏ�����g��
			}
		}
	}
}

//******************************************************************************
//	End of file.
//******************************************************************************
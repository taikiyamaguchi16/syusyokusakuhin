#pragma once
#include	<d3d11.h>
#include	"DatFile.h"

class CModel {
private:
	DatFile					m_datfile;						// dat�t�@�C��
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// �e�N�X�`���r�q�u
public:
	bool Init(const char* filename,const char* vsfile,const char* psfile);
	void Uninit();
	void Update();
	void Draw();
	DatFileLoader* GetDatFileData() {
		return m_datfile.GetDatFileLoader();
	}
};

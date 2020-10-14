//----------------------------------------------------------------------------------------------------
//	Include header files.
//----------------------------------------------------------------------------------------------------
#include	<d3d11.h>
#include	"DatFile.h"

//----------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//----------------------------------------------------------------------------------------------------
class CObject2D
{
protected:

	XMFLOAT4X4 m_matrix;

	ID3D11Buffer*	m_pVertexBuffer = nullptr;		// ���_�o�b�t�@
	ID3D11Buffer*	m_pIndexBuffer = nullptr;		// �C���f�b�N�X�o�b�t�@	
	ID3D11Buffer*	m_pCBuffer = nullptr;			// �萔�o�b�t�@	

	ID3D11VertexShader*     m_pVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// �e�N�X�`���r�q�u
	ID3D11Resource* m_texRes = nullptr;			// texture resource

	// ���_�t�H�[�}�b�g
	struct Vertex {
		XMFLOAT3 pos;					// ���W
		XMFLOAT3 normal;				// �@���x�N�g��
		XMFLOAT2 tex;					// �e�N�X�`�����W
	};

	// �萔�o�b�t�@��`�i�}�e���A���P�ʁj
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// �����̔��˗�
		XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
		XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
	};

	// �}�e���A��
	ConstantBufferMaterial material = {
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),			// �����}�e���A��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),			// �g�U���˃}�e���A��
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),			// ���ʔ��˃}�e���A��
	};

	float m_geight;
	float m_width;

	XMFLOAT3 m_pos = XMFLOAT3(0, 0, 0);

	float alfa = 1.0f;
public:
	bool c_fg = false;
	XMFLOAT4X4 m_cameraMtx;
	CObject2D() { Init("assets/title.png","shader/vs.fx","shader/ps.fx"); };
	virtual ~CObject2D() { Uninit(); };

	bool Init(const char* filename, const char* vsfile, const char* psfile);
	void Uninit();
	void Update();
	void Draw();

	bool CountUp();
};

//****************************************************************************************************
//	End of file.
//****************************************************************************************************
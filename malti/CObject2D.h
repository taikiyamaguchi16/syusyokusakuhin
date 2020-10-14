//----------------------------------------------------------------------------------------------------
//	Include header files.
//----------------------------------------------------------------------------------------------------
#include	<d3d11.h>
#include	"DatFile.h"

//----------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//----------------------------------------------------------------------------------------------------
class CObject2D
{
protected:

	XMFLOAT4X4 m_matrix;

	ID3D11Buffer*	m_pVertexBuffer = nullptr;		// 頂点バッファ
	ID3D11Buffer*	m_pIndexBuffer = nullptr;		// インデックスバッファ	
	ID3D11Buffer*	m_pCBuffer = nullptr;			// 定数バッファ	

	ID3D11VertexShader*     m_pVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// テクスチャＳＲＶ
	ID3D11Resource* m_texRes = nullptr;			// texture resource

	// 頂点フォーマット
	struct Vertex {
		XMFLOAT3 pos;					// 座標
		XMFLOAT3 normal;				// 法線ベクトル
		XMFLOAT2 tex;					// テクスチャ座標
	};

	// 定数バッファ定義（マテリアル単位）
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// 環境光の反射率
		XMFLOAT4	DiffuseMaterial;		// ディフューズ光の反射率
		XMFLOAT4	SpecularMaterial;		// スペキュラ光の反射率
	};

	// マテリアル
	ConstantBufferMaterial material = {
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),			// 環境光マテリアル
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),			// 拡散反射マテリアル
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),			// 鏡面反射マテリアル
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
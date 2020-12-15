#include "Sphere.h"


void CSphere::Init()
{
	Init(1.f, 10, 10);
	vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/DefaultShape.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/DefaultShape.hlsl", "psMain"));

	//InputLayout�̍쐬
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 3, "assets/Shaders/DefaultShape.hlsl", "vsMain"));

	//�R���X�^���g�o�b�t�@�̍쐬
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::CreateConstantBuffer(sizeof(Material), &cb2);

	mate.df = XMFLOAT4(1, 1, 1, 1);
}

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void CSphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector);			// XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v);			// ���K��
	XMStoreFloat3(&Normal, v);			// XMVECTOR=>XMFLOAT3
}

// �C���f�b�N�X�f�[�^���쐬
void CSphere::CreateIndex() {

	// �ʐ��Z�b�g
	m_facenum = m_divY * m_divX * 2;

	//m_face = new Face[m_facenum];

//	Face* pface = m_face;
	int pface = 0;
	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1)*y + x;			// ������W�̃C���f�b�N�X

			// �㔼��
			m_face[pface].idx[0] = count;
			m_face[pface].idx[1] = count + 1;
			m_face[pface].idx[2] = count + 1 + (m_divX + 1);

			pface++;

			// ������
			m_face[pface].idx[0] = count;
			m_face[pface].idx[1] = count + (m_divX + 1) + 1;
			m_face[pface].idx[2] = count + (m_divX + 1);

			pface++;
		}
	}

	ib.Attach(DirectX11Manager::CreateIndexBuffer(m_face->idx, (UINT)sizeof(m_face)));
}

// ���_�f�[�^���쐬
void CSphere::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	//m_vertex = new Vertex[(m_divX + 1) * (m_divY + 1)];
	//Vertex* pvtx = m_vertex;
	int pvtx = 0;
	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (float)(PI * (float)y) / (float)m_divY;    // �p���Z�b�g
		float r = m_radius * sinf(elevation);					// �p�ɉ��������a���v�Z

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (float)(2.0f * PI * (float)x) / (float)m_divX;	// ���ʊp���Z�b�g

			// ���_���W�Z�b�g
			m_vertex[pvtx].Pos.x = r * cosf(azimuth);
			m_vertex[pvtx].Pos.y = m_radius * cosf(elevation);
			m_vertex[pvtx].Pos.z = r * sinf(azimuth);

			// �@���x�N�g���Z�b�g
			Normalize(m_vertex[pvtx].Pos, Normal);		// �@�����v�Z
			m_vertex[pvtx].Normal = Normal;				// �@�����Z�b�g
			// ����
			pvtx++;
		}
	}
	vb.Attach(DirectX11Manager::CreateVertexBuffer(m_vertex, (UINT)sizeof(m_vertex)));
}

// �`��
void CSphere::Draw() {
	DirectX11Manager::SetVertexShader(vs.Get());
	DirectX11Manager::SetPixelShader(ps.Get());
	DirectX11Manager::SetInputLayout(il.Get());

	DirectX11Manager::UpdateConstantBuffer(cb.Get(), DirectX11Manager::m_constantBuffer);
	ID3D11Buffer* tmpCb[] = { cb.Get() };
	DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, tmpCb);

	DirectX11Manager::SetVertexBuffer(vb.Get(), sizeof(Vertex));
	DirectX11Manager::SetIndexBuffer(ib.Get());


	DirectX11Manager::UpdateConstantBuffer(cb2.Get(), mate);
	ID3D11Buffer* tmpCb2[] = { cb2.Get() };
	DirectX11Manager::m_pImContext->PSSetConstantBuffers(2, 1, tmpCb2);


	DirectX11Manager::m_pImContext->DrawIndexed(m_facenum * 3,		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}



bool CSphere::Init(float r,				// ���a
	int division_horizontal,			// ���������̕�����
	int division_vertical				// ���������̕�����
	) {
	// ��������ۑ�
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// ���a��ۑ�
	m_radius = r;

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	return true;
}

void CSphere::Exit() {

	//if (m_vertex) {
	//	delete[] m_vertex;
	//}

	//if (m_face) {
	//	delete[] m_face;
	//}
}
#pragma once

#include <stdint.h>
#include <string>
#include <DirectXMath.h>
#include "NonCopyable.h"

namespace X{
    typedef	int32_t VertexSuu; // ���_��
    struct Vertex{
        DirectX::XMFLOAT3 m_Pos;		// �ʒu
		DirectX::XMFLOAT3 m_Normal;		// �@��
		DirectX::XMFLOAT2 m_Tex;		// �e�N�X�`�����W
    };

	typedef	int32_t IndexSuu;     // �C���f�b�N�X�̐��B
	typedef	uint32_t Index;       // �C���f�b�N�X�B

    typedef	int32_t MaterialSuu;  // �}�e���A���̐�
	struct Material{
        float m_Alpha;
		DirectX::XMFLOAT3 m_Diffuse;
		DirectX::XMFLOAT3 m_Specular;
        float	m_SpecularPower;
		DirectX::XMFLOAT3 m_Ambient;
		DirectX::XMFLOAT3 m_Emissive;

        Material()
            : m_Alpha(0),
            m_Diffuse(DirectX::XMFLOAT3(0,0,0)),
			m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
            m_SpecularPower(0),
			m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
			m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
        {
        }
	};

    typedef	int32_t SubsetSuu;  // �T�u�Z�b�g�̐�
    struct Subset{
        // �}�e���A���e�[�u���̃C���f�b�N�X�B
        // �g��Ȃ��Ȃ�-1�B
        int32_t m_MaterialIdx;				// �}�e���A���̃C���f�b�N�X

        int32_t m_VertexSuu;			// ���_��

		unsigned int	m_FaceStart;			// �J�n��
		unsigned int	m_FaceCount;			// �ʐ�
		unsigned int	m_VertexStart;			// �J�n���_
		unsigned int	m_VertexCount;			// ���_�̐�

        Subset()
            : m_MaterialIdx(-1),
            m_VertexSuu (0)
        {
        }
    };
}

class DatFileLoader : private NonCopyable{
public:
    X::VertexSuu          m_VertexSuu;
    X::Vertex*            m_Vertex;
    
    X::IndexSuu           m_IndexSuu;
    X::Index*             m_Index;
    
    X::MaterialSuu        m_MaterialSuu;
    X::Material*          m_Material;
    std::string*          m_DecalTex;     // �}�e���A���z��ƑΉ��B�Ȃ���΋󕶎���B

    X::SubsetSuu          m_SubsetSuu;
    X::Subset*            m_Subset;

public:
	explicit DatFileLoader(){}
    ~DatFileLoader();
	void Serialize(const char* filename);
	bool Deserialize(const char* filename);
};
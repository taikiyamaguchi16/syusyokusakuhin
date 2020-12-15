#include "CorePBR.hlsli"

SamplerState samLinear : register(s0);


cbuffer ConstBuff : register(b0)
{
	matrix mtxWorld;
	matrix mtxView;
	matrix mtxProj;
}

cbuffer ConstantBufferMaterial : register(b2)
{
	float4	diffuseMaterial;
}


struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Nor : NORMAL;
	float4 Col : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Nor : NORMAL;
	float4 Col : COLOR;
};

PS_INPUT vsMain(VS_INPUT pos)
{
	PS_INPUT o = (PS_INPUT)0;
	o.Pos = mul(float4(pos.Pos, 1), mtxWorld);
	//o.ViewDirection = o.Pos.xyz / o.Pos.w - mtxView._41_42_43;
	o.Pos = mul(o.Pos, mtxView);
	o.Pos = mul(o.Pos, mtxProj);


	float3x3 rotWorld = float3x3(mtxWorld._11_12_13, mtxWorld._21_22_23, mtxWorld._31_32_33);
	o.Nor = mul(pos.Nor, rotWorld);
	return o;
}

float4 psMain(PS_INPUT input) : SV_TARGET
{
	return diffuseMaterial;
}
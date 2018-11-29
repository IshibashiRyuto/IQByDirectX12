cbuffer mat:register(b0)
{
	float4x4 wvp;			// wvp行列
	float4x4 world;			// world行列
	float4x4 view;			// view行列
	float4x4 projection;	// projection行列
    float3 eyePos;          // 視点
    float3 eyeDir;          // 視線ベクトル
    float3 upper;           // 上方向ベクトル
    float3 right;           // 右方向ベクトル
}

cbuffer material : register(b1)
{
	float4 diffuseColor;
	float specularity;
	float3 specularColor;
	float3 ambientColor;
	uint isUseTexture;
	uint sphereFlag;
}


cbuffer boneMatrix : register(b2)
{
	float4x4 boneMatrix[512];
}

struct VSInput
{
	float3 position     : POSITION0;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD;
	min16uint2	boneno	: BONENO;
	min16uint	weight	: WEIGHT;
	float4x4 modelMatrix : INSTANCE_MATRIX;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 origPosition	: POSITION1;
	float3 normal   : NORMAL;
    float3 color : COLOR;
	float2 uv       : TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	float w = input.weight / 100.f;
	uint idx1 = input.boneno.x;
	uint idx2 = input.boneno.y;

	float4x4 localModelMatrix = boneMatrix[idx1] * w + boneMatrix[idx2] * (1.0f - w);

	output.position = mul(wvp, mul(input.modelMatrix, mul(localModelMatrix, float4(input.position, 1.0f))));
	output.origPosition = mul(world, mul(input.modelMatrix,mul(localModelMatrix, float4(input.position, 1.0f))));

	output.normal = normalize(mul(world,mul(input.modelMatrix, mul(localModelMatrix,float4(input.normal, 0.0f)))).xyz);

	output.uv = input.uv;

	return output;
}

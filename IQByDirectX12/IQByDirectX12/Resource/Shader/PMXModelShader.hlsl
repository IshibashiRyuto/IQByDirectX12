Texture2D<float4> materialNormalTexture : register(t0);		// マテリアル通常テクスチャ
Texture2D<float4> materialSphereTexture : register(t1);		// マテリアルスフィアテクスチャ

SamplerState smp : register(s0);  

cbuffer mat : register(b0)
{
    float4x4 wvp;           // wvp行列
    float4x4 world;         // world行列
    float4x4 view;          // view行列
    float4x4 projection;    // projection行列	
}

cbuffer material : register(b1)
{
    float3 diffuseColor;
    float alpha;
    float specularity;
    float3 specularColor;
    float3 ambientColor;
}

cbuffer boneMatrix : register(b2)
{
	float4x4 boneMatrix[256];
}

struct VSInput
{
    float3 position : POSITION0;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4x4 appendUV : APPEND_UV;
	uint deformType : DEFORM_TYPE;
	uint4 boneIndex : BONE_INDEX;
	float4 boneWeight : BONE_WEIGHT;
    float4x4 modelMatrix : INSTANCE_MATRIX;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 origPosition : POSITION1;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

typedef VSOutput PSInput;

VSOutput VSMain(VSInput input)
{
    VSOutput output;

	uint boneIndex = input.boneIndex.x;
	float4x4 localBoneMatrix = boneMatrix[boneIndex];
    float bone1Weight,bone2Weight,bone3Weight,bone4Weight;


    switch (input.deformType)
    {
        case 0: // BDEF1
            localBoneMatrix = boneMatrix[boneIndex];
            break;
        case 1: // BDEF2
            bone1Weight = input.boneWeight.x;
            localBoneMatrix = boneMatrix[input.boneIndex.x] * bone1Weight + boneMatrix[input.boneIndex.y] * (1.0f - bone1Weight);
            break;
        case 2: // BDEF4
            bone1Weight = input.boneWeight.x;
            bone2Weight = input.boneWeight.y;
            bone3Weight = input.boneWeight.z;
            bone4Weight = input.boneWeight.w;
            localBoneMatrix = boneMatrix[input.boneIndex.x] * bone1Weight + boneMatrix[input.boneIndex.y] * bone2Weight + boneMatrix[input.boneIndex.z] * bone3Weight + boneMatrix[input.boneIndex.w] * bone4Weight;
        break;
        case 3: // SDEF
            bone1Weight = input.boneWeight.x;
            localBoneMatrix = boneMatrix[input.boneIndex.x] * bone1Weight + boneMatrix[input.boneIndex.y] * (1.0f - bone1Weight);
            break;
        case 4: // QDEF
            break;
    }

    output.position = mul(wvp, mul(input.modelMatrix, mul(localBoneMatrix, float4(input.position, 1.0f))));
    output.origPosition = mul(world, mul(input.modelMatrix, mul( localBoneMatrix ,float4(input.position, 1.0f))));

    input.modelMatrix._14_24_34 = float3(0.0f, 0.0f, 0.0f);
	localBoneMatrix._14_24_34 = float3(0.0f, 0.0f, 0.0f);
	
	output.normal = normalize( mul(input.modelMatrix, mul(localBoneMatrix, float4(input.normal, 1.0f))) .xyz);

    output.uv = input.uv;
    
    return output;
}


float4 PSMain(PSInput input) : SV_Target
{
	float3 light = normalize(float3(0.0f, 1.0f, -1.0f));
    float3 lightSpecularColor = float3(1.0f, 1.0f, 1.0f);
    float3 lightDiffuseColor = float3(1.0f, 1.0f, 1.0f);
    float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);

    float3 eyePosition = float3(0.0f, 20.0f, -15.0f);

    float3 vray = normalize(input.origPosition - eyePosition);
	
    float brightness = dot(input.normal, light);
    
    float attenuation = 1.0f;

	/// アンビエントカラー計算
	float3 modelAmbientColor = ambientColor;


	// ディフューズカラー計算
    float3 modelDiffuseColor = diffuseColor * brightness;
	modelDiffuseColor = modelDiffuseColor * lightDiffuseColor;
	modelDiffuseColor += modelAmbientColor;

	// スペキュラ計算
	float3 reflectRay = normalize(reflect(vray, input.normal));
	float3 modelSpecularColor = specularColor * materialSphereTexture.Sample(smp, reflectRay.xy * float2(0.5, -0.5) + float2(0.5, 0.5)).rgb;
    float spec = saturate(pow(dot(reflect(-light, input.normal), -vray), specularity));
    modelSpecularColor = modelSpecularColor * spec;

    
    float4 texColor = materialNormalTexture.Sample(smp, input.uv);

    float4 modelColor = float4(modelDiffuseColor, alpha);
	modelColor = modelColor * texColor +float4(modelSpecularColor, 0.0f);

    return modelColor;
}
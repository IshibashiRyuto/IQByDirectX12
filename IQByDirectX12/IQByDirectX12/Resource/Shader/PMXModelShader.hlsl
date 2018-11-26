Texture2D<float4> surfaceTexture		: register(t0);		// �}�e���A���ʏ�e�N�X�`��
Texture2D<float4> materialAddSphereTexture	: register(t1);	// �}�e���A�����Z�X�t�B�A�e�N�X�`��
Texture2D<float4> materialMulSphereTexture	: register(t2);	// �}�e���A����Z�X�t�B�A�e�N�X�`��
Texture2D<float4> toonTexture				: register(t3);	// �g�D�[���e�N�X�`��

SamplerState smp : register(s0);  

cbuffer mat : register(b0)
{
    float4x4 wvp;           // wvp�s��
    float4x4 world;         // world�s��
    float4x4 view;          // view�s��
    float4x4 projection;    // projection�s��	
    float3 eyePos;          // ���_
    float3 eyeDir;          // �����x�N�g��
    float upper;            // ������x�N�g��
    float right;            // �E�����x�N�g��
}

cbuffer material : register(b1)
{
    float4 diffuseColor;
    float4 specularColor;
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

	matrix m = mul(input.modelMatrix, localBoneMatrix);
	m = mul(wvp, m);
	output.position = mul(m, float4(input.position, 1.0f));

    output.origPosition = mul(world, mul(input.modelMatrix, mul( localBoneMatrix ,float4(input.position, 1.0f))));

	output.normal = normalize( mul(input.modelMatrix, mul(localBoneMatrix, float4(input.normal, 0.0f))) .xyz);

    output.uv = input.uv;
    
    return output;
}


float4 PSMain(PSInput input) : SV_Target
{
	float3 light = normalize(float3(-0.41f, -0.82f, 0.41f));
	float3 lightSpecularColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightDiffuseColor = float3(0.6f, 0.6f, 0.6f);
	float3 lightAmbientColor = float3(1.0f, 1.0f, 1.0f);

    float3 vray = normalize(input.origPosition - eyePos);
	
    float brightness = dot(input.normal, -light);
    
    float attenuation = 1.0f;

	/// �A���r�G���g�J���[�v�Z
	float3 modelAmbientColor = ambientColor * lightAmbientColor;


	// �f�B�t���[�Y�J���[�v�Z
    float3 modelDiffuseColor = saturate( diffuseColor.rgb/* * brightness*/);
    //toonTexture.Sample(smp, float2(0, 1 - brightness)).xyz;
    modelDiffuseColor = modelDiffuseColor * lightDiffuseColor;
    modelDiffuseColor = saturate(modelDiffuseColor + modelAmbientColor);

	// �X�y�L�����v�Z
	float3 reflectRay = normalize(reflect(vray, input.normal));
    float spec = saturate(pow(dot(reflect(light, input.normal), -vray), specularColor.w));
    float3 modelSpecularColor = specularColor.rgb * spec * lightSpecularColor;
    
    float4 texColor = surfaceTexture.Sample(smp, input.uv);


	// �X�t�B�A�}�b�v�puv�v�Z
    float3 vrayAxisX = normalize(cross(vray, upper));
    float3 vrayAxisY = normalize(cross(vray, vrayAxisX));
	float2 sphereUV = float2(dot(vrayAxisX, input.normal), dot(vrayAxisY, input.normal));
	sphereUV = sphereUV * float2(0.5, -0.5) + float2(0.5f, 0.5f);

	// �J���[�v�Z
	float4 modelColor = saturate( float4(modelDiffuseColor, diffuseColor.a) * texColor );
	modelColor = modelColor * toonTexture.Sample(smp, float2(0.0f, saturate( acos(brightness) / (3.141592f) )));
	modelColor = saturate(modelColor * materialMulSphereTexture.Sample(smp, sphereUV) + materialAddSphereTexture.Sample(smp, sphereUV) +float4(modelSpecularColor, 0.0f));
    

    return modelColor;
}
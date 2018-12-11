/**
*   Fast Approximate Anti-Aliasing(FXAA)
*/
Texture2D<float4> tex : register(t0);
Texture2D<float> depthMap : register(t1);
SamplerState smp : register(s0);

/**
*   Define's
*/
#define LUMA_THRESHOLD      ( 1.0 / 16.0)   //! �P�x�X���b�V�����h
#define FXAA_REDUCE_MUL     ( 1.0 / 8.0)    //! 
#define FXAA_REDUCE_MIN     ( 1.0 / 128.0 ) 
#define FXAA_SPAN_MAX       ( 4.0 )             

/**
*   @brief  �P�x���v�Z����
*
*   @param[in]  color   : �P�x�v�Z���s���F
*
*   @return �P�x
*/
float CalcLuma(float3 color)
{
    return dot(color, float3(0.299, 0.587, 0.114));
}


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 NVIDIA_FXAA(PSInput input);
float4 Geeks3D_FXAA(PSInput input);

float4 PSMain(PSInput input) : SV_TARGET
{
   return Geeks3D_FXAA(input);

}

/** Nvidia WhitePaper �Q�l
http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
*/
float4 NVIDIA_FXAA(PSInput input)
{
    // �T���v�����O�̒����_���`
    float2 posM = input.uv;

    // �T���v�����O�_�Ǝ���4�ߖT�̉�f�l���擾����
    float3 colorM = tex.Sample(smp, posM);
    float3 colorN = tex.Sample(smp, posM, int2(0, -1));
    float3 colorS = tex.Sample(smp, posM, int2(0, 1));
    float3 colorE = tex.Sample(smp, posM, int2(1, 0));
    float3 colorW = tex.Sample(smp, posM, int2(-1, 0));
    
    // ��f�l����P�x���v�Z
    float lumaN = CalcLuma(colorN);
    float lumaS = CalcLuma(colorS);
    float lumaE = CalcLuma(colorE);
    float lumaW = CalcLuma(colorW);
    float lumaM = CalcLuma(colorM);
    float rangeMin = (min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW))));
    float rangeMax = (max(lumaM, max(min(lumaN, lumaS), max(lumaE, lumaW))));
    float range = rangeMax - rangeMin;

    // �P�x������������΂��̂܂܂̐F��Ԃ�
    if (range < LUMA_THRESHOLD)
    {
        return float4(colorM, 1);
    }

    // ���ߖT�̋P�x���v�Z
    float lumaNE = CalcLuma(tex.Sample(smp, posM, int2(1, -1)).xyz);
    float lumaNW = CalcLuma(tex.Sample(smp, posM, int2(-1, -1)).xyz);
    float lumaSE = CalcLuma(tex.Sample(smp, posM, int2(1, 1)).xyz);
    float lumaSW = CalcLuma(tex.Sample(smp, posM, int2(-1, 1)).xyz);

    // ���ߖT����G�b�W�����Z�o
    /*--------------------------------------------------*/
    /*
        �������̋P�x��p���ăG�b�W���o
        ������
        ������
        ������
    */
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
    /*--------------------------------------------------*/
    /*
        �������̋P�x��p���ăG�b�W���o
        ������
        ������
        ������
    */
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
    /*--------------------------------------------------*/
    /*
        �������̋P�x��p���ăG�b�W���o
        ������
        ������
        ������
    */
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    
    //  ����8�ߖT�̃G�b�W���v�Z
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;

    
    float2 rcpFrame;
    tex.GetDimensions(rcpFrame.x, rcpFrame.y);
    float lengthSign = rcpFrame.x;
    bool horzSpan = edgeHorz >= edgeVert;
    if (!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }
    if (horzSpan)
    {
        lengthSign = rcpFrame.y;
    }
}

/**
    Geeks 3D �Q�l
    http://www.geeks3d.com/20110405/fxaa-fast-approximate-anti-aliasing-demo-glsl-opengl-test-radeon-geforce/
*/
float4 Geeks3D_FXAA(PSInput input)
{
    float2 size;
    float2 rcpFrame;
    tex.GetDimensions(size.x, size.y);

    rcpFrame.x = 1.0f / size.x;
    rcpFrame.y = 1.0f / size.y;
    
    // �T���v�����O�̒����_�ƍ�����`
    float2 posM = input.uv;
    float2 posNW = posM - rcpFrame * 0.5;

    // �T���v�����O�_�Ǝl�ߖT�̃J���[���擾
    float3 colorM = tex.Sample(smp, posM).rgb;
    float3 colorSE = tex.Sample(smp, posNW, int2(1, 1)).rgb;
    float3 colorSW = tex.Sample(smp, posNW, int2(0, 1)).rgb;
    float3 colorNE = tex.Sample(smp, posNW, int2(1, 0)).rgb;
    float3 colorNW = tex.Sample(smp, posNW).rgb;

    // �T���v�����O�_�Ǝl�ߖT�̋P�x���v�Z
    float lumaM = CalcLuma(colorM);
    float lumaNE = CalcLuma(colorNE);
    float lumaNW = CalcLuma(colorNW);
    float lumaSE = CalcLuma(colorSE);
    float lumaSW = CalcLuma(colorSW);

    // �P�x�����Z�o
    float lumaMin = min(lumaM, min(min(lumaNE, lumaNW), min(lumaSE, lumaSW)));
    float lumaMax = max(lumaM, max(max(lumaNE, lumaNW), max(lumaSE, lumaSW)));
    

    // �P�x������G�b�W�x�N�g�����Z�o
    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    // �G�b�W�x�N�g����p���ăT���v�����O�ʒu����
    float dirReduce = max(
    (lumaNW + lumaNE + lumaSE + lumaSW) * 0.25 * FXAA_REDUCE_MUL,
        FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
        max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * rcpFrame;

    float4 color = float4(0, 0, 0, 1);
    
    //// ���_�Ɉʒu�␳�������ăT���v�����O
    //for (int i = 0; i < 4; ++i)
    //{
    //    color.rgb += (1.0 / 4.0) * tex.Sample(smp, posM + dir * (float(i) / (4.0 - 1.0) - 0.5)).rgb;
    //}
    float3 colorA = (1.0 / 2.0) * (
        tex.Sample(smp, posM + dir * (1.0 / 3.0 - 0.5)).xyz +
        tex.Sample(smp, posM + dir * (2.0 / 3.0 - 0.5)).xyz 
    );
    float3 colorB = colorA * (1.0 / 2.0) + (1.0 / 4.0) * (
        tex.Sample(smp, posM + dir * (0.0 / 3.0 - 0.5)).xyz +
        tex.Sample(smp, posM + dir * (3.0 / 3.0 - 0.5)).xyz 
    );
    float lumaB = CalcLuma(colorB);

    if ((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        color.rgb = colorA;
    }
    else
    {
        color.rgb = colorB;
    }
    saturate(color);
    
    return color;
}
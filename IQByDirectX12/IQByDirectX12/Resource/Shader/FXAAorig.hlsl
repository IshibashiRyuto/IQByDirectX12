/**
*   Fast Approximate Anti-Aliasing(FXAA)
*/
Texture2D<float4> tex : register(t0);
Texture2D<float> depthMap : register(t1);
SamplerState smp : register(s0);

/**
*   Define's
*/
#define LUMA_THRESHOLD      ( 1.0 / 16.0)   //! 輝度スレッショルド
#define FXAA_REDUCE_MUL     ( 1.0 / 8.0)    //! 
#define FXAA_REDUCE_MIN     ( 1.0 / 128.0 ) 
#define FXAA_SPAN_MAX       ( 4.0 )             

/**
*   @brief  輝度を計算する
*
*   @param[in]  color   : 輝度計算を行う色
*
*   @return 輝度
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

/** Nvidia WhitePaper 参考
http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
*/
float4 NVIDIA_FXAA(PSInput input)
{
    // サンプリングの中央点を定義
    float2 posM = input.uv;

    // サンプリング点と周囲4近傍の画素値を取得する
    float3 colorM = tex.Sample(smp, posM);
    float3 colorN = tex.Sample(smp, posM, int2(0, -1));
    float3 colorS = tex.Sample(smp, posM, int2(0, 1));
    float3 colorE = tex.Sample(smp, posM, int2(1, 0));
    float3 colorW = tex.Sample(smp, posM, int2(-1, 0));
    
    // 画素値から輝度を計算
    float lumaN = CalcLuma(colorN);
    float lumaS = CalcLuma(colorS);
    float lumaE = CalcLuma(colorE);
    float lumaW = CalcLuma(colorW);
    float lumaM = CalcLuma(colorM);
    float rangeMin = (min(lumaM, min(min(lumaN, lumaS), min(lumaE, lumaW))));
    float rangeMax = (max(lumaM, max(min(lumaN, lumaS), max(lumaE, lumaW))));
    float range = rangeMax - rangeMin;

    // 輝度差が小さければそのままの色を返す
    if (range < LUMA_THRESHOLD)
    {
        return float4(colorM, 1);
    }

    // 八近傍の輝度を計算
    float lumaNE = CalcLuma(tex.Sample(smp, posM, int2(1, -1)).xyz);
    float lumaNW = CalcLuma(tex.Sample(smp, posM, int2(-1, -1)).xyz);
    float lumaSE = CalcLuma(tex.Sample(smp, posM, int2(1, 1)).xyz);
    float lumaSW = CalcLuma(tex.Sample(smp, posM, int2(-1, 1)).xyz);

    // 八近傍からエッジ方向算出
    /*--------------------------------------------------*/
    /*
        黒部分の輝度を用いてエッジ検出
        □■□
        ■■■
        □■□
    */
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
    /*--------------------------------------------------*/
    /*
        黒部分の輝度を用いてエッジ検出
        ■■■
        □□■
        □□■
    */
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
    /*--------------------------------------------------*/
    /*
        黒部分の輝度を用いてエッジ検出
        ■□□
        ■□□
        ■■■
    */
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    
    //  周囲8近傍のエッジを計算
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
    Geeks 3D 参考
    http://www.geeks3d.com/20110405/fxaa-fast-approximate-anti-aliasing-demo-glsl-opengl-test-radeon-geforce/
*/
float4 Geeks3D_FXAA(PSInput input)
{
    float2 size;
    float2 rcpFrame;
    tex.GetDimensions(size.x, size.y);

    rcpFrame.x = 1.0f / size.x;
    rcpFrame.y = 1.0f / size.y;
    
    // サンプリングの中央点と左上を定義
    float2 posM = input.uv;
    float2 posNW = posM - rcpFrame * 0.5;

    // サンプリング点と四近傍のカラーを取得
    float3 colorM = tex.Sample(smp, posM).rgb;
    float3 colorSE = tex.Sample(smp, posNW, int2(1, 1)).rgb;
    float3 colorSW = tex.Sample(smp, posNW, int2(0, 1)).rgb;
    float3 colorNE = tex.Sample(smp, posNW, int2(1, 0)).rgb;
    float3 colorNW = tex.Sample(smp, posNW).rgb;

    // サンプリング点と四近傍の輝度を計算
    float lumaM = CalcLuma(colorM);
    float lumaNE = CalcLuma(colorNE);
    float lumaNW = CalcLuma(colorNW);
    float lumaSE = CalcLuma(colorSE);
    float lumaSW = CalcLuma(colorSW);

    // 輝度差を算出
    float lumaMin = min(lumaM, min(min(lumaNE, lumaNW), min(lumaSE, lumaSW)));
    float lumaMax = max(lumaM, max(max(lumaNE, lumaNW), max(lumaSE, lumaSW)));
    

    // 輝度差からエッジベクトルを算出
    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    // エッジベクトルを用いてサンプリング位置調整
    float dirReduce = max(
    (lumaNW + lumaNE + lumaSE + lumaSW) * 0.25 * FXAA_REDUCE_MUL,
        FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
        max(float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * rcpFrame;

    float4 color = float4(0, 0, 0, 1);
    
    //// 中点に位置補正をかけてサンプリング
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
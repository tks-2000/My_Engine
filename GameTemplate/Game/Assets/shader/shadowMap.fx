/*!
 * @brief シャドウマップ描画用のシェーダー
 */

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
    int4 Indices    : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION; // モデルの頂点座標
    float3 normal : NORMAL;//法線
    float2 uv : TEXCOORD0; //UV座標
    SSkinVSIn skinVert;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_shadowMap : register(t10); //シャドウマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);
sampler g_sampler : register(s0);


float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダー
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	
	psIn.normal = mul(m, vsIn.normal); // 法線を回転させる。
	psIn.uv = vsIn.uv;

	return psIn;
}
SPSIn VSMain(SVSIn vsIn)
{
    // SPSIn psIn;
    // psIn.pos = mul(mWorld, vsIn.pos);
    // psIn.pos = mul(mView, psIn.pos);
    // psIn.pos = mul(mProj, psIn.pos);
    // psIn.uv = vsIn.uv;
    // psIn.normal = mul(mWorld,vsIn.normal);
    // return psIn;
    return VSMainCore(vsIn,false);
}

SPSIn VSSkinMain(SVSIn vsIn)
{
    // SPSIn psIn;
    // float4x4 skinning;
    // float w = 0.0f;
    // [unroll]
    // for(int i = 0; i < 3; i++)
    // {
    //     skinning += g_boneMatrix[skinVert.Indices[i]*skinVert.Weights[i]];
    //     w+=SVSIn.skinVert.Weights[i];

    // }
    // skinning += g_boneMatrix[SVSIn.skinVert.Indices[3]] * (1.0f-w);
    // psIn.pos = mul(skinning, vsIn.pos);
    // psIn.pos = mul(mView, psIn.pos);
    // psIn.pos = mul(mProj, psIn.pos);
    // psIn.uv = vsIn.uv;
    // psIn.normal = mul(skinning,vsIn.normal);
    // return psIn;
    return VSMainCore(vsIn,true);

}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // step-5 シャドウマップ描画用のピクセルシェーダーを作成する
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}

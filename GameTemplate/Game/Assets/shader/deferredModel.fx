/*!
 * @brief	シンプルなモデルシェーダー。
 */
 ///////////////////////////////////////////////
 //定数
 ///////////////////////////////////////////////
static const float PI = 3.1415926f;
////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	//float4 metallicAndSmooth : TEXCOORD1;
	float3 worldPos 	: TEXCOORD1;
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線
	float4 posInLVP		: TEXCOORD3;	//ライトビュースクリーン空間でのピクセルの座標
	float distToEye : TEXCOORD4;
};
//ピクセルシェーダーからの出力
struct SPSOut
{
	float4 albedo : SV_Target0;
	float3 normal : SV_Target1;
	float4 metallicAndSmooth : SV_Target2;
	float3 worldPos : SV_Target3;
	float3 normalInView : SV_Target4;
	float4 posInLVP : SV_Target5;
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer shadowCb : register(b1)
{
	float4x4 mLVP;

};

cbuffer ditheringCb : register(b2)
{
	float ditherWeight;
}

////////////////////////////////////////////////
//関数宣言
////////////////////////////////////////////////


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_texture : register(t0);				//アルベドマップ
//Texture2D<float4> g_shadowMap : register(t10);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

//ティザパターン
static const int pattern[4][4] = {
	{0,32,8,40},
	{48,16,56,24},
	{12,44,4,36},
	{60,28,52,20}
};

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
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
/// 頂点シェーダーのコア関数。
/// </summary>
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
	float4 worldPos = mul(m,vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = mul(m, vsIn.normal); // 法線を回転させる。
	psIn.normal = normalize(psIn.normal);
	psIn.uv = vsIn.uv;

	psIn.normalInView = normalize(mul(mView,psIn.normal));	//カメラ空間の法線を求める
	psIn.posInLVP = mul(mLVP,worldPos);

	
	psIn.distToEye = length(psIn.pos);

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
SPSOut PSMain( SPSIn psIn )
{
	int x = (int)fmod(psIn.pos.x,4.0f);
	int y = (int)fmod(psIn.pos.y,4.0f);
	
	int dither = pattern[y][x];

	float clipRange = 300.0f;

	float eyeToClipRange = max(0.0f,psIn.distToEye - clipRange);

	float clipRate = 1.0f - min(1.0f,eyeToClipRange / 100.0f);

	clip(dither - 64 * ditherWeight);

	SPSOut psOut;
	
	psOut.albedo = g_texture.Sample(g_sampler,psIn.uv);
	psOut.normal = (psIn.normal/2.0f) + 0.5f;
	psOut.metallicAndSmooth = float4(0.0f,0.0f,0.0f,0.0f);
	psOut.worldPos = psIn.worldPos;
	psOut.normalInView = (psIn.normalInView/2.0f) + 0.5f;
	

	psOut.posInLVP = psIn.posInLVP;

	return psOut;
}
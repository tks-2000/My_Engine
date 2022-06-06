/*!
 * @brief	シンプルなモデルシェーダー。
 */
 ///////////////////////////////////////////////
 //定数
 ///////////////////////////////////////////////
static const int NUM_DIRECTION_LIGHT = 4;
static const int NUM_SPOT_LIGHT = 4;
static const int NUM_POINT_LIGHT = 5;
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
	float3 worldPos 	: TEXCOORD1;
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線
	float4 posInLVP		: TEXCOORD3;	//ライトビュースクリーン空間でのピクセルの座標
};

struct LightNum
{
	int dLigNum;
	int pLigNum;
	int sLigNum;
};

//ディレクションライト
struct DirectionLight
{
	float3 direction;	//方向
	float3 color;		//カラー
};

//ポイントライト
struct PointLight
{
	float3 position;	//位置
	float3 color;		//カラー
	float Range;		//影響範囲

};

//スポットライト
struct SpotLight
{
	float3 position;	//位置
	float3 color;		//カラー
	float Range;		//射出範囲
	float3 direction;	//射出方向
	float angle;		//射出角度
};

//半球ライト
struct HemiSphereLight
{
	float3 groundColor;		//照り返しのライト
	float3 skyColor;		//天球ライト
	float3 groundNormal;	//地面の法線
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

cbuffer LightCb : register(b1)
{
	//LightNum ligNums;
	DirectionLight directionLight[NUM_DIRECTION_LIGHT];		//ディレクションライト
	PointLight pointLight[NUM_POINT_LIGHT];				//ポイントライト
	SpotLight spotLight[NUM_SPOT_LIGHT];				//スポットライト
	HemiSphereLight hemiSphereLight;	//半球ライト
	float3 eyePos;						//視点の位置
	float3 ambientLight;				//環境光
};

cbuffer shadowCb : register(b2)
{
	float4x4 mLVP;
}

////////////////////////////////////////////////
//関数宣言
////////////////////////////////////////////////
//ランバート拡散反射を計算する
float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
//フォン鏡面反射を計算する
float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos);
//影響率を計算
float CalculateImpactRate(float3 ligPos, float ligRange, float3 worldPos);
//ディレクションライトを計算
float3 CalculateDirectionLight(DirectionLight dirLig,float3 normal,float3 worldPos,float3 toEye);
//ポイントライトを計算
float3 CalculatePointLight(PointLight ptLig, float3 normal, float3 worldPos, float3 toEye);
//スポットライトを計算
float3 CalculateSpotLight(SpotLight spLig, float3 normal, float3 worldPos, float3 toEye);
//リムライトを計算
float3 CalculateRimlight(float3 lightDirection, float3 lightColor, float3 normal, float3 toEye);
//半球ライトを計算
float3 CalculateHemiSphereLight(HemiSphereLight hemLig, float3 normal);
float3 CalculateShadow(float4 psLvp);

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_shadowMap : register(t10);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

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

	psIn.normalInView = mul(mView,psIn.normal);	//カメラ空間の法線を求める
	psIn.posInLVP = mul(mLVP,worldPos);

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
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	float3 toEye = normalize(eyePos - psIn.worldPos);


	///////////////////////////////////////////////////////////////////////////////////

	//ここからディレクションライトの計算を行う

	float3 directionLigColor = {0.0f,0.0f,0.0f};

    for(int dirLigNum = 0; dirLigNum < NUM_DIRECTION_LIGHT; dirLigNum++){
		directionLigColor += CalculateDirectionLight(
			directionLight[dirLigNum],
			psIn.normal,
			psIn.worldPos,
		    toEye
		);
	}

	///////////////////////////////////////////////////////////////////////////////////

	//ここからポイントライトの計算を行う
    //全てのポイントライトの計算結果をまとめる物を用意
    float3 pointLigColor = {0.0f,0.0f,0.0f};

    //ポイントライトの数だけ計算を行う
    for(int ptNum = 0; ptNum < NUM_POINT_LIGHT; ptNum++){
	    pointLigColor += CalculatePointLight(
			pointLight[ptNum],
			psIn.normal,
			psIn.worldPos,
			toEye
		);
    }

	///////////////////////////////////////////////////////////////////////////////////

	//ここからスポットライトの計算を行う
    //全てのスポットライトの計算結果をまとめる物を用意
    float3 spotLigColor = {0.0f,0.0f,0.0f};

    //スポットライトの数だけ計算を行う
    for(int spLigNo = 0; spLigNo < NUM_SPOT_LIGHT; spLigNo++){
	    spotLigColor += CalculateSpotLight(
			spotLight[spLigNo],
			psIn.normal,
			psIn.worldPos,
			toEye
		);
    }

	///////////////////////////////////////////////////////////////////////////////////

	//ここから半球ライトの計算を行う
    float3 hemiSphereLigColor = CalculateHemiSphereLight(hemiSphereLight,psIn.normal);

    ///////////////////////////////////////////////////////////////////////////


	//全てのライトの計算結果を加算
	float3 finalLigColor = directionLigColor + pointLigColor + spotLigColor + ambientLight + hemiSphereLigColor;
	
	//最終的なカラーを確定。
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	//テクスチャカラーに求めた光を乗算して最終出力カラーを求める。
	albedoColor.xyz *= finalLigColor;

    // 【注目】ライトビュースクリーン空間からUV座標空間に変換している
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

	//ライトビュースクリーン空間でのz値を計算する
	float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;

    //UV座標を使ってシャドウマップから影情報をサンプリング
    
    if(shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
		&& zInLVP < 1.0f && zInLVP > 0.1f)
    {
        float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
		if(zInLVP >= zInShadowMap + 0.00003f)
		{
			
			albedoColor.xyz *= 0.5f;
		}
    }

    // テクスチャカラーにシャドウマップからサンプリングした情報を掛け算する
    // 影が描き込まれていたら0.5になるので、色味が落ちて影っぽくなる
    //albedoColor.xyz *= shadowMap;

	return albedoColor;
}

float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する。
	float t = dot(normal,lightDirection);

	//内積の結果に-1を乗算する。
	t *= -1.0f;

	//内積の結果が0以下なら0にする。
	t = max(0.0f, t);

	//拡散反射光を求める。
	return lightColor * t;
}

float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos)
{
//反射ベクトルを求める。
	float3 refVec = reflect(lightDirection,normal);

	//光が当たったサーフェイスから視点に伸びるベクトルを求める。
	float3 toEye = eyePos - worldPos;
	//正規化する。
	toEye = normalize(toEye);

	//鏡面反射の強さを求める。
	//dot関数を利用してrefVecとtoEyeの内積を求める。
	float t = dot(refVec,toEye);
	//内積の結果はマイナスになるので、マイナスの場合は0にする。
	t = max(0.0f, t);

	//鏡面反射の強さを絞る。
	t = pow(t,5.0f);

	//鏡面反射光を求める。
	return lightColor * t;
}

float CalculateImpactRate(float3 ligPos, float ligRange, float3 worldPos)
{
	//距離による影響率を計算する。
	//ポイントライトとの距離を計算する。
	float3 lDistance = length(worldPos - ligPos);

	//影響率は距離に比例して小さくなっていく。
	float lAffect = 1.0f - 1.0f / ligRange * lDistance;

	//影響力がマイナスにならないように補正をかける。
	lAffect = max(0.0, lAffect);

	//影響を指数関数的にする。
	lAffect = pow(lAffect,3.0f);
	return lAffect;
}

float3 CalculateDirectionLight(DirectionLight dirLig,float3 normal,float3 worldPos,float3 toEye)
{
	 //拡散反射を計算
   float3 diffuseColor = CalculateLambertDiffuse(
	   dirLig.direction,
	   dirLig.color,
	   normal
   );

   //鏡面反射を計算
   float3 specularColor = CalculatePhoneSpecular(
	   dirLig.direction,
	   dirLig.color,
	   normal,
	   worldPos
   );

   float3 dirLimColor =  CalculateRimlight(
		dirLig.direction,
		dirLig.color,
		normal,
		toEye
	);

   //拡散反射・鏡面反射を加算して最終的のディレクションライトのカラーを求める
   float3 dirLigColor = diffuseColor + specularColor + dirLimColor;

   return dirLigColor;

}

float3 CalculatePointLight(PointLight ptLig, float3 normal, float3 worldPos, float3 toEye)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 ligDir = worldPos - ptLig.position;
	//正規化する。
	ligDir = normalize(ligDir);

	//減衰なしのランバート拡散反射光を計算する。
	float3 diffPoint =  CalculateLambertDiffuse(
		ligDir,
		ptLig.color,
		normal
	);

	//減衰なしのフォン鏡面反射光を計算する。
	float3 specPoint = CalculatePhoneSpecular(
		ligDir,
		ptLig.color,
		normal,
		worldPos
	);

	//距離による影響率を計算する。
	float pAffect = CalculateImpactRate(
		ptLig.position,
		ptLig.Range,
		worldPos
	);

	float3 rimPoint = CalculateRimlight(
		ligDir,
		ptLig.color,
		normal,
		toEye
	);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	diffPoint *= pAffect;
	specPoint *= pAffect;
	rimPoint *= pAffect;

	float3 ptLigColor = diffPoint + specPoint + rimPoint;

	return ptLigColor;
}

float3 CalculateSpotLight(SpotLight spLig, float3 normal, float3 worldPos, float3 toEye)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 ligDir = worldPos - spLig.position;
	//正規化する。
	ligDir = normalize(ligDir);

	//減衰なしのランバート拡散反射光を計算する。
	float3 diffSpot =  CalculateLambertDiffuse(
		ligDir,
		spLig.color,
		normal
	);

	//減衰なしのフォン鏡面反射光を計算する。
	float3 specSpot = CalculatePhoneSpecular(
		ligDir,
		spLig.color,
		normal,
		worldPos
	);

	//距離による影響率を計算する。
	float pAffect = CalculateImpactRate(
		spLig.position,
		spLig.Range,
		worldPos
	);

	float3 rimSpot = CalculateRimlight(
		ligDir,
		spLig.color,
		normal,
		toEye
	);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	diffSpot *= pAffect;
	specSpot *= pAffect;
	rimSpot *= pAffect;

	//入射光と射出方向の角度を求める。
	//dot()を利用して内積を求める。
	float sAngle = dot(ligDir,spLig.direction);
	//dot()で求めた値をacosに渡して角度を求める。
	sAngle = acos(sAngle);

	//角度による影響率を求める。
	//角度に比例して小さくなっていく影響率を計算する。
	float sAffect = 1.0f - 1.0f / spLig.angle * sAngle;
	//影響力がマイナスにならないように補正をかける。
	sAffect = max(0.0f,sAffect);
	//影響の仕方を指数関数的にする。
	sAffect = pow(sAffect,0.5f);

	diffSpot *= sAffect;
	specSpot *= sAffect;
	rimSpot *= sAffect;

	float3 spLigColor = diffSpot + specSpot + rimSpot;

	return spLigColor;
}



float3 CalculateRimlight(float3 lightDirection, float3 lightColor, float3 normal, float3 toEye)
{
	//法線とライトの方向の内積でリムの強さを求める
	float power1 = dot(normal,lightDirection);
	//内積の結果を2乗し結果を1~0の値にする
	power1 = pow(power1,2.0f);
	//1から2乗した内積の結果を引いた値をリムの強さにする
	//内積の結果が0に近い(二つのベクトルが直角に近い)ほどリムの強さが上がる
	power1 = 1.0f - power1;

	//法線とカメラへの方向の内積でリムの強さを求める
	float power2 = dot(normal,toEye);
	//内積の結果を2乗し結果を1~0の値にする
	power2 = pow(power2,2.0f);
	//1から2乗した内積の結果を引いた値をリムの強さにする
	//内積の結果が0に近い(二つのベクトルが直角に近い)ほどリムの強さが上がる
	power2 = 1.0f - power2;
	
	//ライトの方向とカメラへの方向の内積でリムの強さを求める
	float power3 = dot(lightDirection,toEye);
	//内積の結果は1~-1の間なのでリムの強さがマイナスにならないように最低でも0にする
	power3 = max(0.0f,power3);

	//最終的なリムの強さを求める。
	float limPower = power1 * power2 * power3;

	//pow()を使用して、強さの変化を指数関数的にしてリムの発生範囲を絞る。
	limPower = pow(limPower,10.0f);

	//リムライトのカラーを計算する。
	float3 limColor = limPower * lightColor;

	return limColor;
}

float3 CalculateHemiSphereLight(HemiSphereLight hemLig, float3 normal)
{
	//半球ライトを計算する。
	//サーフェイスの法線と地面の法線との内積を計算する。
	float hLigT = dot(normal,hemLig.groundNormal);

	//内積の結果を0~1の範囲に変換する。
	hLigT = (hLigT + 1.0f) / 2.0f;

	//地面色と天球色を補完率hLigTで線形補完する。
	float3 hemLigColor = lerp(hemLig.groundColor,hemLig.skyColor,hLigT);

	//float3 hemLigColor = {0.0f,0.0f,0.0f};

	//半球ライトを最終的な反射光に加算する。
	return hemLigColor;
}
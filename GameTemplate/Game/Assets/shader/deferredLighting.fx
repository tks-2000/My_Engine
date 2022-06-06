
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

struct VSInput
{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

cbuffer Cb : register(b0){
	float4x4 mvp;
	float4 mulColor;
};

cbuffer LightCb : register(b1)
{
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

};


////////////////////////////////////////////////
//関数宣言
////////////////////////////////////////////////
float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos);


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);				//アルベドマップ
Texture2D<float4> g_normalTexture : register(t1);
Texture2D<float4> g_metaricSmoothMap : register(t2);
Texture2D<float4> g_worldPosTexture : register(t3);
Texture2D<float4> g_normalInViewTexture : register(t4);
Texture2D<float4> g_posInLVP : register(t5);
Texture2D<float4> g_shadowMap : register(t6);
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
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


PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(PSInput In) : SV_Target0
{
	//G-BUfferの内容を取り出す
	float4 albedo = g_albedoTexture.Sample(g_sampler,In.uv);
	float3 normal = g_normalTexture.Sample(g_sampler,In.uv).xyz;
	float metallic = g_metaricSmoothMap.Sample(g_sampler,In.uv).x;
	float smooth = g_metaricSmoothMap.Sample(g_sampler,In.uv).w;
	float3 worldPos = g_worldPosTexture.Sample(g_sampler,In.uv).xyz;
	float3 normalInView = g_normalInViewTexture.Sample(g_sampler,In.uv).xyz;
	float4 shadowMap = g_shadowMap.Sample(g_sampler,In.uv);
	normal = (normal * 2.0f)-1.0f;
	normalInView = (normalInView * 2.0f)-1.0f;
	float4 posInLVP = g_posInLVP.Sample(g_sampler,In.uv);
	float3 toEye = normalize(eyePos - worldPos);

   ///////////////////////////////////////////////////////////////////////////

   //ここからディレクションライトの計算を行う

   float3 directionLigColor = {0.0f,0.0f,0.0f};

   for(int dirLigNum = 0; dirLigNum < NUM_DIRECTION_LIGHT; dirLigNum++){
	   directionLigColor += CalculateDirectionLight(
		   directionLight[dirLigNum],
		   normal,
		   worldPos,
		   toEye
	   );
   }

   ///////////////////////////////////////////////////////////////////////////

   //ここからポイントライトの計算を行う
   //全てのポイントライトの計算結果をまとめる物を用意
   float3 pointLigColor = {0.0f,0.0f,0.0f};

   //ポイントライトの数だけ計算を行う
   for(int ptNum = 0; ptNum < NUM_POINT_LIGHT; ptNum++){
	   pointLigColor += CalculatePointLight(pointLight[ptNum],normal,worldPos,toEye);
   }

   ///////////////////////////////////////////////////////////////////////////

   //ここからスポットライトの計算を行う
   //全てのスポットライトの計算結果をまとめる物を用意
   float3 spotLigColor = {0.0f,0.0f,0.0f};

   //スポットライトの数だけ計算を行う
   for(int spLigNo = 0; spLigNo < NUM_SPOT_LIGHT; spLigNo++){
	   spotLigColor += CalculateSpotLight(spotLight[spLigNo],normal,worldPos,toEye);
   }

   ///////////////////////////////////////////////////////////////////////////

   //ここから半球ライトの計算を行う
   float3 hemiSphereLigColor = CalculateHemiSphereLight(hemiSphereLight,normal);

   ///////////////////////////////////////////////////////////////////////////


   


   //全てのライティング結果を加算して最終的なカラーを求める
   float3 finalLigColor = directionLigColor + pointLigColor + spotLigColor + ambientLight + hemiSphereLigColor;

   float4 finalColor = albedo;

   //アルベドカラーにライトのカラーを乗算し最終出力カラーを確定
   finalColor.xyz *= finalLigColor;

   // 【注目】ライトビュースクリーン空間からUV座標空間に変換している
   float2 shadowMapUV = posInLVP.xy / posInLVP.w;
   shadowMapUV *= float2(0.5f, -0.5f);
   shadowMapUV += 0.5f;
   //ライトビュースクリーン空間でのz値を計算する
   float zInLVP = posInLVP.z / posInLVP.w;
   
   float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
   //UV座標を使ってシャドウマップから影情報をサンプリング 
   if(shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
		&& zInLVP < 1.0f && zInLVP > 0.1f) {
			
			if(zInLVP >= zInShadowMap + 0.00003f){
				 finalColor.xyz *= 0.5f;
			}
	}

   return finalColor;
}



float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する。
	float t = dot(normal,lightDirection);

	//内積の結果に-1を乗算する。
	t *= -1.0f;

	//内積の結果が0以下なら0にする。
	t = max(0.0f, t);

	float diffColor = lightColor * t;

	//拡散反射光を求める。
	return diffColor / PI;
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
	power3 /= 2.0f;

	power3 += 0.5f;

	power3 *= power3;

	//最終的なリムの強さを求める。
	float limPower = power1 * power2 * power3;

	//pow()を使用して、強さの変化を指数関数的にしてリムの発生範囲を絞る。
	limPower = pow(limPower,5.0f);

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
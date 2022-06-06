
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
Texture2D<float4> g_metallicSmoothMap : register(t2);
Texture2D<float4> g_worldPosTexture : register(t3);
Texture2D<float4> g_normalInViewTexture : register(t4);
Texture2D<float4> g_posInLVP : register(t5);
Texture2D<float4> g_shadowMap : register(t6);
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
//フレネル反射を考慮した拡散反射を計算
float CalcDiffuseFromFresnel(float3 normal,float3 reverseLigDir,float3 toEye,float smooth);
//ランバート拡散反射を計算
float3 CalculateLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
//クックトランスモデルの鏡面反射を計算
float CookTorranceSpecular(float3 reverseLigDir,float3 toEye,float3 normal,float metallic);
float3 CalculatePhoneSpecular(float3 lightDirection, float3 lightColor, float3 normal, float3 worldPos);
float3 CalculateHemiSphereLight(HemiSphereLight hemLig, float3 normal);
float3 CalculateShadow(float4 psLvp);

//拡散反射光を計算
float3 CalculateDiffuseLight(
	float3 ligColor,
	float3 ligDirection,
	float3 normal,
	float3 toEye,
	float smooth,
	float3 albedoColor
);
//鏡面反射光を計算
float3 CalculateSpecularLight(
	float3 ligColor,
	float3 ligDirection,
	float3 normal, 
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
);
//リムライトを計算
float3 CalculateRimlight(
	float3 lightColor,
	float3 lightDirection, 
	float3 normal, 
	float3 toEye
);
//距離による影響率を計算
float CalculateImpactRate(float3 ligPos, float ligRange, float3 worldPos);
//ディレクションライトを計算
float3 CalculateDirectionLight(
	DirectionLight dirLig,
	float3 normal,
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
);
//ポイントライトを計算
float3 CalculatePointLight(
	PointLight ptLig,
	float3 normal, 
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
);
//スポットライトを計算
float3 CalculateSpotLight(
	SpotLight spLig, 
	float3 normal, 
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
);

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f-t2)/ t2);
    return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1-f0) * pow(1-u, 5);
}


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
	float metallic = g_metallicSmoothMap.Sample(g_sampler,In.uv).r;
	float smooth = g_metallicSmoothMap.Sample(g_sampler,In.uv).a;
	float3 worldPos = g_worldPosTexture.Sample(g_sampler,In.uv).xyz;
	float3 normalInView = g_normalInViewTexture.Sample(g_sampler,In.uv).xyz;
	float4 shadowMap = g_shadowMap.Sample(g_sampler,In.uv);
	normal = (normal * 2.0f)-1.0f;
	normalInView = (normalInView * 2.0f)-1.0f;
	float4 posInLVP = g_posInLVP.Sample(g_sampler,In.uv);
	float3 toEye = normalize(eyePos - worldPos);

   ///////////////////////////////////////////////////////////////////////////

   //ここからディレクションライトの計算を行う

   //拡散反射を計算

   float3 directionLigColor = {0.0f,0.0f,0.0f};

   for(int dirLigNum = 0; dirLigNum < NUM_DIRECTION_LIGHT; dirLigNum++){
	   directionLigColor += CalculateDirectionLight(
		   directionLight[dirLigNum],
		   normal,
		   worldPos,
		   toEye,
		   metallic,
		   smooth,
		   albedo.xyz
	   );
   }
   ///////////////////////////////////////////////////////////////////////////

   //ここからポイントライトの計算を行う
   //全てのポイントライトの計算結果をまとめる物を用意
   float3 pointLigColor = {0.0f,0.0f,0.0f};

   //ポイントライトの数だけ計算を行う
   for(int ptNum = 0; ptNum < NUM_POINT_LIGHT; ptNum++){
	   pointLigColor += CalculatePointLight(
		   pointLight[ptNum],
		   normal,
		   worldPos,
		   toEye,
		   metallic,
		   smooth,
		   albedo.xyz
		);
   }

   ///////////////////////////////////////////////////////////////////////////

   //ここからスポットライトの計算を行う
   //全てのスポットライトの計算結果をまとめる物を用意
   float3 spotLigColor = {0.0f,0.0f,0.0f};

   //スポットライトの数だけ計算を行う
   for(int spLigNo = 0; spLigNo < NUM_SPOT_LIGHT; spLigNo++){
	   spotLigColor += CalculateSpotLight(
		   spotLight[spLigNo],
		   normal,
		   worldPos,
		   toEye,
		   metallic,
		   smooth,
		   albedo.xyz
		);
   }

   ///////////////////////////////////////////////////////////////////////////

   //ここから半球ライトの計算を行う
   float3 hemiSphereLigColor = CalculateHemiSphereLight(hemiSphereLight,normal) * albedo.xyz;

   ///////////////////////////////////////////////////////////////////////////


   


   //全てのライティング結果を加算して最終的なカラーを求める
   float3 finalLigColor = 0.0f;

   float3 ambientColor = ambientLight * albedo;
   
   finalLigColor += directionLigColor +  pointLigColor + spotLigColor + hemiSphereLigColor + ambientColor;

   float4 finalColor = 1.0f;

   //アルベドカラーにライトのカラーを加算し最終出力カラーを確定
   finalColor.xyz = finalLigColor;

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
			
			if(zInLVP >= zInShadowMap + 0.0003f){
				 finalColor.xyz *= 0.5f;
			}
	}

   return finalColor;
}

float CalcDiffuseFromFresnel(float3 normal,float3 reverseLigDir,float3 toEye,float smooth)
{
	float3 halfVec = normalize(reverseLigDir+toEye);

	//float roughness = 0.0f;

	float energyBias = lerp(0.0f,0.5f,smooth);
	float energyFactor = lerp(1.0,1.0/1.51,smooth);

	float ligDotHalf = saturate(dot(reverseLigDir,halfVec));

	float Fd90 = energyBias + 2.0 * ligDotHalf * ligDotHalf * smooth;

	float normalDotLig = saturate(dot(normal,reverseLigDir));
	float FL = (1 + (Fd90 - 1) * pow(1 - normalDotLig, 5));

	float normalDotToEye = saturate(dot(normal,toEye));
	float FV = (1 + (Fd90 - 1) * pow(1 - normalDotToEye, 5));

	return (FL*FV*energyFactor);

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
	float3 diffColor = lightColor * t;

	//正規化ランバート拡散反射を返す
	return diffColor / PI;
}

float CookTorranceSpecular(float3 reverseLigDir, float3 toEye, float3 normal, float metallic)
{
	float microfacet = 0.76f;

	float f0 = metallic;

	float3 halfVec = normalize(reverseLigDir+toEye);

	float normalDotHalf = saturate(dot(normal,halfVec));
	float toEyeDotHalf = saturate(dot(toEye,halfVec));
	float normalDotReverseLig = saturate(dot(normal,reverseLigDir));
	float normalDotToEye = saturate(dot(normal,toEye));

	float D = Beckmann(microfacet,normalDotHalf);

	float F = SpcFresnel(f0,toEyeDotHalf);

	float G = min(1.0f,min(2*normalDotHalf*normalDotToEye/toEyeDotHalf,2*normalDotHalf*normalDotReverseLig/toEyeDotHalf));

	float m = PI * normalDotToEye * normalDotHalf;

	return max(F * D * G / m, 0.0f);
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

float3 CalculateHemiSphereLight(HemiSphereLight hemLig, float3 normal)
{
	//半球ライトを計算する。
	//サーフェイスの法線と地面の法線との内積を計算する。
	float hLigT = dot(normal,hemLig.groundNormal);

	//内積の結果を0~1の範囲に変換する。
	hLigT = (hLigT + 1.0f) / 2.0f;

	//地面色と天球色を補完率hLigTで線形補完する。
	float3 hemLigColor = lerp(hemLig.groundColor,hemLig.skyColor,hLigT);


	//最終的な半球ライトのカラーを返す
	return hemLigColor;
}

float3 CalculateDiffuseLight(
	float3 ligColor,
	float3 ligDirection,
	float3 normal,
	float3 toEye,
	float smooth,
	float3 albedoColor
)
{
	float3 lambartDiffuseColor = CalculateLambertDiffuse(
	   ligDirection,
	   ligColor,
	   normal
   );

   float diffuseFromFresnel = CalcDiffuseFromFresnel(normal,-ligDirection,toEye,smooth);

   float3 diffuseColor = albedoColor * lambartDiffuseColor * diffuseFromFresnel;

   return diffuseColor;
}

float3 CalculateSpecularLight(
	float3 ligColor,
	float3 ligDirection,
	float3 normal,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
)
{
	float3 specularColor = CookTorranceSpecular(
	   -ligDirection,
	   toEye,
	   normal,
	   smooth
	) * ligColor;

	specularColor *= lerp(ligColor,albedoColor,metallic);

	return specularColor;
}

float3 CalculateRimlight(
	float3 lightColor, 
	float3 lightDirection, 
	float3 normal, 
	float3 toEye
)
{
	//法線とライトの方向の内積でリムの強さを求める
	float power1 = dot(normal,lightDirection);
	//内積の結果を2乗し結果を1~0の値にする
	power1 *= power1;
	//1から2乗した内積の結果を引いた値をリムの強さにする
	//内積の結果が0に近い(二つのベクトルが直角に近い)ほどリムの強さが上がる
	power1 = 1.0f - power1;

	//法線とカメラへの方向の内積でリムの強さを求める
	float power2 = dot(normal,toEye);
	//内積の結果を2乗し結果を1~0の値にする
	power2 *= power2;
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
	limPower = pow(limPower,6.0f);

	//リムライトのカラーを計算する。
	float3 limColor = limPower * lightColor;

	return limColor;
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

float3 CalculateDirectionLight(
	DirectionLight dirLig,
	float3 normal,
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
)
{
	 //拡散反射を計算
   float3 diffuseColor = CalculateDiffuseLight(
	   dirLig.color,
	   dirLig.direction,
	   normal,
	   toEye,
	   smooth,
	   albedoColor
	);

	float3 specularColor = CalculateSpecularLight(
		dirLig.color,
		dirLig.direction,
		normal,
		toEye,
		metallic,
		smooth,
		albedoColor
	);

   float3 dirLimColor =  CalculateRimlight(
		dirLig.color,
		dirLig.direction,
		normal,
		toEye
	);

   //滑らかさを参照して拡散反射を求めて鏡面反射とリムライトを加算
   float3 dirLigColor = diffuseColor * (1.0f - smooth) + specularColor + dirLimColor;

   return dirLigColor;
}

float3 CalculatePointLight(
	PointLight ptLig,
	float3 normal,
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 ligDir = worldPos - ptLig.position;
	//正規化する。
	ligDir = normalize(ligDir);

	//減衰なしの拡散反射光を計算する。
	float3 diffPoint =  CalculateDiffuseLight(
		ptLig.color,
		ligDir,
		normal,
		toEye,
		smooth,
		albedoColor
	);

	//減衰なしの鏡面反射光を計算する。
	float3 specPoint = CalculateSpecularLight(
		ptLig.color,
		ligDir,
		normal,
		toEye,
		metallic,
		smooth,
		albedoColor
	);

	//減衰なしのリムライトを計算する
	float3 rimPoint = CalculateRimlight(
		ptLig.color,
		ligDir,
		normal,
		toEye
	);

	//距離による影響率を計算する。
	float pAffect = CalculateImpactRate(
		ptLig.position,
		ptLig.Range,
		worldPos
	);

	float3 ptLigColor = diffPoint * (1.0f - smooth) + specPoint + rimPoint;

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	ptLigColor *= pAffect;

	

	return ptLigColor;
}

float3 CalculateSpotLight(
	SpotLight spLig, 
	float3 normal, 
	float3 worldPos,
	float3 toEye,
	float metallic,
	float smooth,
	float3 albedoColor
)
{

	//サーフェイスに入射するポイントライトの光の向きを計算する。
	float3 ligDir = worldPos - spLig.position;
	//正規化する。
	ligDir = normalize(ligDir);

	//減衰なしの拡散反射光を計算する。
	float3 diffSpot =  CalculateDiffuseLight(
		spLig.color,
		ligDir,
		normal,
		toEye,
		smooth,
		albedoColor
	);

	//減衰なしの鏡面反射光を計算する。
	float3 specSpot = CalculateSpecularLight(
		spLig.color,
		ligDir,
		normal,
		toEye,
		metallic,
		smooth,
		albedoColor
	);

	float3 limSpot = CalculateRimlight(
		spLig.color,
		ligDir,
		normal,
		toEye
	);

	//距離による影響率を計算する。
	float pAffect = CalculateImpactRate(
		spLig.position,
		spLig.Range,
		worldPos
	);

	//入射光と射出方向の角度を求める。
	//dot()を利用して内積を求める。
	float sAngle = dot(ligDir,spLig.direction);

	if(-1.0f < sAngle && sAngle < 1.0f){
		//dot()で求めた値をacosに渡して角度を求める。
		sAngle = acos(sAngle);
	}
	else if(sAngle > 0.9f){
		sAngle = 0.0f;
	}
	else{
		sAngle = acos(-1.0f);
	}
	

	//角度による影響率を求める。
	//角度に比例して小さくなっていく影響率を計算する。
	float sAffect = 1.0f - 1.0f / spLig.angle * sAngle;
	//影響力がマイナスにならないように補正をかける。
	sAffect = max(0.0f,sAffect);
	//影響の仕方を指数関数的にする。
	sAffect = pow(sAffect,0.5f);

	float3 spLigColor = diffSpot * (1.0f - smooth) + specSpot + limSpot;

	spLigColor *=  pAffect * sAffect;

	return spLigColor;
}

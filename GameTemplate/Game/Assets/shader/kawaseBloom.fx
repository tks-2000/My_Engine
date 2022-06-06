/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> g_bokeTexture_0 : register(t0);	//�J���[�e�N�X�`���B
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{

	float4 combineColor = g_bokeTexture_0.Sample(Sampler,In.uv);
	combineColor += g_bokeTexture_1.Sample(Sampler,In.uv);
	combineColor += g_bokeTexture_2.Sample(Sampler,In.uv);
	combineColor += g_bokeTexture_3.Sample(Sampler,In.uv);
	combineColor /= 4.0;
	combineColor.a = 1.0f;

	return combineColor;
}
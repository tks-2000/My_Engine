#include "stdafx.h"
#include "Bloom.h"

namespace render {
	namespace postEffect {
		Bloom::Bloom()
		{
			for (int blurNum = 0; blurNum < GAUSSIAN_BLUR_NUM; blurNum++) {
				m_gaussianBlur[blurNum] = NewGO<GaussianBlur>(0);
			}


		}

		Bloom::~Bloom()
		{
			for (int blurNum = 0; blurNum < GAUSSIAN_BLUR_NUM; blurNum++) {
				DeleteGO(m_gaussianBlur[blurNum]);
			}
		}

		void Bloom::Init(RenderTarget* rt)
		{
			//輝度抽出用のスプライトを初期化
			m_luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
			m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
			m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
			m_luminanceSpriteInitData.m_width = rt->GetWidth();
			m_luminanceSpriteInitData.m_height = rt->GetHeight();
			m_luminanceSpriteInitData.m_textures[0] = &rt->GetRenderTargetTexture();
			m_luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_luminanceSprite.Init(m_luminanceSpriteInitData);

			//ブルームをかけるレンダリングターゲットのアドレスを記憶しておく
			m_mainRenderTargetAddress = rt;

			//輝度抽出用のレンダリングターゲット
			m_luminanceRenderTarget.Create(
				rt->GetWidth(),
				rt->GetHeight(),
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_D32_FLOAT
			);

			//ガウシアンブラーを初期化

			//輝度抽出のテクスチャを設定する
			m_gaussianBlur[0]->Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
			//残りは順番にブラーをかけたテクスチャを設定する
			m_gaussianBlur[1]->Init(&m_gaussianBlur[0]->GetBlurTexture());
			m_gaussianBlur[2]->Init(&m_gaussianBlur[1]->GetBlurTexture());
			m_gaussianBlur[3]->Init(&m_gaussianBlur[2]->GetBlurTexture());

			//ガウシアンブラー用のスプライトを初期化
			InitBlurSprite();
		}

		bool Bloom::Start()
		{
			return true;
		}

		void Bloom::Update()
		{

		}

		void Bloom::LuminanceExtraction(RenderContext& rc)
		{
			//輝度抽出用のレンダリングターゲットに輝度抽出スプライトを描画する 
			rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
			rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
			rc.ClearRenderTargetView(m_luminanceRenderTarget);
			m_luminanceSprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);
		}

		void Bloom::Execute(RenderContext& rc)
		{
			//輝度抽出を実行
			LuminanceExtraction(rc);

			//ガウシアンブラーを複数回実行
			for (int blurNum = 0; blurNum < GAUSSIAN_BLUR_NUM; blurNum++) {
				m_gaussianBlur[blurNum]->ExecuteOnGPU(rc, m_blurWeight);
			}

			//メインレンダリングターゲットにブラーのかかった輝度抽出テクスチャを加算合成する
			rc.WaitUntilToPossibleSetRenderTarget(*m_mainRenderTargetAddress);
			rc.SetRenderTargetAndViewport(*m_mainRenderTargetAddress);
			m_blurSprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(*m_mainRenderTargetAddress);

		}

		void Bloom::InitBlurSprite()
		{
			//ブラーをかけた状態のスプライトを作成する
			for (int blurNum = 0; blurNum < GAUSSIAN_BLUR_NUM; blurNum++) {
				m_blurSpriteInitData.m_textures[blurNum] = &m_gaussianBlur[blurNum]->GetBlurTexture();
			}

			m_blurSpriteInitData.m_width = 1280;
			m_blurSpriteInitData.m_height = 720;
			m_blurSpriteInitData.m_fxFilePath = "Assets/shader/kawaseBloom.fx";
			m_blurSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
			m_blurSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_blurSprite.Init(m_blurSpriteInitData);
		}
	}
}
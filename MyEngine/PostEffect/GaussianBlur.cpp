#include "stdafx.h"
#include "GaussianBlur.h"

namespace render {
	namespace postEffect {
		GaussianBlur::GaussianBlur()
		{

		}

		GaussianBlur::~GaussianBlur()
		{

		}

		bool GaussianBlur::Start()
		{
			return true;
		}

		void GaussianBlur::Update()
		{

		}

		void GaussianBlur::Init(Texture* originalTexture)
		{
			m_originalTexture = originalTexture;

			//レンダリングターゲットを初期化
			InitRenderTargets();
			//スプライトを初期化
			InitSprites();
		}

		void GaussianBlur::ExecuteOnGPU(RenderContext& rc, float blurPower)
		{
			UpdateWeightsTable(blurPower);

			rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
			rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
			rc.ClearRenderTargetView(m_xBlurRenderTarget);
			m_xBlurSprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

			rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
			rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
			rc.ClearRenderTargetView(m_yBlurRenderTarget);
			m_yBlurSprite.Draw(rc);
			rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);


		}

		void GaussianBlur::InitRenderTargets()
		{
			m_xBlurRenderTarget.Create(
				m_originalTexture->GetWidth() / 2,
				m_originalTexture->GetHeight(),
				1,
				1,
				m_originalTexture->GetFormat(),
				DXGI_FORMAT_D32_FLOAT
			);

			m_yBlurRenderTarget.Create(
				m_originalTexture->GetWidth() / 2,
				m_originalTexture->GetHeight() / 2,
				1,
				1,
				m_originalTexture->GetFormat(),
				DXGI_FORMAT_D32_FLOAT
			);
		}

		void GaussianBlur::InitSprites()
		{
			SpriteInitData xBlurSpriteInitData;
			xBlurSpriteInitData.m_fxFilePath = "Assets/shader/gaussianBlur.fx";
			xBlurSpriteInitData.m_vsEntryPointFunc = "VSXBlur";
			xBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";

			xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
			xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();

			xBlurSpriteInitData.m_textures[0] = m_originalTexture;

			xBlurSpriteInitData.m_expandConstantBuffer[0] = &m_weights;
			xBlurSpriteInitData.m_expandConstantBufferSize[0] = sizeof(m_weights);

			m_xBlurSprite.Init(xBlurSpriteInitData);

			SpriteInitData yBlurSpriteInitData;
			yBlurSpriteInitData.m_fxFilePath = "Assets/shader/gaussianBlur.fx";
			yBlurSpriteInitData.m_vsEntryPointFunc = "VSYBlur";
			yBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";

			yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
			yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();

			yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();

			yBlurSpriteInitData.m_expandConstantBuffer[0] = &m_weights;
			yBlurSpriteInitData.m_expandConstantBufferSize[0] = sizeof(m_weights);

			m_yBlurSprite.Init(yBlurSpriteInitData);
		}

		void GaussianBlur::UpdateWeightsTable(float blurPower)
		{
			float total = 0;
			for (int i = 0; i < NUM_WEIGHTS; i++) {
				m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
				total += 2.0f * m_weights[i];
			}

			for (int i = 0; i < NUM_WEIGHTS; i++) {
				m_weights[i] /= total;
			}
		}
	}
}
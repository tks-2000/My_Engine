#pragma once

namespace render {
	namespace postEffect {

		/// @brief ブルームをかけるクラス
		class Bloom : public IGameObject
		{
		public:
			Bloom();
			~Bloom();
			/// @brief ブルームを初期化
			/// @param rt ブルームをかけるレンダリングターゲットのアドレス
			void Init(RenderTarget* rt);
			bool Start();
			void Update();
			/// @brief ブルームを実行
			/// @param rc レンダリングコンテキスト
			void Execute(RenderContext& rc);
			/// @brief 輝度抽出を実行
			/// @param rc レンダリングコンテキスト
			void LuminanceExtraction(RenderContext& rc);
			/// @brief 輝度抽出したテクスチャを取得
			/// @return 輝度抽出されたテクスチャの参照
			Texture& GetLuminanceRenderTargetTexture() { return m_luminanceRenderTarget.GetRenderTargetTexture(); }
			/// @brief ブラーの重みを設定
			/// @param weight ブラーに設定する重み
			void SetBlurWeight(const float weight) { m_blurWeight = weight; }
		private:

			/// @brief ブラーをかけるスプライトを初期化
			void InitBlurSprite();

			/// @brief 輝度抽出用のレンダリングターゲット
			RenderTarget m_luminanceRenderTarget;
			/// @brief 輝度抽出用のスプライト
			Sprite m_luminanceSprite;
			/// @brief 輝度抽出用のスプライトの初期化情報
			SpriteInitData m_luminanceSpriteInitData;
			/// @brief ブラーをかけるレンダリングターゲットのアドレス
			RenderTarget* m_mainRenderTargetAddress = nullptr;
			/// @brief ガウシアンブラーをかける数
			enum { GAUSSIAN_BLUR_NUM = 4 };
			/// @brief ガウシアンブラー
			GaussianBlur* m_gaussianBlur[GAUSSIAN_BLUR_NUM] = { nullptr };
			/// @brief ブラーをかけるスプライト
			Sprite m_blurSprite;
			/// @brief ブラーをかけるスプライトの初期化情報
			SpriteInitData m_blurSpriteInitData;
			/// @brief ブラーの重み
			float m_blurWeight = 10.0f;
		};
	}
}
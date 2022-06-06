#pragma once


namespace render {
	namespace postEffect {
		class GaussianBlur;
		class Bloom;

		/// @brief ポストエフェクトをまとめたクラス
		class PostEffect : public IGameObject
		{
		public:
			PostEffect();
			~PostEffect();
			bool Start();
			void Update();
			void Render(RenderContext& rc);

			/// @brief ポストエフェクトを実行する
			/// @param rc レンダリングコンテキスト
			void Execute(RenderContext& rc);
			/// @brief ブラーをかける
			/// @param rt ブラーをかけるレンダリングターゲットのアドレス
			/// @param blurWeight ブラーの重み
			void SetBlur(RenderTarget* rt, float blurWeight = 20.0f);

			/// @brief ブラーの重みを設定
			/// @param blurWeight 設定するブラーの重み
			void SetBlurWeight(const float blurWeight) { m_blurWeight = blurWeight; }

			/// @brief ブルームをかける
			/// @param rt ブルームをかけるレンダリングターゲットのアドレス
			void SetBloom(RenderTarget* rt);

		private:
			/// @brief ブラー用のスプライト
			Sprite m_blurSprite;
			/// @brief ブラー用のスプライトの初期化情報
			SpriteInitData m_blurSpriteInitData;
			/// @brief ガウシアンブラー
			GaussianBlur* m_gaussianBlur = nullptr;
			/// @brief ガウシアンブラーの重み
			float m_blurWeight = 0.0f;
			/// @brief ブラーを行うフラグ
			bool m_blurFlag = false;
			/// @brief ブルーム
			Bloom* m_bloom = nullptr;
			/// @brief ブルームを行うフラグ
			bool m_bloomFlag = false;
			/// @brief メインレンダリングターゲットのアドレス
			RenderTarget* m_mainRenderTargetAddress = nullptr;
		};
	}
}
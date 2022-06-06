#pragma once

namespace render {
	namespace postEffect {
		/// @brief ガウシアンブラーを行うクラス
		class GaussianBlur : public IGameObject
		{
		public:
			GaussianBlur();
			~GaussianBlur();
			bool Start();
			void Update();
			/// @brief 初期化を行う
			/// @param originalTexture ガウシアンブラーをかけるテクスチャのアドレス
			void Init(Texture* originalTexture);

			/// @brief ガウシアンブラーを実行
			/// @param rc レンダリングコンテキスト
			/// @param blorPower ブラーの強さ
			void ExecuteOnGPU(RenderContext& rc, float blorPower);

			/// @brief ブラーをかけたテクスチャを取得
			/// @return ブラーが適用されたテクスチャ
			Texture& GetBlurTexture()
			{
				return m_yBlurRenderTarget.GetRenderTargetTexture();
			}
		private:
			//外部で使わないメンバ関数

			/// @brief レンダリングターゲットを初期化
			void InitRenderTargets();
			/// @brief スプライトを初期化
			void InitSprites();
			/// @brief 重みテーブルを更新する
			/// @param blurPower ブラーの重み
			void UpdateWeightsTable(float blurPower);

			//ここからメンバ変数

			/// @brief 重みの数
			enum { NUM_WEIGHTS = 8 };
			/// @brief 重みテーブル
			float m_weights[NUM_WEIGHTS];
			/// @brief オリジナルテクスチャ
			Texture* m_originalTexture = nullptr;
			/// @brief 横ブラー画像を描画するレンダリングターゲット
			RenderTarget m_xBlurRenderTarget;
			/// @brief 縦ブラー画像を描画するレンダリングターゲット
			RenderTarget m_yBlurRenderTarget;
			/// @brief 横ブラー画像を描画するためのスプライト
			Sprite m_xBlurSprite;
			/// @brief 縦ブラー画像を描画するためのスプライト
			Sprite m_yBlurSprite;
		};
	}
}
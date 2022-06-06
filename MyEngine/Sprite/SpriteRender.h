#pragma once

class RenderingEngine;

namespace render {
	namespace sprite {
		/// @brief 2D画像を表示するクラス
		class SpriteRender : public IGameObject
		{
		public:
			SpriteRender();
			~SpriteRender();
			/// @brief スプライトの初期化
			/// @param spriteFilePath 画像のddsファイルパス
			/// @param width 画像の横幅
			/// @param height 画像の縦幅
			void Init(const char* spriteFilePath, int width, int height);

			void InitTexture(Texture* texture, int width, int height);
			/// @brief スプライトの座標を設定
			/// @param pos スプライトに設定する座標
			void SetPosition(const Vector3& pos) { m_position = pos; }
			/// @brief スプライトの座標を入手
			/// @return スプライトの座標
			Vector3 GetPosition() { return m_position; }
			/// @brief スプライトの回転を設定
			/// @param qRot スプライトに設定するクォータニオン
			void SetRotation(Quaternion qRot) { m_qRot = qRot; }
			/// @brief スプライトの拡大率を設定
			/// @param scale スプライトに設定する拡大倍率
			void SetScale(Vector3 scale) { m_scale = scale; }
			/// @brief スプライトの中心を設定
			/// @param pivot スプライトに設定する中心位置
			void SetPivot(Vector2 pivot) { m_pivot = pivot; }
			/// @brief スプライトに乗算するカラーを設定
			/// @param color スプライトに乗算するカラー
			void SetColor(Vector4 color) { m_color = color; }
			/// @brief フェードの処理
			void Fade();
			/// @brief スプライトのフェードインを開始
			/// @param rate フェードのレート
			void FadeIn(float rate) { m_fadeInFlag = true; m_fadeOutFlag = false; m_fadeRate = rate; }
			/// @brief スプライトのフェードアウトを開始
			/// @param rate フェードのレート
			void FadeOut(float rate) { m_fadeOutFlag = true; m_fadeInFlag = false; m_fadeRate = rate; }
			/// @brief スプライトの不透明度を入手
			/// @return カラーのアルファ値
			float GetOpacity() { return m_color.w; }
			/// @brief スプライトが不透明か?
			/// @return trueなら不透明 falseなら透明か半透明
			bool IsOpacity();
			/// @brief スプライトが透明か？
			/// @return trueなら透明 falseなら不透明か半透明
			bool IsTransparent();
			/// @brief 画像を移動させる
			/// @param targetPos 移動させる座標
			/// @param velocity 移動させる速度
			void MoveStart(Vector3 targetPos, float velocity);
			/// @brief 実行
			void Execute();

		private:
			/// @brief 初期化フラグ
			bool m_isInitd = false;
			/// @brief スプライト
			Sprite m_sprite;
			/// @brief スプライトの初期化データ
			SpriteInitData m_spriteInitData;

			/// @brief スプライトの座標
			Vector3 m_position = Vector3::Zero;
			/// @brief スプライトの回転
			Quaternion m_qRot = Quaternion::Identity;
			/// @brief スプライトの拡大率
			Vector3 m_scale = Vector3::One;
			/// @brief スプライトの中心
			Vector2 m_pivot = { 0.5f,0.5f };
			/// @brief スプライトのカラー
			Vector4 m_color = Vector4::White;

			/// @brief 目標地点の座標
			Vector3 m_targetPos = Vector3::Zero;
			/// @brief 移動速度
			Vector3 m_moveSpeed = Vector3::Zero;
			/// @brief 移動計算前の自分の座標
			Vector3 m_previousPos = Vector3::Zero;

			/// @brief フェードインフラグ
			bool m_fadeInFlag = false;
			/// @brief フェードアウトフラグ
			bool m_fadeOutFlag = false;
			/// @brief フェードのレート
			float m_fadeRate = 0.0f;

			/// @brief 移動フラグ
			bool m_moveFlag = false;

			RenderingEngine* m_renderingEngine = nullptr;

			/// @brief 移動の処理
			void Move();
		};
	}
}

#pragma once

namespace render {
	namespace effect {
		class EffectRender : public IGameObject
		{
		public:
			EffectRender();
			~EffectRender();

			/// @brief 初期化
			/// @param filepath 再生するエフェクトのファイルパス
			void Init(const char16_t* filepath);
			/// @brief 実行
			void Execution();
			/// @brief 座標を設定
			/// @param pos 設定する座標
			void SetPosition(const Vector3& pos) { m_position = pos; }
			/// @brief 回転を設定
			/// @param qRot 設定する回転クォータニオン
			void SetRotation(const Quaternion& qRot) { m_qRot = qRot; }
			/// @brief 拡大率を設定
			/// @param scale 設定する拡大率
			void SetScale(const Vector3& scale) { m_scale = scale; }
			/// @brief エフェクトを再生
			/// @param isLoop ループ再生フラグ trueでループする falseで一回だけ
			/// @param isLayers 重ねて再生するフラグ trueで前のエフェクトが残っていても再生 falseで前のエフェクトが残っていると再生しない
			void Play(const bool isLoop, const bool isLayers = false);
			/// @brief エフェクトを連続再生 重ねて再生したい場合はこっち
			/// @param playbackInterval エフェクトを連続再生する間隔
			/// @param playbackTime 再生し続ける時間
			void ContinuousPlay(const float playbackInterval,const float playbackTime);
			/// @brief エフェクトを停止
			/// @param isEffectDelete 再生中のエフェクトを消すか？ trueで消す falseで終わるまで待つ
			void Stop(const bool isEffectDelete);

		private:
			/// @brief 通常再生の処理
			void Playing();
			/// @brief 連続再生の処理
			void ContinuousPlaying();

			/// @brief 初期化
			bool m_isInitd = false;
			/// @brief ワンショット再生の為の再生したかどうかのフラグ
			bool m_isPlay = true;
			/// @brief ループ再生フラグ
			bool m_isLoop = false;
			/// @brief 連続再生フラグ
			bool m_isContinuousPlayback = false;
			/// @brief 再生間隔
			float m_playbackInterval = 0.0f;
			/// @brief 再生間隔を計るタイマー
			float m_playbackIntervalTimer = 0.0f;
			/// @brief 再生し続ける時間
			float m_playbackTime = 0.0f;
			/// @brief 再生時間を計るタイマー
			float m_playbackTimer = 0.0f;
			/// @brief エフェクト
			Effect m_effect;
			/// @brief 座標
			Vector3 m_position = g_vec3Zero;
			/// @brief 回転
			Quaternion m_qRot = Quaternion::Identity;
			/// @brief 拡大率
			Vector3 m_scale = g_vec3One;

		};
	}
}

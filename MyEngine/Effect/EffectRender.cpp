#include "stdafx.h"
#include "EffectRender.h"

namespace render {
	namespace effect {
		EffectRender::EffectRender()
		{

		}

		EffectRender::~EffectRender()
		{

		}

		void EffectRender::Init(const char16_t* filepath)
		{
			//再生するエフェクトを初期化
			m_effect.Init(filepath);
			m_isInitd = true;
		}

		void EffectRender::Execution()
		{
			//未初期化なら実行しない
			if (m_isInitd == false) {
				return;
			}

			//座標・回転・拡大率を設定
			m_effect.SetPosition(m_position);
			m_effect.SetRotation(m_qRot);
			m_effect.SetScale(m_scale);
			m_effect.Update();

			//連続再生フラグが立っていたら…
			if (m_isContinuousPlayback == true) {
				//連続再生の処理を行う
				ContinuousPlaying();
			}
			//連続再生フラグがたっていなければ…
			else {
				//通常再生の処理を行う
				Playing();
			}
		}

		void EffectRender::Play(const bool isLoop, const bool isLayers)
		{
			//既に再生中なら実行しない
			if (m_effect.IsPlay() == true && isLayers == false) {
				return;
			}
			//再生フラグを設定
			m_isLoop = isLoop;
			m_isPlay = false;
		}

		void EffectRender::ContinuousPlay(const float playbackInterval, const float playbackTime)
		{
			//既に連続再生中なら実行しない
			if (m_isContinuousPlayback == true) {
				return;
			}
			//再生間隔と再生時間を設定
			m_isContinuousPlayback = true;
			m_playbackInterval = playbackInterval;
			m_playbackIntervalTimer = 0.0f;
			m_playbackTime = playbackTime;
			m_playbackTimer = 0.0f;
			//エフェクトを再生開始
			m_effect.Play(false);
		}

		void EffectRender::Stop(const bool isEffectDelete)
		{
			//再生フラグを停止
			m_isLoop = false;
			m_isPlay = true;
			m_isContinuousPlayback = false;

			//エフェクト削除フラグが立っていたら…
			if (isEffectDelete == true) {
				//再生中のエフェクトを停止
				m_effect.Stop();
			}
		}

		void EffectRender::Playing()
		{
			

			//ループ再生フラグが立っていたら…
			if (m_isLoop == true) {

				//既に再生中なら実行しない
				if (m_effect.IsPlay() == true) {
					return;
				}

				//再生
				m_effect.Play(true);

			}
			//ループ再生フラグが立っていなかったら…
			else {
				//まだ再生していない場合…
				if (m_isPlay == false) {
					//エフェクトを再生
					m_effect.Play(false);
					//エフェクトが再生されたフラグを立てる
					m_isPlay = true;
				}
			}
		}

		void EffectRender::ContinuousPlaying()
		{
			//再生時間と再生間隔を計るタイマーを進める
			m_playbackIntervalTimer += g_gameTime->GetFrameDeltaTime();
			m_playbackTimer += g_gameTime->GetFrameDeltaTime();

			//再生間隔のタイマーが再生時間に達したとき…
			if (m_playbackIntervalTimer > m_playbackInterval) {
				//エフェクトを再生
				m_effect.Play(false);
				//再生間隔のタイマーを元に戻す
				m_playbackIntervalTimer = 0.0f;
			}

			//再生時間のタイマーが終了時間に達したとき…
			if (m_playbackTimer > m_playbackTime) {
				//連続再生を終了
				m_isContinuousPlayback = false;
			}
		}
	}
}
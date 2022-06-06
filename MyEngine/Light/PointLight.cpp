#include "stdafx.h"
#include "PointLight.h"

namespace render {
	namespace light {

		const Vector3 START_POSITION = { 0.0f,100.0f,0.0f };

		const Vector3 START_COLOR = { 1.0f,1.0f,1.0f };

		const float START_RANGE = 1000.0f;


		PointLight::PointLight()
		{

		}

		PointLight::~PointLight()
		{

		}

		void PointLight::Init()
		{
			m_pointLight.position = START_POSITION;
			m_pointLight.color = START_COLOR;
			m_pointLight.range = START_RANGE;

			m_isInitd = true;
		}

		void PointLight::Execution()
		{
			if (m_isInitd == false) {
				return;
			}

			Blinking();
			Gradation();
			
		}

		void PointLight::SetBlinking(const float interval)
		{
			//点滅に必要な情報を設定
			m_blinkingInterval = interval;
		}

		void PointLight::StartBlinking()
		{
			m_blinkingTimer = 0.0f;

			m_blinkingColor = g_vec3Zero;

			m_saveColor = m_pointLight.color;

			m_blinkingFlag = true;
		}

		void PointLight::StopBlinking()
		{
			if (m_blinkingFlag == false) {
				return;
			}

			m_pointLight.color = m_saveColor;

			m_blinkingFlag = false;
		}

		void PointLight::SetGradation(std::vector<Vector3>& colors, const float speed)
		{
			//グラディエーションのために必要な情報を設定
			m_gradationColors = colors;

			m_gradationSpeed = speed;

			m_gradationTimer = 0.0f;

			m_gradationNum = 0;

		}

		void PointLight::StartGradation()
		{
			m_gradationFlag = true;
		}

		void PointLight::StopGradation()
		{
			if (m_gradationFlag == false) {
				return;
			}

			m_gradationFlag = false;
		}

		void PointLight::LightOFF()
		{
			m_pointLight.position = g_vec3Zero;
			m_pointLight.color = g_vec3Zero;
			m_pointLight.range = 0.0f;
		}

		void PointLight::Blinking()
		{
			if (m_blinkingFlag == false) {
				return;
			}
			
			//タイマーを進める
			m_blinkingTimer += g_gameTime->GetFrameDeltaTime();

			//タイマーが点滅時間に達したら
			if (m_blinkingTimer > m_blinkingInterval) {

				//現在のカラーと点滅用のカラーを入れ替える
				Vector3 color = m_pointLight.color;
				m_pointLight.color = m_blinkingColor;
				m_blinkingColor = color;

				//タイマーを0に戻す
				m_blinkingTimer = 0.0f;
			}

		}

		void PointLight::Gradation()
		{
			if (m_gradationFlag == false) {
				return;
			}

			//タイマーを速度に応じて進める
			m_gradationTimer += g_gameTime->GetFrameDeltaTime() * m_gradationSpeed;
			//補完用の次のカラー配列にアクセスする番号を用意
			int colorNum = m_gradationNum + 1;
			//番号が配列の要素数と同じになっていたら…
			if (colorNum == m_gradationColors.size()) {
				//現在の要素が配列の最後尾なので先頭の番号を次のカラー配列の番号とする
				colorNum = 0;
			}
			//タイマーの値と要素番号を使ってグラディエーション用のカラー配列を線形補完する
			m_pointLight.color.Lerp(m_gradationTimer, m_gradationColors[m_gradationNum], m_gradationColors[colorNum]);

			//タイマーが1.0を超えたら…
			if (m_gradationTimer > 1.0f) {
				//カラー配列にアクセスする番号を加算
				m_gradationNum++;
				//タイマーを元に戻す
				m_gradationTimer = 0.0f;
				//カラー配列にアクセスする番号が配列の要素数と同じになっていたら…
				if (m_gradationNum == m_gradationColors.size()) {
					//先頭の番号に戻る
					m_gradationNum = 0;
				}
			}
		}
	}
}
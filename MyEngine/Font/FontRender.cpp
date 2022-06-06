#include "stdafx.h"
#include "FontRender.h"

namespace render {
	namespace font {
		FontRender::FontRender()
		{
			m_renderingEngine = FindGO<RenderingEngine>(RENDERING_ENGINE_NAME);
			m_isInitd = false;
		}

		FontRender::~FontRender()
		{
			m_renderingEngine->DeleteFont(&m_fontData);
		}

		bool FontRender::Start()
		{
			
			return true;
		}

		void FontRender::Init(const wchar_t* text)
		{
			if (m_isInitd == false) {
				if (wcscmp(m_fontData.text.c_str(), text) != 0) {
					m_fontData.text = text;
					m_renderingEngine->SetDrawFont(&m_fontData);
					m_isInitd = true;
				}
			}
		}

		void FontRender::SetText(const wchar_t* text)
		{
			if (m_isInitd == true) {
				if (wcscmp(m_fontData.text.c_str(), text) != 0) {
					m_fontData.text = text;
				}
			}
		}

		bool FontRender::IsOpacity()
		{
			//不透明か調べる
			if (m_fontData.color.w >= 0.0f) {
				return true;
			}
			else {
				return false;
			}
		}

		bool FontRender::IsTransparent()
		{
			//透明か調べる
			if (m_fontData.color.w <= 0.0f) {
				return true;
			}
			else {
				return false;
			}
		}

		void FontRender::Fade()
		{
			if (m_fadeInFlag == true) {
				m_fontData.color.w += m_fadeRate * g_gameTime->GetFrameDeltaTime();
			}
			if (m_fadeOutFlag == true) {
				m_fontData.color.w -= m_fadeRate * g_gameTime->GetFrameDeltaTime();
			}
			if (m_fontData.color.w > 1.0f) {
				m_fontData.color.w = 1.0f;
				m_fadeInFlag = false;
			}
			if (m_fontData.color.w < 0.0f) {
				m_fontData.color.w = 0.0f;
				m_fadeOutFlag = false;
			}
		}

		void FontRender::MoveStart(const Vector2& targetPos, float velocity)
		{
			m_targetPos = targetPos;
			m_moveSpeed.x = m_targetPos.x - m_position.x;
			m_moveSpeed.y = m_targetPos.y - m_position.y;
			float Length = m_moveSpeed.x * m_moveSpeed.x;
			Length = m_moveSpeed.y * m_moveSpeed.y;
			Length = sqrtf(Length);
			m_moveSpeed.x /= Length;
			m_moveSpeed.y /= Length;

			m_moveSpeed.x *= velocity;
			m_moveSpeed.y *= velocity;

			m_moveFlag = true;

		}

		void FontRender::Move()
		{
			/// @brief 移動する前の自分の座標を記憶する
			m_previousPos = m_position;

			m_position.x += m_moveSpeed.x * g_gameTime->GetFrameDeltaTime();
			m_position.y += m_moveSpeed.y * g_gameTime->GetFrameDeltaTime();

			/// @brief 目標地点を通り過ぎたら目標地点に合わせる
			if (m_previousPos.x < m_targetPos.x) {
				if (m_targetPos.x <= m_position.x) {
					m_position.x = m_targetPos.x;
				}
			}
			if (m_previousPos.x > m_targetPos.x) {
				if (m_targetPos.x >= m_position.x) {
					m_position.x = m_targetPos.x;
				}
			}

			if (m_previousPos.y < m_targetPos.y) {
				if (m_targetPos.y <= m_position.y) {
					m_position.y = m_targetPos.y;
				}
			}
			if (m_previousPos.y > m_targetPos.y) {
				if (m_targetPos.y >= m_position.y) {
					m_position.y = m_targetPos.y;
				}
			}

			/// @brief 目標地点に着いたら移動を終わる
			if (m_position.x == m_targetPos.x && m_position.y == m_targetPos.y) {
				m_moveFlag = false;
			}
		}

		void FontRender::Update()
		{
			
		}

		void FontRender::Execution()
		{
			if (m_isInitd == false) {
				return;
			}

			//Fade();
			if (m_moveFlag == true) {
				//Move();
			}
		}

		void FontRender::Render(RenderContext& rc)
		{
			m_font.Begin(rc);
			const wchar_t* text = nullptr;
			if (m_text.c_str() != nullptr) {
				text = m_text.c_str();
			}
			m_font.Draw(
				text,
				m_position,
				m_color,
				m_rotation,
				m_scale,
				m_pivot
			);
			m_font.End(rc);

		}
	}
}
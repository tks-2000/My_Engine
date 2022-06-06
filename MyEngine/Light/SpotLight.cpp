#include "stdafx.h"
#include "SpotLight.h"

namespace render {
	namespace light {

		const Vector3 START_POSITION = { 0.0f,400.0f,0.0f };

		const Vector3 START_COLOR = { 0.0f,0.0f,0.0f };

		const float START_RANGE = 1000.0f;

		const Vector3 START_DIRECTION = { 0.0f,-1.0f,0.0f };

		const float START_ANGLE = 0.5f;


		SpotLight::SpotLight()
		{

		}

		SpotLight::~SpotLight()
		{

		}

		void SpotLight::Init()
		{
			m_spotLight.position = START_POSITION;
			m_spotLight.color = START_COLOR;
			m_spotLight.range = START_RANGE;
			m_spotLight.direction = START_DIRECTION;
			m_spotLight.angle = START_ANGLE;

			m_isInitd = true;
		}

		void SpotLight::Execution()
		{
			if (m_isInitd == false) {
				return;
			}
		}

		void SpotLight::LightOFF()
		{
			m_spotLight.position = g_vec3Zero;
			m_spotLight.color = g_vec3Zero;
			m_spotLight.range = 0.0f;
			m_spotLight.direction = g_vec3Zero;
			m_spotLight.direction.Normalize();
			m_spotLight.angle = 0.0f;
		}
	}
}
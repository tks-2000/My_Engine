#include "stdafx.h"
#include "DirectionLight.h"

namespace render {
	namespace light {

		const Vector3 START_COLOR = { 0.5f,0.5f,0.5f };

		const Vector3 START_DIRECTION = { 1.0f,0.0f,0.0f };

		DirectionLight::DirectionLight()
		{

		}

		DirectionLight::~DirectionLight()
		{

		}

		void DirectionLight::Init()
		{
			//ライトを初期化
			m_directionLight.color = START_COLOR;
			m_directionLight.direction = START_DIRECTION;

			m_isInitd = true;
		}

		void DirectionLight::Execution()
		{
			if (m_isInitd == false) {
				return;
			}

			Rotation();
		}

		void DirectionLight::SetVirtualPosition(const Vector3& pos)
		{
			//設定された仮想位置から方向を求める
			m_virtualPosition = pos;

			m_directionLight.direction = m_virtualTarget - m_virtualPosition;

			m_directionLight.direction.Normalize();
		}

		void DirectionLight::SetVirtualTarget(const Vector3& target)
		{
			//設定された仮想目標から方向を求める
			m_virtualTarget = target;

			m_directionLight.direction = m_virtualTarget - m_virtualPosition;

			m_directionLight.direction.Normalize();
		}

		void DirectionLight::SetRotation(const Vector3& axis, const float angle)
		{
			//回転軸と角度を設定し回転を始める
			m_rotationAxis = axis;
			m_rotationAngle = angle;
			m_rotationFlag = true;
		}

		void DirectionLight::LightOFF()
		{
			m_directionLight.color = g_vec3Zero;
			m_directionLight.direction = g_vec3Zero;
		}

		void DirectionLight::Rotation()
		{
			//回転フラグが立っていなかったら…
			if (m_rotationFlag == false) {
				//実行せず終了させる
				return;
			}
			
			Quaternion qRot = Quaternion::Identity;
			//回転軸と回転角度から回転を作成
			qRot.SetRotation(m_rotationAxis, m_rotationAngle);
			//ライトの方向に回転を適用する
			qRot.Apply(m_directionLight.direction);
		}
	}
}
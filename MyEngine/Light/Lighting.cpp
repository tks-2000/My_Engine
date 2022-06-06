#include "stdafx.h"
#include "Lighting.h"
#include "math.h"

namespace render {
	namespace light {
		Lighting::Lighting()
		{
			
		}

		Lighting::~Lighting()
		{

		}

		void Lighting::Init()
		{
			if (m_isInitd == true) {
				return;
			}

			for (int dirLigNo = 0; dirLigNo < DIRECTION_LIGHT_SUM; dirLigNo++) {
				m_directionLight[dirLigNo].Init();
			}

			for (int spotLigNo = 0; spotLigNo < SPOT_LIGHT_SUM; spotLigNo++) {
				//InitSpotLight(spotLigNo);
				m_spotLight[spotLigNo].Init();
			}
			for (int pointLigNo = 0; pointLigNo < POINT_LIGHT_SUM; pointLigNo++) {
				//InitPointLight(pointLigNo);
				m_pointLight[pointLigNo].Init();
			}
			InitHemiSphereLight();

			//視点の位置
			m_light.eyePos = g_camera3D->GetPosition();



			//環境光
			m_light.ambientlight.x = 0.3f;
			m_light.ambientlight.y = 0.3f;
			m_light.ambientlight.z = 0.3f;

			m_isInitd = true;
		}

		bool Lighting::Start()
		{

			return true;
		}

		void Lighting::Execution()
		{
			if (m_isInitd == false) {
				return;
			}

			for (int dirLigNo = 0; dirLigNo < DIRECTION_LIGHT_SUM; dirLigNo++) {

				m_directionLight[dirLigNo].Execution();
				m_light.directionLight[dirLigNo] = m_directionLight[dirLigNo].GetDirectionLight();
			}
			
			for (int ptLigNum = 0; ptLigNum < POINT_LIGHT_SUM; ptLigNum++) {
				
				m_pointLight[ptLigNum].Execution();
				m_light.pointLight[ptLigNum] = m_pointLight[ptLigNum].GetPointLight();
			}

			for (int spLigNum = 0; spLigNum < SPOT_LIGHT_SUM; spLigNum++) {
				
				m_spotLight[spLigNum].Execution();
				m_light.spotLight[spLigNum] = m_spotLight[spLigNum].GetSpotLight();
			}

			

			m_light.eyePos = g_camera3D->GetPosition();
		}

		void Lighting::Update()
		{
			
		}

		
		void Lighting::InitHemiSphereLight()
		{
			//地面の照り返しカラー
			m_light.hemiSphereLight.groundColor.x = 0.1f;
			m_light.hemiSphereLight.groundColor.y = 0.1f;
			m_light.hemiSphereLight.groundColor.z = 0.1f;

			//天球ライトのカラー
			m_light.hemiSphereLight.skyColor.x = -0.3f;
			m_light.hemiSphereLight.skyColor.y = -0.3f;
			m_light.hemiSphereLight.skyColor.z = -0.3f;

			//地面の法線を設定
			m_light.hemiSphereLight.groundNormal.x = 0.0f;
			m_light.hemiSphereLight.groundNormal.y = 1.0f;
			m_light.hemiSphereLight.groundNormal.z = 0.0f;

		}
	}
}
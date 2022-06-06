#pragma once
#include "math.h"
#include "stdafx.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace render {
	namespace light {
		/// @brief ディレクションライトの数
		static const int DIRECTION_LIGHT_SUM = 4;
		/// @brief ポイントライトの数
		static const int POINT_LIGHT_SUM = 5;
		/// @brief スポットライトの数
		static const int SPOT_LIGHT_SUM = 4;


		/// @brief ディレクションライト構造体
		//struct DirectionLight
		//{
		//	Vector3 direction = Vector3::Zero;	//ライトの方向
		//	float pad0 = 0.0f;					//パディング
		//	Vector3 color = Vector3::Zero;		//ライトのカラー
		//	float pad1 = 0.0f;					//パディング

		//};

		/// @brief ポイントライト構造体
		//struct PointLight
		//{
		//	Vector3 position = Vector3::Zero;	//位置
		//	float pad0 = 0.0f;					//パディング
		//	Vector3 color = Vector3::Zero;		//カラー
		//	float Range = 0.0f;					//影響範囲

		//};

		/// @brief スポットライト構造体
		//struct SpotLight
		//{
		//	Vector3 position = Vector3::Zero;	//位置
		//	float pad0 = 0.0f;					//パディング
		//	Vector3 color = Vector3::Zero;		//カラー
		//	float Range = 0.0f;					//影響範囲
		//	Vector3 direction = Vector3::Zero;	//射出方向
		//	float angle = 0.0f;					//射出角度

		//};

		/// @brief 半球ライト構造体
		struct HemiSphereLight
		{
			Vector3 groundColor = Vector3::Zero;	//地面色
			float pad0 = 0.0f;						//パディング
			Vector3 skyColor = Vector3::Zero;		//天球色
			float pad1 = 0.0f;						//パディング
			Vector3 groundNormal = Vector3::Zero;	//地面の法線
			float pad2 = 0.0f;						//パディング
		};

		/// @brief ライト全ての構造体
		struct Light
		{
			StDirectionLight directionLight[DIRECTION_LIGHT_SUM];			//ディレクションライト
			StPointLight pointLight[POINT_LIGHT_SUM];					//ポイントライト
			StSpotLight spotLight[SPOT_LIGHT_SUM];					//スポットライト
			HemiSphereLight hemiSphereLight;		//半球ライト
			Vector3 eyePos = Vector3::Zero;			//視点の位置
			float pad4 = 0.0f;						//パディング
			Vector3 ambientlight = Vector3::Zero;	//環境光
			float pad5 = 0.0f;
		};

		/// @brief ライティングを行うクラス
		class Lighting : public IGameObject
		{
		public:
			Lighting();
			~Lighting();
			bool Start();
			void Update();

			void Init();
			void Execution();


			Light& GetLight() { return m_light; }

			/// @brief ライトの構造体を入手
			/// @return ライト構造体のアドレス
			Light* GetLightAddress() { return &m_light; }

			void SetAmbientLight(const Vector3& color) { m_light.ambientlight = color; }

			/// @brief ディレクションライトの方向を入手
			/// @return ディレクションライトの方向
			Vector3 GetDirectionLightDirection() { return m_light.directionLight[0].direction; }

			/// @brief ディレクションライトの方向を設定
			/// @param dirLigVec ディレクションライトに設定する方向
			void SetDirectionLightDirection(const int num, const Vector3& dirLigVec) { m_directionLight[num].SetDirection(dirLigVec); }

			/// @brief ディレクションライトのカラーを設定
			/// @param color ディレクションライトに設定するカラー
			void SetDirectionLightColor(const int num, const Vector3& color) { m_directionLight[num].SetColor(color); }

			void SetDirectionLightRotation(const int num, const Vector3& axis, const float angle) { m_directionLight[num].SetRotation(axis, angle); }

			void StopDirectionLightRotation(const int num) { m_directionLight[num].StopRotation(); }


			/// @brief ポイントライトの座標を入手
			/// @return ポイントライトの座標
			Vector3 GetPointLightPos(int num) { return m_light.pointLight[num].position; }

			/// @brief ポイントライトの座標を設定
			/// @param pos ポイントライトに設定する座標
			void SetPointLighitPos(const int num, const Vector3& pos) { m_pointLight[num].SetPosition(pos); }

			/// @brief ポイントライトのカラーを設定
			/// @param color ポイントライトに設定する座標
			void SetPointLightColor(const int num, const Vector3& color) { m_pointLight[num].SetColor(color); }

			/// @brief ポイントライトの影響範囲を設定
			/// @param num 設定したいポイントライトの番号
			/// @param range ポイントライトに設定する影響範囲
			void SetPointLightRange(const int num, const float range) { m_pointLight[num].SetRange(range); }

			void SetPointLightBlinking(const int num, const float interval) { m_pointLight[num].SetBlinking(interval); }

			void StartPointLightBlinking(const int num) { m_pointLight[num].StartBlinking(); }

			void StopPointLightBlinking(const int num) { m_pointLight[num].StopBlinking(); }

			void SetPointLightGradation(const int num, std::vector<Vector3>& colors, const float speed) { m_pointLight[num].SetGradation(colors, speed); }

			void StartPointLightGradation(const int num) { m_pointLight[num].StartGradation(); }

			void StopPointLightGradation(const int num) { m_pointLight[num].StopGradation(); }
			/// @brief スポットライトの座標を入手
			/// @param num 入手するスポットライトの番号
			/// @return スポットライトの座標
			const Vector3& GetSpotLightPos(int num) { return m_light.spotLight[num].position; }

			/// @brief スポットライトの座標を設定
			/// @param num 設定したいスポットライトの番号
			/// @param pos スポットライトに設定する座標
			void SetSpotLightPos(const int num, const Vector3& pos) { m_spotLight[num].SetPosition(pos); }

			void SetSpotLightTarget(const int num, const Vector3& target) { m_spotLight[num].SetTarget(target); }

			/// @brief スポットライトのカラーを設定
			/// @param num 設定したいスポットライトの番号
			/// @param color スポットライトに設定するカラー
			void SetSpotLightColor(const int num, const Vector3& color) { m_spotLight[num].SetColor(color); }

			void SetSpotLightRange(const int num, const float range) { m_spotLight[num].SetRange(range); }

			
			/// @brief スポットライトの方向を設定
			/// @param num 設定したいスポットライトの番号
			/// @param dir スポットライトに設定する方向
			void SetSpotLightDirection(const int num, const Vector3& dir) { m_spotLight[num].SetDirection(dir); }

			void SetSpotLightAngle(const int num, const float angle) { m_spotLight[num].SetAngle(angle); }

			/// @brief 半球ライトの初期化
			void InitHemiSphereLight();

			void SetHemiSphereLifhtGroundColor(const Vector3& groundColor) { m_light.hemiSphereLight.groundColor = groundColor; }

			void SetHemiSphereLifhtSkyColor(const Vector3& skyColor) { m_light.hemiSphereLight.skyColor = skyColor; }

		private:
			//初期化フラグ
			bool m_isInitd = false;

			/// @brief ライティング全てのデータ
			Light m_light;

			DirectionLight m_directionLight[DIRECTION_LIGHT_SUM];

			PointLight m_pointLight[POINT_LIGHT_SUM];

			SpotLight m_spotLight[SPOT_LIGHT_SUM];
		};
	}
}

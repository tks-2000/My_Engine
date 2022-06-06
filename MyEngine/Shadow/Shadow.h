#pragma once
#include "../../MiniEngine/RenderTarget.h"
#include "../../MiniEngine/Camera.h"

namespace render {
	namespace shadow {
		const int MAX_SHADOW_MODEL_NUM = 100;

		/// @brief 影生成を行うクラス
		class Shadow
		{
		public:
			Shadow();
			~Shadow();
			void Init();
			void Execute();
			/// @brief シャドウマップを取得
			/// @return シャドウマップのconst参照
			const RenderTarget& GetShadowMap() const { return m_shadowMap; }

			Texture& GetShadowMapTexture() { return m_shadowMap.GetRenderTargetTexture(); }

			/// @brief ライトカメラを取得
			/// @return ライトカメラのconst参照
			const Camera& GetLightCamera() const { return m_lightCamera; }

			const Matrix& GetLightCameraMatrix() { return m_lightCamera.GetViewProjectionMatrix(); }

			/// @brief ライトカメラの座標を設定
			/// @param pos ライトカメラに設定する座標のconst参照
			void SetLightCameraPos(const Vector3& pos) { m_ligCameraPos = pos; }

			/// @brief ライトカメラの座標を入手
			/// @return ライトカメラの座標のconst参照
			const Vector3& GetLightCameraPos() { return m_ligCameraPos; }

			/// @brief ライトカメラの注視点を設定
			/// @param target ライトカメラに設定する注視点座標のconst参照
			void SetLightCameraTarget(const Vector3& target) { m_ligCameraTarget = target; }


			const Vector3& GetLightCameraTarget() { return m_ligCameraTarget; }

			/// @brief 影を生成するモデルを作成
			/// @param shadowModel 影を生成するモデル
			void SetShadowModel(Model* shadowModel);

			/// @brief 影を生成するモデルを削除
			/// @param shadowModel 削除するモデルのアドレス
			void DeleteShadowModel(Model* shadowModel);

			void Render(RenderContext& rc);

			void CreateShadowMap(RenderContext& rc);
		private:
			/// @brief 初期化フラグ
			bool m_isInitd = false;
			/// @brief シャドウマップ
			RenderTarget m_shadowMap;
			/// @brief カラーバッファのクリアカラー
			float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
			/// @brief ライトカメラ
			Camera m_lightCamera;
			/// @brief ライトカメラの座標
			Vector3 m_ligCameraPos = g_vec3One;
			/// @brief ライトカメラの注視点
			Vector3 m_ligCameraTarget = g_vec3Zero;
			/// @brief ライトカメラの上方向
			Vector3 m_ligCameraUp = { 0.0f,0.0f,1.0f };
			/// @brief ライトカメラの画角
			float m_ligCameraViewAngle = 50.0f;
			/// @brief シャドウマップに書き込むモデルの合計数
			int m_shadowModelSum = 0;
			/// @brief シャドウマップ生成のためのモデル
			std::vector<Model*> m_shadowModel;
			/// @brief シャドウモデルの番号
			int m_shadowModelNum[MAX_SHADOW_MODEL_NUM] = { MAX_SHADOW_MODEL_NUM };

			/// @brief ライトカメラの更新
			void LightCameraUpdate();
		};
	}
}


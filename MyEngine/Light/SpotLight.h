#pragma once

namespace render {
	namespace light {
		/// @brief スポットライト構造体
		struct StSpotLight
		{
			Vector3 position = Vector3::Zero;	//位置
			float pad0 = 0.0f;					//パディング
			Vector3 color = Vector3::Zero;		//カラー
			float range = 0.0f;					//影響範囲
			Vector3 direction = Vector3::Zero;	//射出方向
			float angle = 0.0f;					//射出角度

		};

		/// @brief スポットライトの機能をまとめたクラス
		class SpotLight
		{
		public:
			SpotLight();
			~SpotLight();
			/// @brief 初期化
			void Init();
			/// @brief 実行
			void Execution();
			/// @brief 座標を設定
			/// @param pos 設定する座標
			void SetPosition(const Vector3& pos) { m_spotLight.position = pos; }
			/// @brief 照らす目標を設定
			/// @param target 照らす目標の座標 
			void SetTarget(const Vector3& target) { m_spotLight.direction = target - m_spotLight.position; m_spotLight.direction.Normalize(); }
			/// @brief カラーを設定
			/// @param color 設定するカラー
			void SetColor(const Vector3& color) { m_spotLight.color = color; }
			/// @brief 影響範囲を設定
			/// @param range 設定する影響範囲
			void SetRange(const float range) { m_spotLight.range = range; }
			/// @brief 射出方向を設定
			/// @param dir 設定する射出方向・正規化ベクトルを渡して下さい
			void SetDirection(const Vector3& direction) { m_spotLight.direction = direction; }
			/// @brief 射出角度を設定
			/// @param angle 設定する射出角度
			void SetAngle(const float angle) { m_spotLight.angle = angle; }
			/// @brief ライトを消す
			void LightOFF();
			/// @brief スポットライトを入手
			/// @return スポットライト構造体のconst参照
			const StSpotLight& GetSpotLight() const { return m_spotLight; }
		private:
			/// @brief 初期化フラグ
			bool m_isInitd = false;
			/// @brief スポットライト
			StSpotLight m_spotLight;
		};
	}
}



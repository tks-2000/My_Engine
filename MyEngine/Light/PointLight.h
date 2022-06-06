#pragma once

namespace render {
	namespace light {

		const int GRADATION_COLOR_NUM = 7;

		/// @brief ポイントライト構造体
		struct StPointLight
		{
			Vector3 position = Vector3::Zero;	//位置
			float pad0 = 0.0f;					//パディング
			Vector3 color = Vector3::Zero;		//カラー
			float range = 0.0f;					//影響範囲
		};

		/// @brief ポイントライトの機能をまとめたクラス
		class PointLight
		{
		public:
			PointLight();
			~PointLight();
			/// @brief 初期化
			void Init();
			/// @brief 実行
			void Execution();
			/// @brief 座標を設定
			/// @param pos 設定する座標
			void SetPosition(const Vector3& pos) { m_pointLight.position = pos; }
			/// @brief カラーを設定
			/// @param color 設定するカラー
			void SetColor(const Vector3& color) { m_pointLight.color = color; }
			/// @brief 影響範囲を設定
			/// @param range 設定する影響範囲
			void SetRange(const float range) { m_pointLight.range = range; }
			/// @brief 点滅を設定
			/// @param interval 点滅の間隔
			void SetBlinking(const float interval);
			/// @brief 点滅を開始
			void StartBlinking();
			/// @brief 点滅を停止
			void StopBlinking();
			/// @brief グラディエーションを設定
			/// @param colors グラディエーションをする色のvector型配列
			/// @param speed グラディエーションをする速度
			void SetGradation(std::vector<Vector3>& colors, const float speed);
			/// @brief グラディエーションを開始
			void StartGradation();
			/// @brief グラディエーションを停止
			void StopGradation();
			/// @brief ライトを消す
			void LightOFF();
			/// @brief ポイントライトを入手
			/// @return ポイントライト構造体のconst参照
			const StPointLight& GetPointLight() const { return m_pointLight; }
		private:
			/// @brief 点滅の処理を行う
			void Blinking();
			/// @brief グラディエーションの処理を行う
			void Gradation();
			/// @brief 初期化フラグ
			bool m_isInitd = false;
			/// @brief 点滅フラグ
			bool m_blinkingFlag = false;
			/// @brief 点滅するための時間を計るタイマー
			float m_blinkingTimer = 0.0f;
			/// @brief 点滅間隔
			float m_blinkingInterval = 0.0f;
			/// @brief 点滅する色
			Vector3 m_blinkingColor = g_vec3Zero;
			/// @brief 点滅する前の色
			Vector3 m_saveColor = g_vec3Zero;
			/// @brief グラディエーションフラグ
			float m_gradationFlag = false;
			/// @brief グラディエーションをする色の配列
			std::vector<Vector3> m_gradationColors;
			/// @brief グラディエーションする速度
			float m_gradationSpeed = 0.0f;
			/// @brief グラディエーションをするための時間を計るタイマー
			float m_gradationTimer = 0.0f;
			/// @brief グラディエーション用の配列にアクセスするための番号
			int m_gradationNum = 0;

			/// @brief ポイントライト
			StPointLight m_pointLight;
		};
	}
}

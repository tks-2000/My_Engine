#pragma once

namespace render {
	namespace light {
		/// @brief ディレクションライト構造体
		struct StDirectionLight
		{
			Vector3 direction = Vector3::Zero;	//ライトの方向
			float pad0 = 0.0f;					//パディング
			Vector3 color = Vector3::Zero;		//ライトのカラー
			float pad1 = 0.0f;					//パディング

		};

		/// @brief ディレクションライトを管理するクラス
		class DirectionLight
		{
		public:
			DirectionLight();
			~DirectionLight();
			/// @brief 初期化
			void Init();
			/// @brief 実行
			void Execution();
			/// @brief 仮想的な位置を設定
			/// @param pos 光源の位置
			void SetVirtualPosition(const Vector3& pos);
			/// @brief 仮想的の目標を設定
			/// @param target 目標の位置
			void SetVirtualTarget(const Vector3& target);
			/// @brief 方向を設定
			/// @param direction 設定したい方向・正規化ベクトルを設定して下さい
			void SetDirection(const Vector3& direction) { m_directionLight.direction = direction; }
			/// @brief カラーを設定
			/// @param color 設定したい色
			void SetColor(const Vector3& color) { m_directionLight.color = color; }
			/// @brief 回転を設定
			/// @param Axis 回転軸
			/// @param angle 回転角度
			void SetRotation(const Vector3& axis, const float angle);
			/// @brief 回転を停止
			void StopRotation() { m_rotationFlag = false; }
			/// @brief ライトを消す
			void LightOFF();
			/// @brief ディレクションライトを取得
			/// @return ディレクションライト構造体のconst参照
			const StDirectionLight& GetDirectionLight() const { return m_directionLight; }
			
		private:

			void Rotation();

			/// @brief 初期化フラグ
			bool m_isInitd = false;
			/// @brief ディレクションライト
			StDirectionLight m_directionLight;
			/// @brief 仮想的な位置・方向のみのライトなので計算して方向を決める
			Vector3 m_virtualPosition = g_vec3Zero;
			/// @brief 仮想的な目標・方向のみのライトなので計算して方向を決める
			Vector3 m_virtualTarget = g_vec3Zero;
			/// @brief 回転フラグ
			bool m_rotationFlag = false;
			/// @brief 回転軸
			Vector3 m_rotationAxis = g_vec3Zero;
			/// @brief 回転角度
			float m_rotationAngle = 0.0f;
		};
	}
}

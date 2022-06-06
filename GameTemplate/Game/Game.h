#pragma once


class SkinModelRender;
class SpriteRender;
class Effect;
class CSoundSource;


namespace nsAI {
	class NaviMesh;
	class Path;
	class PathFinding;
}

namespace sound {
	class SoundPlayer;
}

namespace mainGame {
	namespace title {
		class Title;
	}
	namespace player {
		class Player;
	}
	namespace defensiveTarget {
		class DefensiveTarget;
	}
	namespace enemy {
		class Generator;
		class Enemy;
	}
	namespace stage {
		class Stage;
	}
	namespace timer {
		class Timer;
	}
	namespace map {
		class MiniMap;
	}
	namespace item {
		class ItemGenerator;
	}
	namespace ui {
		class GameUI;
	}
	
	class GameScene;

	class GameCamera;

	class SceneTransition;

	/// @brief ゲームの状態を表す列挙型
	enum EnGameState {
		/// @brief タイトル画面
		enTitleScene,
		/// @brief ゲーム進行中
		enGameInProgress,
		/// @brief ゲームの状態を表すステートの数
		enGameStateNum
	};

	/// @brief ゲーム全体の流れを制御するクラス
	class Game : public IGameObject
	{
	public:
		Game();
		~Game();
		
		/// @brief 開始時の処理
		/// @return 
		bool Start();
		/// @brief 更新
		void Update();

		///// @brief タイトルシーン開始の処理
		//void TitleSceneStart();
		///// @brief タイトルシーン削除の処理
		//void DeleteTitleScene(); 
		///// @brief ゲームシーン開始の処理
		//void GameSceneStart();
		///// @brief ゲームシーン削除の処理
		//void DeleteGameScene();

		///// @brief ゲームの状態を変更
		///// @param state ゲームの状態を表すステート
		//void SetGameState(const EnGameState& state) { m_state = state; }

		/// @brief ゲームの状態を取得
		/// @return ゲームの状態を表すステート
		const EnGameState& GetGameState() { return m_state; }

		

	private:
		

		/// @brief 初期化フラグ
		bool m_isInitd = false;
		/// @brief ゲームの状態を表すステート
		EnGameState m_state = enGameStateNum;
		/// @brief タイトル
		title::Title* m_title = nullptr;
		/// @brief ゲームシーン
		GameScene* m_gameScene = nullptr;
		/// @brief サウンドプレイヤー
		sound::SoundPlayer* m_soundPlayer = nullptr;
		/// @brief レンダリングエンジン
		render::RenderingEngine* m_renderingEngine = nullptr;
		/// @brief シーン遷移
		SceneTransition* m_sceneTransition = nullptr;

		render::model::SkinModelRender* m_model = nullptr;
	};

}
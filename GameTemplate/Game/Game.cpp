#include "stdafx.h"
#include "Game.h"

namespace mainGame {

	Game::Game()
	{
		//�Q�[���̗v�f���쐬
		//m_soundPlayer = NewGO<sound::SoundPlayer>(PRIORITY_VERYLOW, sound::SOUND_PLAYER_NAME);	
		//m_sceneTransition = NewGO<SceneTransition>(PRIORITY_VERYLOW, SCENE_TRANSITION_NAME);
		m_renderingEngine = FindGO<render::RenderingEngine>(render::RENDERING_ENGINE_NAME);
	}

	Game::~Game()
	{
		
	}

	bool Game::Start()
	{
		//���������s��
		//m_sceneTransition->Init();

		//�^�C�g���V�[������n�߂�
		//TitleSceneStart();
		
		m_model = NewGO<render::model::SkinModelRender>(PRIORITY_VERYLOW);
		m_model->Init("Assets/modelData/unityChan/utc_PBR.tkm");

		//����������
		m_isInitd = true;

		

		return true;
	}

	void Game::Update()
	{
		
		//���������Ȃ���s���Ȃ�
		if (m_isInitd == false) {
			return;
		}

		m_model->Execution();

		/*if (m_state == enTitleScene) {
			m_title->Execution();
		}

		if (m_state == enGameInProgress) {
			m_gameScene->Execution();
		}

		m_sceneTransition->Execution();*/
	}

	/*void Game::TitleSceneStart()
	{
		m_title = NewGO<title::Title>(PRIORITY_VERYLOW, title::TITLE_NAME);
		m_title->Init();
		m_state = enTitleScene;
	}

	void Game::DeleteTitleScene()
	{
		DeleteGO(m_title);
	}

	void Game::GameSceneStart()
	{
		m_gameScene = NewGO<GameScene>(PRIORITY_VERYLOW, GAME_SCENE_NAME);
		m_gameScene->Init();
		m_state = enGameInProgress;
	}

	void Game::DeleteGameScene()
	{
		DeleteGO(m_gameScene);
	}*/
}
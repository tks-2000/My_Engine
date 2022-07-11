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

		render::model::StModelInitData utcModelInitData;
		render::model::StModelInitData stageModelInitData;

		utcModelInitData.tkmFilePath = "Assets/modelData/unityChan/utc_PBR.tkm";
		utcModelInitData.modelRenderFormat = render::model::enDeferrdPBR;
		utcModelInitData.tksFilePath = "Assets/modelData/unityChan/utc_PBR.tks";
		utcModelInitData.tkaFilePaths.push_back("Assets/animData/unitychan/idle.tka");
		utcModelInitData.modelUpAxis = enModelUpAxisY;

		stageModelInitData.tkmFilePath = "Assets/modelData/bg/Stage2.tkm";
		stageModelInitData.modelRenderFormat = render::model::enDeferrdPBR;
		
		m_model = NewGO<render::model::SkinModelRender>(PRIORITY_VERYLOW);
		m_model->Init(utcModelInitData);

		m_stageModel = NewGO<render::model::SkinModelRender>(PRIORITY_VERYLOW);
		m_stageModel->Init(stageModelInitData);

		//����������
		m_isInitd = true;

		g_camera3D->SetPosition(0.0f, 50.0f, 200.0f);
		g_camera3D->SetTarget(0.0f, 50.0f, 0.0f);

		return true;
	}

	void Game::Update()
	{
		
		//���������Ȃ���s���Ȃ�
		if (m_isInitd == false) {
			return;
		}

		m_weight += g_pad[0]->GetRStickYF() * 0.01f;

		m_model->SetDitheringWeight(m_weight);

		m_model->Execution();
		m_stageModel->Execution();

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
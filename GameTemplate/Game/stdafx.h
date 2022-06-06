#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define TK_ENABLE_HDR		//定義でHDRレンダリングを有効にする。

#include "MiniEngine.h"
#include "ExEngine.h"
#include "../MyEngine/RenderingEngine.h"
#include "../MyEngine/DeferredRender.h"
#include "../MyEngine/Light/Lighting.h"
#include "../MyEngine/Shadow/Shadow.h"
#include "../MyEngine/PostEffect/PostEffect.h"
#include "../MyEngine/PostEffect/GaussianBlur.h"
#include "../MyEngine/PostEffect/Bloom.h"
#include "../MyEngine/Model/SkinModelRender.h"
#include "../MyEngine/Sprite/SpriteRender.h"
#include "../MyEngine/Font/FontRender.h"
#include "../../ExEngine/effect/Effect.h"
#include "../MyEngine/Effect/EffectRender.h"
#include "../MyEngine/Sound/SoundSource.h"
#include "../MyEngine/Sound/SoundPlayer.h"
#include "../MyEngine/NaviAI/NaviMesh.h"
#include "../MyEngine/NaviAI/Path.h"
#include "../MyEngine/NaviAI/PathFinding.h"
#include "Game.h"


namespace {
	const int PRIORITY_VERYLOW = 0;
}

namespace sound {
	static const char* SOUND_PLAYER_NAME = "SoundPlayer";
}

namespace mainGame {

	static const int PLAYER1_CONTROLLER_NUM = 0;

	static const char* GAME_NAME = "Game";
	static const char* GAME_SCENE_NAME = "GameScene";
	static const char* GAME_CAMERA_NAME = "GameCamera";
	static const char* SCENE_TRANSITION_NAME = "SceneTransition";

	namespace title{
		static const char* TITLE_NAME = "Title";
	}
	namespace player {
		static const char* PLAYER_NAME = "Player";
	}
	namespace defensiveTarget {
		static const char* DEFENSIVE_TARGET_NAME = "DefensiveTarget";
	}
	namespace enemy {

		static const char* ENEMY_GENERATOR_NAME = "EnemyGenerator";

		static const char* ENEMY_NAME = "Enemy";

		
	}
	namespace stage {
		static const char* STAGE_NAME = "Stage";
	}

	namespace timer {
		static const char* TIMER_NAME = "Timer";
	}

	

	namespace item {
		static const char* ITEM_GENERATOR_NAME = "ItemGenerator";
	}

	namespace ui {
		static const char* GAME_UI_NAME = "GameUI";
		static const int MINI_MAP_RENDER_GROUP = render::model::enExpandModelGroup1;
		static const char* MINI_MAP_NAME = "MIniMap";
	}

	
}
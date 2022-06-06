#pragma once
#include "MiniEngine.h"
#include "ExEngine.h"

/// @brief 他の場所でも必要になるレンダリングエンジンの定数
namespace render {

	static const char* RENDERING_ENGINE_NAME = "RenderingEngine";

	/// @brief ライト
	namespace light {
		static const char* LIGHTING_NAME = "Lighting";
	}

	/// @brief シャドウ
	namespace shadow {
		static const char* SHADOW_NAME = "Shadow";
	}

	/// @brief ポストエフェクト
	namespace postEffect {
		static const char* POST_EFFECT_NAME = "PostEffect";
		static const char* GAUSSIAN_BLUR_NAME = "GaussianBlur";
		static const char* BLOOM_NAME = "Bloom";
	}

	/// @brief 無色
	static const Vector4 COLORLESS = { 0.0f,0.0f,0.0f,0.0f };

}
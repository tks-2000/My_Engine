#pragma once
#include "RenderResource.h"
#include "DeferredRender.h"
#include "Light/Lighting.h"
#include "Shadow/Shadow.h"
#include "Font/FontRender.h"
#include "PostEffect/PostEffect.h"

namespace render {
	namespace light {
		class Lighting;
	}
	namespace shadow {
		class Shadow;
	}
	namespace postEffect{
		class PostEffect;
	}
	/// @brief レンダリングに必要な機能をまとめたクラス
	class RenderingEngine : public IGameObject
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

		Texture* GetTexture() { return &m_mainRenderTarget.GetRenderTargetTexture(); }

		/// @brief 描画するモデルを追加
		/// @param model 追加するモデルのアドレス
		void SetDrawModel(Model* model);

		/// @brief 描画しているモデルを削除
		/// @param model 削除するモデルのアドレス
		void DeleteModel(Model* model);
 

		void SetExpansionModlsRenderTarget(int modelGroupNum, RenderTarget* rt);

		void DeleteExpansionModelsRenderTarget(int modelGroupNum);

		/// @brief 
		/// @param modelGroupNum 
		/// @param model 
		void SetExpansionDrawModel(int modelGroupNum,Model* model);

		/// @brief 
		/// @param modelGroupNum 
		/// @param model 
		void DeleteExpansionDrawModel(int modelGroupNum, Model* model);

		/// @brief 
		/// @param modelGroupNum 
		/// @param model 
		void SetExpansionModelDrawCamera(int modelGroupNum, Camera* camera);
	
		/// @brief 
		/// @param modelGroupNum 
		void DeleteExpansionModelDrawCamera(int modelGroupNum);

		void SetDeferredModel(Model* model) { m_deferredRender.SetDrawModel(model); }

		void DeleteDeferrdModel(Model* model) { m_deferredRender.DeleteDrawModel(model); }

		/// @brief 描画するスプライトを追加
		/// @param sprite 追加するスプライトのアドレス
		void SetDrawSprite(Sprite* sprite);

		/// @brief 描画しているスプライトを削除
		/// @param sprite 削除するスプライトのアドレス
		void DeleteSprite(Sprite* sprite);

		/// @brief 描画するフォントを追加
		/// @param font 追加するフォントのアドレス
		void SetDrawFont(font::FontData* fontdata);

		/// @brief 描画しているフォントを削除
		/// @param font 削除するフォントのアドレス
		void DeleteFont(font::FontData* fontdata);

		void SetLightFlag(const bool Authenticity) { m_ligFlag = Authenticity; }

		void SetEffectFlag(const bool Authenticity) { m_effectFlag = Authenticity; }

		light::Lighting* GetLighting(){ return &m_lig; }

		const int GetLightingSize(){return sizeof(m_lig); }

		shadow::Shadow* GetShadow() { return &m_shadow; }

		void SetFinalSpriteColor(const Vector4& color) { m_finalSprite.ColorUpdate(color); }

	private:
		/// @brief ライトの更新
		void LightUpdate();

		void DrawExpansionModel(int modelGroupNum,RenderContext& rc);

		DeferredRender m_deferredRender;

		RenderTarget m_deferredRenderTarget;

		Sprite m_deferredSprite;
		
		/// @brief ライティング
		light::Lighting m_lig;
		/// @brief シャドウ
		shadow::Shadow m_shadow;
		/// @brief ポストエフェクト
		postEffect::PostEffect* m_postEffect = nullptr;

		/// @brief メインレンダリングターゲット
		RenderTarget m_mainRenderTarget;

		/// @brief 描画するモデルの配列
		std::vector<Model*> m_drawModels;

		/// @brief 追加で描画する描画モデルグループの数
		enum{EXPANSION_MODEL_GROUP_NUM = 3};

		/// @brief 追加で描画するモデルグループの描画フラグ
		bool m_isExpansionModelDraw[EXPANSION_MODEL_GROUP_NUM] = { false };

		/// @brief 追加で描画するモデルグループ
		std::vector<Model*> m_expansionDrawModels[EXPANSION_MODEL_GROUP_NUM];

		/// @brief 追加で描画するレンダリングターゲットが作られたか？
		bool m_isCreateExpansionModelRenderTarget[EXPANSION_MODEL_GROUP_NUM] = { false };

		/// @brief 追加で描画するモデルグループの描画先のポインタ
		RenderTarget* m_expansionModelsRenderTarget[EXPANSION_MODEL_GROUP_NUM] = { nullptr };

		/// @brief 追加で描画するモデルグループを映すカメラ
		Camera* m_expansionModelsDrawCamera[EXPANSION_MODEL_GROUP_NUM] = { nullptr };

		/// @brief 描画するスプライトの配列
		std::vector<Sprite*> m_drawSprites;

		/// @brief 描画するフォントの配列
		std::vector<font::FontData*> m_drawFontsData;

		/// @brief メインレンダリングターゲットのスプライトの初期化情報
		SpriteInitData m_mainRenderTargetSpriteInitData;
		/// @brief メインレンダリングターゲットのスプライト
		Sprite m_mainRenderTargetSprite;

		

		RenderTarget m_finalRenderTarget;

		SpriteInitData m_finalSpriteInitData;

		Sprite m_finalSprite;

		


		Vector3 m_ligColor = g_vec3Zero;

		bool m_ligFlag = false;

		bool m_effectFlag = false;

	};
}

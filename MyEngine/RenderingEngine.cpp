#include "stdafx.h"
#include "RenderingEngine.h"

namespace render {
	RenderingEngine::RenderingEngine()
	{
		//m_lig = NewGO<light::Lighting>(0, light::LIGHTING_NAME);
		//m_shadow = NewGO<shadow::Shadow>(0, shadow::SHADOW_NAME);
		m_postEffect = NewGO<postEffect::PostEffect>(0, postEffect::POST_EFFECT_NAME);

		m_deferredRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
		SpriteInitData deferrdSpriteInitData;
		deferrdSpriteInitData.m_textures[0] = &m_deferredRenderTarget.GetRenderTargetTexture();
		deferrdSpriteInitData.m_width = FRAME_BUFFER_W;
		deferrdSpriteInitData.m_height = FRAME_BUFFER_H;
		deferrdSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_deferredSprite.Init(deferrdSpriteInitData);

		//メインレンダリングターゲットを作成
		m_mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//フレームバッファのコピー用のスプライトを用意する
		m_mainRenderTargetSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		m_mainRenderTargetSpriteInitData.m_width = 1280;
		m_mainRenderTargetSpriteInitData.m_height = 720;
		m_mainRenderTargetSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_mainRenderTargetSprite.Init(m_mainRenderTargetSpriteInitData);

		m_finalRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		m_finalSpriteInitData.m_textures[0] = &m_finalRenderTarget.GetRenderTargetTexture();
		m_finalSpriteInitData.m_width = 1280;
		m_finalSpriteInitData.m_height = 720;
		m_finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		m_finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		m_finalSprite.Init(m_finalSpriteInitData);
		m_lig.Init();
		m_shadow.Init();
		m_deferredRender.SetPBR(true);
		m_deferredRender.Init(this);
		
	
	}

	RenderingEngine::~RenderingEngine()
	{
		//DeleteGO(m_lig);
		//DeleteGO(m_shadow);
		DeleteGO(m_postEffect);
	}

	bool RenderingEngine::Start()
	{
		Vector3 dirColor = { 0.5f,0.5f,0.5f };
		m_lig.SetDirectionLightColor(0, dirColor);
		m_lig.SetDirectionLightColor(1, dirColor);
		m_lig.SetDirectionLightColor(2, dirColor);
		m_lig.SetDirectionLightColor(3, dirColor);
		Vector3 dir = { 0.0f,0.0f,-1.0f };
		dir.Normalize();
		m_lig.SetDirectionLightDirection(0,dir);
		dir = { 0.0f,0.0f,1.0f };
		dir.Normalize();
		m_lig.SetDirectionLightDirection(1, dir);
		dir = { 1.0f,0.0f,0.0f };
		dir.Normalize();
		m_lig.SetDirectionLightDirection(2, dir);
		dir = { -1.0f,0.0f,0.0f };
		dir.Normalize();
		m_lig.SetDirectionLightDirection(3, dir);
		//m_lig.SetDirectionLightRotation(0, Vector3::AxisY, 0.01f);
		//m_lig.SetDirectionLightRotation(1, Vector3::AxisY, 0.01f);
		//m_lig.SetDirectionLightRotation(2, Vector3::AxisY, 0.01f);
		//m_lig.SetDirectionLightRotation(3, Vector3::AxisY, 0.01f);
		m_lig.SetAmbientLight({ 0.1f,0.1f,0.1f });
		
		m_lig.SetHemiSphereLifhtGroundColor({ 0.5f,0.2f,0.2f });
		m_lig.SetHemiSphereLifhtSkyColor({ 0.2f,0.2f,0.5f });
		//m_lig.SetPointLighitPos(0, { 200.0f,200.0f,0.0f });
		//m_lig.SetPointLightColor(0, { 5.0f,5.0f,5.0f });
		//m_lig.SetPointLightRange(0, 1000.0f);
		//m_lig.SetPointLightBlinking(0, 0.5f);
		std::vector<Vector3> colors;
		colors.push_back({ 5.0f,0.0f,0.0f });
		colors.push_back({ 5.0f,5.0f,0.0f });
		colors.push_back({ 0.0f,5.0f,0.0f });
		colors.push_back({ 0.0f,5.0f,5.0f });
		colors.push_back({ 0.0f,0.0f,5.0f });
		colors.push_back({ 5.0f,0.0f,5.0f });
		//m_lig.SetPointLighitPos(1, { -200.0f,200.0f,0.0f });
		//m_lig.SetPointLightColor(1, { 5.0f,0.0f,0.0f });
		//m_lig.SetPointLightRange(1, 1000.0f);
		//m_lig.SetPointLightGradation(1, colors, 0.5f);
		//m_lig.SetSpotLightPos(0, { 0.0f, 400.0f, 300.0f });
		//m_lig.SetSpotLightTarget(0, { 0.0,0.0f,300.0f });
		//m_lig.SetSpotLightColor(0, { 0.0f,0.0f,15.0f });
		/*m_lig->SetDirectionLightFlickering(
			{ 0.3f,0.3f,0.3f },
			{ 0.5f,0.5f,0.5f },
			0.1f
		);*/
		
	
		m_ligFlag = true;
		m_effectFlag = true;
		//m_shadow->SetLightCameraTarget({ 0.0f,0.0f,0.0f });

		//ブラーをかける
		//m_postEffect->SetBlur(&m_mainRenderTarget);

		//ブルームをかける
		m_postEffect->SetBloom(&m_mainRenderTarget);

		return true;
	}

	void RenderingEngine::Update()
	{
		
	
		
	}

	void RenderingEngine::Render(RenderContext& rc)
	{
		if (m_ligFlag == true) {
			LightUpdate();
		}

		m_shadow.Execute();
		m_shadow.CreateShadowMap(rc);

		m_deferredRender.Execute(rc);

		rc.WaitUntilToPossibleSetRenderTarget(m_deferredRenderTarget);
		rc.SetRenderTargetAndViewport(m_deferredRenderTarget);
		rc.ClearRenderTargetView(m_deferredRenderTarget);

		m_deferredRender.Draw(rc);
		
		rc.WaitUntilFinishDrawingToRenderTarget(m_deferredRenderTarget);

		

		//メインレンダリングターゲットを使用可能になるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(),m_deferredRender.GetGbufferDSV());
		rc.ClearRenderTargetView(m_mainRenderTarget);

		m_deferredSprite.Draw(rc);


		//描画するモデルを全て描画する
		for (int modelNum = 0; modelNum < m_drawModels.size(); modelNum++) {
			m_drawModels[modelNum]->Draw(rc);
		}
	
		if (m_effectFlag == true) {



			EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
			EffectEngine::GetInstance()->Draw();


		}
		//メインレンダリングターゲットへ書き込み終了
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		

		

		//ポストエフェクトを実行
		m_postEffect->Execute(rc);

		

		for (int groupNum = 0; groupNum < EXPANSION_MODEL_GROUP_NUM; groupNum++) {
			if (m_expansionDrawModels[groupNum].size() > 0 &&
				m_expansionModelsRenderTarget[groupNum] != nullptr &&
				m_expansionModelsDrawCamera[groupNum] != nullptr
				) {

				DrawExpansionModel(groupNum,rc);
			}
		}


		rc.WaitUntilToPossibleSetRenderTarget(m_finalRenderTarget);
		rc.SetRenderTargetAndViewport(m_finalRenderTarget);
		rc.ClearRenderTargetView(m_finalRenderTarget);

		//メインレンダリングターゲットのコピーのスプライトを表示する
		m_mainRenderTargetSprite.Draw(rc);

		//スプライトを描画
		for (int spriteNum = 0; spriteNum < m_drawSprites.size(); spriteNum++) {
			m_drawSprites[spriteNum]->Draw(rc);
		}

		//フォントを描画
		for (int fontNum = 0; fontNum < m_drawFontsData.size(); fontNum++) {
			m_drawFontsData[fontNum]->font.Begin(rc);
			const wchar_t* text = nullptr;
			if (m_drawFontsData[fontNum]->text.c_str() != nullptr) {
				text = m_drawFontsData[fontNum]->text.c_str();
			}
			m_drawFontsData[fontNum]->font.Draw(
				text,
				m_drawFontsData[fontNum]->pos,
				m_drawFontsData[fontNum]->color,
				m_drawFontsData[fontNum]->rotation,
				m_drawFontsData[fontNum]->scale,
				m_drawFontsData[fontNum]->pivot
			);
			m_drawFontsData[fontNum]->font.End(rc);

		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_finalRenderTarget);

		// レンダリングターゲットをフレームバッファに戻す
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		m_finalSprite.Draw(rc);
	}

	void RenderingEngine::LightUpdate()
	{
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			m_lig.StopPointLightBlinking(0);
			m_lig.StopPointLightGradation(1);
		}
		if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_lig.StartPointLightBlinking(0);
			m_lig.StartPointLightGradation(1);
		}


		m_lig.Execution();
	}

	void RenderingEngine::DrawExpansionModel(int modelGroupNum,RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(*m_expansionModelsRenderTarget[modelGroupNum]);
		rc.SetRenderTargetAndViewport(*m_expansionModelsRenderTarget[modelGroupNum]);
		rc.ClearRenderTargetView(*m_expansionModelsRenderTarget[modelGroupNum]);

		for (int modelNum = 0; modelNum < m_expansionDrawModels[modelGroupNum].size(); modelNum++) {
			m_expansionDrawModels[modelGroupNum][modelNum]->Draw(rc,*m_expansionModelsDrawCamera[modelGroupNum]);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(*m_expansionModelsRenderTarget[modelGroupNum]);

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	}

	void RenderingEngine::SetDrawModel(Model* model)
	{
		m_drawModels.push_back(model);
	}

	void RenderingEngine::DeleteModel(Model* model)
	{
		std::vector<Model*>::iterator it;
		it = std::find(
			m_drawModels.begin(),
			m_drawModels.end(),
			model
		);
		if (it != m_drawModels.end()) {
			m_drawModels.erase(it);
		}
	}

	void RenderingEngine::SetExpansionModlsRenderTarget(int modelGroupNum, RenderTarget* rt)
	{
		m_expansionModelsRenderTarget[modelGroupNum] = rt;
		m_isCreateExpansionModelRenderTarget[modelGroupNum] = true;
	}

	void RenderingEngine::DeleteExpansionModelsRenderTarget(int modelGroupNum)
	{
		m_expansionModelsRenderTarget[modelGroupNum] = nullptr;
		m_isCreateExpansionModelRenderTarget[modelGroupNum] = false;
	}

	void RenderingEngine::SetExpansionDrawModel(int modelGroupNum, Model* model)
	{
		m_expansionDrawModels[modelGroupNum].push_back(model);
		m_isExpansionModelDraw[modelGroupNum] = true;
	}

	void RenderingEngine::DeleteExpansionDrawModel(int modelGroupNum, Model* model)
	{
		std::vector<Model*>::iterator it;
		it = std::find(
			m_expansionDrawModels[modelGroupNum].begin(),
			m_expansionDrawModels[modelGroupNum].end(),
			model
		);

		if (it != m_expansionDrawModels[modelGroupNum].end()) {
			m_expansionDrawModels[modelGroupNum].erase(it);
		}

		if (m_expansionDrawModels[modelGroupNum].size() == 0) {
			m_isExpansionModelDraw[modelGroupNum] = false;
		}
	}

	void RenderingEngine::SetExpansionModelDrawCamera(int groupNum, Camera* camera)
	{
		m_expansionModelsDrawCamera[groupNum] = camera;
	}

	void RenderingEngine::DeleteExpansionModelDrawCamera(int groupNum)
	{
		m_expansionModelsDrawCamera[groupNum] = nullptr;
	}
	

	void RenderingEngine::SetDrawSprite(Sprite* sprite)
	{
		m_drawSprites.push_back(sprite);
	}

	void RenderingEngine::DeleteSprite(Sprite* sprite)
	{
		std::vector<Sprite*>::iterator it;
		it = std::find(
			m_drawSprites.begin(),
			m_drawSprites.end(),
			sprite
		);
		if (it != m_drawSprites.end()) {
			m_drawSprites.erase(it);
		}
	}

	void RenderingEngine::SetDrawFont(font::FontData* fontData)
	{
		m_drawFontsData.push_back(fontData);
	}

	void RenderingEngine::DeleteFont(font::FontData* fontData)
	{
		std::vector<font::FontData*>::iterator it;
		it = std::find(
			m_drawFontsData.begin(),
			m_drawFontsData.end(),
			fontData
		);
		if (it != m_drawFontsData.end()) {
			m_drawFontsData.erase(it);
		}
	}
}
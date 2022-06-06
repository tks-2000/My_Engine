#pragma once
namespace render {

	static const int RENDER_TARGET_NUM = 6;

	class RenderingEngine;

	/// @brief ディファードレンダリングを行うクラス
	class DeferredRender
	{
	public:
		DeferredRender();
		~DeferredRender();
		void Init(RenderingEngine* renderingEngine);
		void SetPBR(const bool isPBR) { m_isPBR = isPBR; }
		void Execute(RenderContext& rc);
		void Draw(RenderContext& rc);
		void SetDrawModel(Model* model);
		void DeleteDrawModel(Model* model);
		D3D12_CPU_DESCRIPTOR_HANDLE GetGbufferDSV() { return m_rts[0]->GetDSVCpuDescriptorHandle(); }
		
	private:

		bool m_isInitd = false;

		bool m_isPBR = false;

		RenderTarget* m_rts[RENDER_TARGET_NUM] = { nullptr };

		RenderTarget m_albedRT;
		RenderTarget m_normalRT;
		RenderTarget m_metallicAndSmoothRT;
		RenderTarget m_worldPosRT;
		RenderTarget m_normalInViewRT;
		RenderTarget m_depthRT;
		RenderTarget m_lvpRT;


		

		/// @brief ディファードレンダリングを行うモデルの配列
		std::vector<Model*> m_deferredModels;


		SpriteInitData m_defferdLightSpriteInitData;
		Sprite m_defferdLightSprite;

		RenderingEngine* m_renderingEngine = nullptr;
	};
}

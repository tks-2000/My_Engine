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
	/// @brief �����_�����O�ɕK�v�ȋ@�\���܂Ƃ߂��N���X
	class RenderingEngine : public IGameObject
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		bool Start();
		void Update();
		void Render(RenderContext& rc);

		Texture* GetTexture() { return &m_mainRenderTarget.GetRenderTargetTexture(); }

		/// @brief �`�悷�郂�f����ǉ�
		/// @param model �ǉ����郂�f���̃A�h���X
		void SetDrawModel(Model* model);

		/// @brief �`�悵�Ă��郂�f�����폜
		/// @param model �폜���郂�f���̃A�h���X
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

		/// @brief �`�悷��X�v���C�g��ǉ�
		/// @param sprite �ǉ�����X�v���C�g�̃A�h���X
		void SetDrawSprite(Sprite* sprite);

		/// @brief �`�悵�Ă���X�v���C�g���폜
		/// @param sprite �폜����X�v���C�g�̃A�h���X
		void DeleteSprite(Sprite* sprite);

		/// @brief �`�悷��t�H���g��ǉ�
		/// @param font �ǉ�����t�H���g�̃A�h���X
		void SetDrawFont(font::FontData* fontdata);

		/// @brief �`�悵�Ă���t�H���g���폜
		/// @param font �폜����t�H���g�̃A�h���X
		void DeleteFont(font::FontData* fontdata);

		void SetLightFlag(const bool Authenticity) { m_ligFlag = Authenticity; }

		void SetEffectFlag(const bool Authenticity) { m_effectFlag = Authenticity; }

		light::Lighting* GetLighting(){ return &m_lig; }

		const int GetLightingSize(){return sizeof(m_lig); }

		shadow::Shadow* GetShadow() { return &m_shadow; }

		void SetFinalSpriteColor(const Vector4& color) { m_finalSprite.ColorUpdate(color); }

	private:
		/// @brief ���C�g�̍X�V
		void LightUpdate();

		void DrawExpansionModel(int modelGroupNum,RenderContext& rc);

		DeferredRender m_deferredRender;

		RenderTarget m_deferredRenderTarget;

		Sprite m_deferredSprite;
		
		/// @brief ���C�e�B���O
		light::Lighting m_lig;
		/// @brief �V���h�E
		shadow::Shadow m_shadow;
		/// @brief �|�X�g�G�t�F�N�g
		postEffect::PostEffect* m_postEffect = nullptr;

		/// @brief ���C�������_�����O�^�[�Q�b�g
		RenderTarget m_mainRenderTarget;

		/// @brief �`�悷�郂�f���̔z��
		std::vector<Model*> m_drawModels;

		/// @brief �ǉ��ŕ`�悷��`�惂�f���O���[�v�̐�
		enum{EXPANSION_MODEL_GROUP_NUM = 3};

		/// @brief �ǉ��ŕ`�悷�郂�f���O���[�v�̕`��t���O
		bool m_isExpansionModelDraw[EXPANSION_MODEL_GROUP_NUM] = { false };

		/// @brief �ǉ��ŕ`�悷�郂�f���O���[�v
		std::vector<Model*> m_expansionDrawModels[EXPANSION_MODEL_GROUP_NUM];

		/// @brief �ǉ��ŕ`�悷�郌���_�����O�^�[�Q�b�g�����ꂽ���H
		bool m_isCreateExpansionModelRenderTarget[EXPANSION_MODEL_GROUP_NUM] = { false };

		/// @brief �ǉ��ŕ`�悷�郂�f���O���[�v�̕`���̃|�C���^
		RenderTarget* m_expansionModelsRenderTarget[EXPANSION_MODEL_GROUP_NUM] = { nullptr };

		/// @brief �ǉ��ŕ`�悷�郂�f���O���[�v���f���J����
		Camera* m_expansionModelsDrawCamera[EXPANSION_MODEL_GROUP_NUM] = { nullptr };

		/// @brief �`�悷��X�v���C�g�̔z��
		std::vector<Sprite*> m_drawSprites;

		/// @brief �`�悷��t�H���g�̔z��
		std::vector<font::FontData*> m_drawFontsData;

		/// @brief ���C�������_�����O�^�[�Q�b�g�̃X�v���C�g�̏��������
		SpriteInitData m_mainRenderTargetSpriteInitData;
		/// @brief ���C�������_�����O�^�[�Q�b�g�̃X�v���C�g
		Sprite m_mainRenderTargetSprite;

		

		RenderTarget m_finalRenderTarget;

		SpriteInitData m_finalSpriteInitData;

		Sprite m_finalSprite;

		


		Vector3 m_ligColor = g_vec3Zero;

		bool m_ligFlag = false;

		bool m_effectFlag = false;

	};
}
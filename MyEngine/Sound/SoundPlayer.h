#pragma once
namespace sound {

	/// @brief 再生する音声を管理するクラス
	class SoundPlayer : public IGameObject
	{
	public:
		SoundPlayer();
		~SoundPlayer();
		void Init();
		void Execution();

		/// @brief BGMを設定
		/// @param filePath 設定するファイルパス
		/// @return 設定したBGMを指定するID番号
		const int SetBGM(const wchar_t* filePath);
		/// @brief SEを設定
		/// @param filePath 設定するファイルパス 
		/// @return 設定したSEを指定するID番号
		const int SetSE(const wchar_t* filePath);
		/// @brief 音声を設定
		/// @param filePath 設定するファイルパス
		/// @return 設定した音声を指定するID番号
		const int SetVoice(const wchar_t* filePath);

		void SetBGMVolume(const int bgmID, float volume);
		void SetSEVolume(const int seID, float volume);
		void SetVoiceVolume(const int voiceID, float volume);

		/// @brief BGMを再生
		/// @param bgmID 再生するBGMのID番号
		/// @param isLoop ループフラグ
		void PlayBGM(const int bgmID,const bool isLoop = false);
		/// @brief SEを再生
		/// @param seID 再生するSEのID番号
		/// @param isLoop ループフラグ
		void PlaySE(const int seID,const bool isLoop = false);
		/// @brief 音声を再生
		/// @param VoiceID 再生する音声のID番号
		/// @param isLoop ループフラグ
		void PlayVoice(const int VoiceID,const bool isLoop = false);

		void StopBGM(const int bgmID);

		void StopSE(const int seID);

		void StopVoice(const int voiceID);

	private:
		/// @brief ファイルパスが既に入っているか調べる
		/// @param filePaths 調べるファイルパスの配列
		/// @param filePath 配列にあるか調べるファイルパス
		/// @return 中身があればそのファイルの配列のID 中身が無ければ配列の大きさを返す
		const int SearchFile(std::vector<const wchar_t*>& filePaths, const wchar_t* filePath);
		/// @brief 音楽のファイルパス
		std::vector<const wchar_t*> m_bgmFilepath;
		/// @brief 音楽
		std::vector<CSoundSource*> m_bgm;
		/// @brief 効果音のファイルパス
		std::vector<const wchar_t*> m_seFilepath;
		/// @brief 効果音
		std::vector<CSoundSource*> m_se;
		/// @brief 声のファイルパス
		std::vector<const wchar_t*> m_voiceFilepath;
		/// @brief 声
		std::vector<CSoundSource*> m_voice;

	};
}

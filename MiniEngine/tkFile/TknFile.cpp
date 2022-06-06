#include "stdafx.h"
#include "TknFile.h"


bool TknFile::Load(const char* filePath)
{
	auto fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		//ファイルオープン失敗
		return false;
	}
	//ヘッダーをロード
	fread(&m_header, sizeof(m_header), 1, fp);

	//セルの配列を確保
	m_cellArray.resize(m_header.numCell);

	//セルの情報をロード
	fread(&m_cellArray.front(), sizeof(SCell) * m_header.numCell, 1, fp);

	//ファイルを閉じる
	fclose(fp);
}
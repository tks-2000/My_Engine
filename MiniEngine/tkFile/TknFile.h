#pragma once
class TknFile
{
public:
	/// @brief tknファイルのヘッダー
	struct StHeader
	{
		/// @brief セルの数
		std::uint32_t numCell;
	};


	struct SCell {
		/// @brief セルの頂点
		Vector3 vertexPosition[3];
		/// @brief セルの法線
		Vector3 normal;
		/// @brief 隣接セルの番号
		std::int32_t linkCellNo[3];
	};

	/// @brief tknファイルをロードする
	/// @param filePath tknファイルのファイルパス
	/// @return trueでロード成功 falseでロード失敗
	bool Load(const char* filePath);

	int GetNumCell() const
	{
		return m_header.numCell;
	}

	const SCell& GetCell(int cellNo) const
	{
		return m_cellArray.at(cellNo);
	}

private:
	StHeader m_header;
	std::vector<SCell> m_cellArray;
};


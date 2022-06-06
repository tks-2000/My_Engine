#pragma once
#include "Cell.h"
#include "BSP.h"

class TknFile;
namespace nsAI {

	class Cell;

	/// @brief ナビゲーションメッシュを読み込むクラス
	class NaviMesh
	{
	public:
		/// @brief tknファイルからナビゲーションメッシュを構築する
		/// @param tknFilePath 
		void Init(const char* tknFilePath);

		/// @brief 指定した座標に最も近いセルを検索する
		/// @param pos 
		/// @return 
		const Cell& FindNearestCell(const Vector3& pos) const;

		/// @brief セルの数を取得
		/// @return 
		int GetNumCell() const
		{
			return static_cast<int>(m_cellArray.size());
		}

		/// @brief セルを取得
		/// @param cellNo 
		/// @return 
		const Cell& GetCell(int cellNo) const
		{
			return m_cellArray.at(cellNo);
		}

	private:
		std::vector<Cell> m_cellArray;
		BSP m_cellCenterPosBSP;
	};

}
//--------------------------------------------------------------------------------------
// File: SpriteFont3D.h
//
// 文字フォント描画クラス（３Ⅾ版）
//
// Date: 2020.8.31
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <string>

#define font3D SpriteFont3D::GetInstance()

class SpriteFont3D
{
private:

	// 文字列情報
	struct String
	{
		// 位置
		DirectX::SimpleMath::Vector3 position;

		// 文字列
		std::wstring string;

		// 色
		DirectX::SimpleMath::Color color;

		// 大きさ
		float size;
	};

private:

	// フォントサイズ
	static const float DEFAULT_FONT_SIZE;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 表示文字列の配列
	std::vector<String> m_strings;

	// コンストラクタ
	SpriteFont3D();

	void operator=(const SpriteFont3D& obj) = delete;	// 代入演算子削除
	SpriteFont3D(const SpriteFont3D &obj) = delete;		// コピーコンストラクタ削除

public:

	// インスタンスを取得する関数
	static SpriteFont3D* GetInstance();

	// 描画する文字列を登録する関数
	void AddString(const wchar_t* string, DirectX::SimpleMath::Vector3 pos, DirectX::FXMVECTOR color = DirectX::Colors::White, float size = DEFAULT_FONT_SIZE);

	// 描画関数
	void Render();
};


//--------------------------------------------------------------------------------------
// File: SpriteFont3D.cpp
//
// 文字フォント描画クラス（３Ⅾ版）
//
// Date: 2020.8.31
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SpriteFont3D.h"
#include "Cgdi.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// フォントサイズ
const float SpriteFont3D::DEFAULT_FONT_SIZE = 0.5f;

// コンストラクタ
SpriteFont3D::SpriteFont3D()
{
	auto device = gdi->GetDeviceResources()->GetD3DDevice();
	auto context = gdi->GetDeviceResources()->GetD3DDeviceContext();

	BasicEffect* effect = gdi->GetEffect();
	effect->SetTextureEnabled(true);
	effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 入力レイアウトの作成
	device->CreateInputLayout(VertexPositionColorTexture::InputElements, VertexPositionColorTexture::InputElementCount, shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
}

// インスタンスを取得する関数
SpriteFont3D * SpriteFont3D::GetInstance()
{
	static SpriteFont3D instance;

	return &instance;
}

// 描画する文字列を登録する関数
void SpriteFont3D::AddString(const wchar_t * string, DirectX::SimpleMath::Vector3 pos, FXMVECTOR color, float size)
{
	String str;

	str.string = std::wstring(string);
	str.position = pos;
	str.color = color;
	str.size = size;

	m_strings.push_back(str);
}

// 描画関数
void SpriteFont3D::Render()
{
	auto context = gdi->GetDeviceResources()->GetD3DDeviceContext();
	SpriteBatch* sprite = gdi->GetSprite();
	SpriteFont* font = gdi->GetFont();
	BasicEffect* effect = gdi->GetEffect();

	// 先に設定可能な物は設定しておく
	effect->SetTextureEnabled(true);
	effect->SetVertexColorEnabled(true);
	effect->SetView(gdi->GetViewMatrix());
	effect->SetProjection(gdi->GetProjectionMatrix());

	// ビュー行列の回転を打ち消す行列を作成する
	Matrix invView = gdi->GetViewMatrix().Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// スクリーン座標はY軸が＋－逆なので
	Matrix scaleY;
	scaleY._22 = -1.0f;

	for (size_t i = 0; i < m_strings.size(); i++)
	{
		sprite->Begin(SpriteSortMode_Deferred, nullptr, nullptr, gdi->GetStates()->DepthNone(), gdi->GetStates()->CullNone(), [=] 
		{
			// ワールド行列作成
			Matrix world = scaleY * invView * Matrix::CreateTranslation(m_strings[i].position);
			effect->SetWorld(world);
			effect->Apply(context);
			context->IASetInputLayout(m_inputLayout.Get());
		});

		Vector2 textOrigin = font->MeasureString(m_strings[i].string.c_str()) / 2.0f;
		const wchar_t* str = m_strings[i].string.c_str();
		font->DrawString(sprite, m_strings[i].string.c_str(), Vector2::Zero, m_strings[i].color, 0.0f, textOrigin, m_strings[i].size);

		sprite->End();
	}

	// 登録されている文字列をクリア
	m_strings.clear();
}

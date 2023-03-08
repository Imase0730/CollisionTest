//--------------------------------------------------------------------------------------
// File: DisplayCollision.cpp
//
// コリジョン表示クラス（デバッグ用）
//
// Date: 2023.3.8
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "DisplayCollision.h"

using namespace DirectX;
using namespace Imase;

// 球のラインを描画する関数
void DisplayCollision::DrawSphereLine(
	PrimitiveBatch<VertexPositionColor>* primitiveBatch,
	const Sphere& sphere)
{
	int vertices = 16;

	VertexPositionColor* v = new VertexPositionColor[vertices * 2 * 3];
	float centralAngle = XM_2PI / static_cast<float>(vertices);
	float angle_a = 0.0f;
	SimpleMath::Color color;
	SimpleMath::Vector3 pos[2];

	// 高速化のため頂点情報をまとめて描画している
	for (int i = 0; i < vertices; i++)
	{
		// 始点と終点のコサインとサインを先に計算しておく
		float angle_b = angle_a + centralAngle;
		float cos_a = cosf(angle_a);
		float sin_a = sinf(angle_a);
		float cos_b = cosf(angle_b);
		float sin_b = sinf(angle_b);

		for (int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:		// Roll
				color = Colors::GreenYellow;
				pos[0] = SimpleMath::Vector3(cos_a, sin_a, 0.0f);
				pos[1] = SimpleMath::Vector3(cos_b, sin_b, 0.0f);
				break;
			case 1:		// Pitch
				color = Colors::OrangeRed;
				pos[0] = SimpleMath::Vector3(0.0f, sin_a, cos_a);
				pos[1] = SimpleMath::Vector3(0.0f, sin_b, cos_b);
				break;
			default:	// Yaw
				color = Colors::Blue;
				pos[0] = SimpleMath::Vector3(cos_a, 0.0f, sin_a);
				pos[1] = SimpleMath::Vector3(cos_b, 0.0f, sin_b);
				break;
			}
			SimpleMath::Vector3::Transform(pos[0], sphere.rotate, pos[0]);
			v[i * 6 + j * 2] = VertexPositionColor(pos[0] * sphere.radius + sphere.center, color);
			SimpleMath::Vector3::Transform(pos[1], sphere.rotate, pos[1]);
			v[i * 6 + j * 2 + 1] = VertexPositionColor(pos[1] * sphere.radius + sphere.center, color);
		}

		angle_a = angle_b;
	}

	// 線を描画
	primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, v, vertices * 2 * 3);

	delete[] v;
}

// コンストラクタ
DisplayCollision::DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// モデルの作成（球）
	m_modelSphere = GeometricPrimitive::CreateSphere(context, 2.0f, 8);

	// モデルの作成（ボックス）
	m_modelBox = GeometricPrimitive::CreateCube(context);

	// プリミティブバッチの作成（ライン用）
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// ----- エフェクト ----- //

	// エフェクトの作成（モデル用）
	m_modelEffect = std::make_unique<NormalMapEffect>(device);
	m_modelEffect->SetVertexColorEnabled(false);
	m_modelEffect->SetBiasedVertexNormals(false);
	m_modelEffect->SetInstancingEnabled(true);
	m_modelEffect->SetFogEnabled(false);
	m_modelEffect->SetTexture(nullptr);
	m_modelEffect->DisableSpecular();
	m_modelEffect->EnableDefaultLighting();
	m_modelEffect->SetWorld(SimpleMath::Matrix::Identity);

	// エフェクトの作成（ライン用）
	m_lineEffect = std::make_unique<BasicEffect>(device);
	m_lineEffect->SetLightingEnabled(false);
	m_lineEffect->SetTextureEnabled(false);
	m_lineEffect->SetVertexColorEnabled(true);
	m_lineEffect->SetWorld(SimpleMath::Matrix::Identity);

	// ----- 入力レイアウト ----- //

	// 入力レイアウトの作成（モデル用）
	const D3D11_INPUT_ELEMENT_DESC c_InputElements[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "InstMatrix",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "InstMatrix",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "InstMatrix",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(device, m_modelEffect.get(),
			c_InputElements, std::size(c_InputElements),
			m_modelInputLayout.ReleaseAndGetAddressOf())
	);

	// 入力レイアウトの作成（ライン用）
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_lineEffect.get(),
			m_lineInputLayout.ReleaseAndGetAddressOf())
	);

	// ----- 定数バッファ ----- //

	// インスタンシング用のワールド行列格納用定数バッファを作成
	auto desc = CD3D11_BUFFER_DESC(
		static_cast<UINT>(DISPLAY_COLLISION_MAX * sizeof(XMFLOAT3X4)),
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);
	DX::ThrowIfFailed(
		device->CreateBuffer(&desc, nullptr,
			m_instancedVB.ReleaseAndGetAddressOf())
	);

}

// 登録されたコリジョンの描画関数
void DisplayCollision::DrawCollision(
	ID3D11DeviceContext* context,
	CommonStates* states,
	const SimpleMath::Matrix& view,
	const SimpleMath::Matrix& proj,
	DirectX::SimpleMath::Color modelColor)
{
	// 登録数が最大表示数を超えていないか？
	assert(static_cast<uint32_t>(m_spheres.size()) < DISPLAY_COLLISION_MAX);
	assert(static_cast<uint32_t>(m_boxes.size()) < DISPLAY_COLLISION_MAX);

	// ----- 球のモデルを描画 ----- //

	// エフェクトを適応する
	m_modelEffect->SetColorAndAlpha(modelColor);
	m_modelEffect->SetView(view);
	m_modelEffect->SetProjection(proj);
	m_modelEffect->Apply(context);

	{
		// ワールド行列を設定
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(m_instancedVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMFLOAT3X4* p = static_cast<XMFLOAT3X4*>(mappedResource.pData);
		ZeroMemory(p, sizeof(XMFLOAT3X4) * m_spheres.size());
		for (int i = 0; i < m_spheres.size(); i++)
		{
			// 拡大縮小
			p[i]._11 = p[i]._22 = p[i]._33 = m_spheres[i].radius;
			// 位置
			p[i]._14 = m_spheres[i].center.x;
			p[i]._24 = m_spheres[i].center.y;
			p[i]._34 = m_spheres[i].center.z;
		}
		context->Unmap(m_instancedVB.Get(), 0);
	}

	// 球をインスタンスし描画している（高速化のため）
	m_modelSphere->DrawInstanced(m_modelEffect.get(), m_modelInputLayout.Get(), static_cast<uint32_t>(m_spheres.size()), true, false, 0, [=]()
		{
			UINT stride = sizeof(XMFLOAT3X4);
			UINT offset = 0;
			context->OMSetDepthStencilState(states->DepthRead(), 0);
			context->IASetVertexBuffers(1, 1, m_instancedVB.GetAddressOf(), &stride, &offset);
		}
	);

	// ----- ボックスのモデルを描画 ----- //

	{
		// ワールド行列を設定
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(m_instancedVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMFLOAT3X4* p = static_cast<XMFLOAT3X4*>(mappedResource.pData);
		ZeroMemory(p, sizeof(XMFLOAT3X4) * m_boxes.size());
		for (int i = 0; i < m_boxes.size(); i++)
		{
			// 拡大縮小
			p[i]._11 = m_boxes[i].extents.x * 2.0f;
			p[i]._22 = m_boxes[i].extents.y * 2.0f;
			p[i]._33 = m_boxes[i].extents.z * 2.0f;
			// 位置
			p[i]._14 = m_boxes[i].center.x;
			p[i]._24 = m_boxes[i].center.y;
			p[i]._34 = m_boxes[i].center.z;
		}
		context->Unmap(m_instancedVB.Get(), 0);
	}

	// ボックスをインスタンスし描画している（高速化のため）
	m_modelBox->DrawInstanced(m_modelEffect.get(), m_modelInputLayout.Get(), static_cast<uint32_t>(m_boxes.size()), true, false, 0, [=]()
		{
			UINT stride = sizeof(XMFLOAT3X4);
			UINT offset = 0;
			context->OMSetDepthStencilState(states->DepthRead(), 0);
			context->IASetVertexBuffers(1, 1, m_instancedVB.GetAddressOf(), &stride, &offset);
		}
	);

	// ----- ラインの描画 ----- //
	context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());

	// エフェクトを適応する
	m_lineEffect->SetView(view);
	m_lineEffect->SetProjection(proj);
	m_lineEffect->Apply(context);

	// 入力レイアウトの設定
	context->IASetInputLayout(m_lineInputLayout.Get());

	// 球のラインを描画する
	m_primitiveBatch->Begin();
	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		DrawSphereLine(m_primitiveBatch.get(), m_spheres[i]);
	}
	m_primitiveBatch->End();

	// 登録された表示情報をクリアする
	m_spheres.clear();
	m_boxes.clear();
}



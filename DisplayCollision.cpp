#include "pch.h"
#include "DisplayCollision.h"

using namespace DirectX;

void DisplayCollision::DrawSphere(PrimitiveBatch<VertexPositionColor>* primitiveBatch, const CollisionSphere& sphere)
{
	VertexPositionColor v[2] = {};

	int vertices = 16;

	float centralAngle = XM_2PI / static_cast<float>(vertices);
	float angle = 0.0f;

	SimpleMath::Vector3 pos, tmp;

	SimpleMath::Matrix m = SimpleMath::Matrix::CreateFromQuaternion(sphere.rotate);

	// Y軸の輪
	for (int i = 0; i < vertices; i++)
	{
		pos.x = cosf(angle);
		pos.y = sinf(angle);
		pos.z = 0.0f;
		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);

		v[0] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::GreenYellow);
		angle += centralAngle;
		pos.x = cosf(angle);
		pos.y = sinf(angle);
		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);

		v[1] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::GreenYellow);
		primitiveBatch->DrawLine(v[0], v[1]);
	}

	// X軸の輪
	for (int i = 0; i < vertices; i++)
	{
		pos.x = 0;
		pos.y = sinf(angle);
		pos.z = cosf(angle);

		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);

		v[0] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::OrangeRed);
		angle += centralAngle;
		pos.y = sinf(angle);
		pos.z = cosf(angle);
		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);
		v[1] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::OrangeRed);
		primitiveBatch->DrawLine(v[0], v[1]);
	}

	// Y軸の輪
	for (int i = 0; i < vertices; i++)
	{
		pos.x = cosf(angle);
		pos.y = 0.0f;
		pos.z = sinf(angle);
		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);
		v[0] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::Blue);
		angle += centralAngle;
		pos.x = cosf(angle);
		pos.z = sinf(angle);
		SimpleMath::Vector3::Transform(pos, sphere.rotate, tmp);
		v[1] = VertexPositionColor(tmp * sphere.radius + sphere.center, Colors::Blue);
		primitiveBatch->DrawLine(v[0], v[1]);
	}
}

DisplayCollision::DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context)
{
	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// エフェクトの作成
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetLightingEnabled(false);
	m_effect->SetTextureEnabled(false);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetWorld(SimpleMath::Matrix::Identity);

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	m_effectFactory = std::make_unique<DGSLEffectFactory>(device);
	m_model = Model::CreateFromCMO(device, L"Resources/CollisionSphere.cmo", *m_effectFactory.get());
}

void DisplayCollision::DrawCollision(
	ID3D11DeviceContext* context,
	CommonStates* states,
	const SimpleMath::Matrix& view,
	const SimpleMath::Matrix& proj)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::CreateScale(3);
	m_model->Draw(context, *states, world, view, proj, false, [&]()
		{
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullCounterClockwise());
		}
	);

	context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());

	// エフェクトを適応する
	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->Apply(context);

	context->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->Begin();

	// 球を描画する
	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		DrawSphere(m_primitiveBatch.get(), m_spheres[i]);
	}

	m_primitiveBatch->End();


}

void DisplayCollision::AddColiision(DirectX::SimpleMath::Vector3 center, float radius
	, DirectX::SimpleMath::Quaternion rotate)
{
	CollisionSphere sphere;

	sphere.center = center;
	sphere.radius = radius;
	sphere.rotate = rotate;
	m_spheres.push_back(sphere);
}

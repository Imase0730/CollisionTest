#include "pch.h"
#include "DisplayCollision.h"

using namespace DirectX;

void DisplayCollision::DrawSphere(PrimitiveBatch<VertexPositionColor>* primitiveBatch, const CollisionSphere& sphere)
{
	VertexPositionColor v[2] = {};

	int vertices = 3;

	for (int i = 0; i < vertices; i++)
	{

		primitiveBatch->DrawLine(v[0], v[1]);
	}
}

DisplayCollision::DisplayCollision(ID3D11Device* device)
{
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(device);

	// �G�t�F�N�g�̍쐬
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetLightingEnabled(false);
	m_effect->SetTextureEnabled(false);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetWorld(SimpleMath::Matrix::Identity);

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

void DisplayCollision::DrawCollision(
	ID3D11DeviceContext* context,
	CommonStates* states,
	const SimpleMath::Matrix& view,
	const SimpleMath::Matrix& proj)
{
	context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthNone(), 0);

	// �G�t�F�N�g��K������
	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->Apply(context);

	m_primitiveBatch->Begin();

	// ����`�悷��
	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		DrawSphere(m_primitiveBatch.get(), m_spheres[i]);
	}

	m_primitiveBatch->End();
}

//--------------------------------------------------------------------------------------
// File: DisplayCollision.h
//
// �R���W�����\���N���X�i�f�o�b�O�p�j
//
// Usage: AddBoundingSphere��AddBoundingBox�œo�^���Ă��������B
//        DrawCollision�֐��ŕ\�����܂��B�o�^���ꂽ���͕`���N���A����܂��B
//        ���f�����̏Փ˔���p�̃R���W�������̕\���ȂǂɎg�p���Ă��������B
//        �{�b�N�X�͉�]�ɂ͑Ή����Ă��܂���B�iAABB�̏Փ˔���p�j
//
// Date: 2023.3.8
// Author: Hideyasu Imase
//
//--------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include "SimpleMath.h"
#include "CommonStates.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "DirectXHelpers.h"

namespace Imase
{

	class DisplayCollision
	{
	private:

		// �\���\�ȃR���W�����̍ő吔
		static const uint32_t DISPLAY_COLLISION_MAX = 100;

		// ���̏��
		struct Sphere
		{
			DirectX::SimpleMath::Vector3 center;	// ���S
			float radius;							// ���a

			constexpr Sphere(const DirectX::SimpleMath::Vector3& center, float radius) noexcept
				: center(center), radius(radius) {}
		};

		// �{�b�N�X�̏��
		struct Box
		{
			DirectX::SimpleMath::Vector3 center;	// ���S
			DirectX::SimpleMath::Vector3 extents;	// �e�ʂ̒��S����̋���.

			constexpr Box(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& extents) noexcept
				: center(center), extents(extents) {}
		};

		// ���̃R���W�������
		std::vector<Sphere> m_spheres;

		// �{�b�N�X�̃R���W�������
		std::vector<Box> m_boxes;

		// ���̃��f��
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelSphere;

		// �{�b�N�X�̃��f��
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelBox;

		// �G�t�F�N�g�i���f���p�j
		std::unique_ptr<DirectX::NormalMapEffect> m_modelEffect;

		// ���̓��C�A�E�g�i���f���p�j
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_modelInputLayout;

		// �C���X�^���X�p���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_instancedVB;

	public:

		// �R���X�g���N�^
		DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

		// �o�^���ꂽ�R���W�����̕`��֐�
		void DrawCollision(
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& proj,
			DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,0.5f)
		);
		
		// ���̃R���W������o�^����֐�
		void AddBoundingSphere(DirectX::BoundingSphere shpere, DirectX::SimpleMath::Vector3 pos)
		{
			DirectX::XMFLOAT3 center = shpere.Center + pos;
			m_spheres.push_back(Sphere(center, shpere.Radius));
		}

		// �{�b�N�X�̃R���W������o�^����֐�
		void AddBoundingBox(DirectX::BoundingBox box, DirectX::SimpleMath::Vector3 pos)
		{
			DirectX::XMFLOAT3 center = box.Center + pos;
			m_boxes.push_back(Box(center, box.Extents));
		}

	};

}



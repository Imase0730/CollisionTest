//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "DisplayCollision.h"
#include "DebugCamera.h"
#include "DebugFont.h"
#include "TaskManager.h"
#include "Player.h"
#include "DebugDraw.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

private:

    // ���ʃX�e�[�g
    std::unique_ptr<DirectX::CommonStates> m_states;

    // �G�t�F�N�g�t�@�N�g���[
    std::unique_ptr<DirectX::EffectFactory> m_EffectFactory;

    // ���̃��f��
    std::unique_ptr<DirectX::Model> m_floorModel;

    // �J�����i�f�o�b�O�p�j
    std::unique_ptr<Imase::DebugCamera> m_debugCamera;

    // �e�X�g�p���f��
    std::unique_ptr<DirectX::Model> m_monkeyModel;

    // �e�X�g�p���f���̕\���ʒu
    DirectX::SimpleMath::Vector3 m_monkeyPos;

    std::unique_ptr<DirectX::DGSLEffectFactory> m_DGSLEffectFactory;
    std::unique_ptr<DirectX::Model> m_shieldModel;


    // �R���W�������������邽�߂̃I�u�W�F�N�g�i�f�o�b�O�p�j
    std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

    // �f�o�b�O�p�t�H���g�o�̓I�u�W�F�N�g
    std::unique_ptr<Imase::DebugFont> m_font;

    // �f�o�b�O�p�t�H���g�o�̓I�u�W�F�N�g�i3D�Łj
    std::unique_ptr<Imase::DebugFont3D> m_font3D;

    // �^�X�N�}�l�[�W���[
    std::unique_ptr<Imase::TaskManager> m_taskManager;

    // �v���C���[�^�X�N
    Player* m_playerTask;

    // ���̃��f��
    std::unique_ptr<DirectX::Model> m_model;

    // �v���~�e�B�u�o�b�`
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;


};

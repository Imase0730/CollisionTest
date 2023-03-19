//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    //: m_monkeyPos{}
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    // �f�o�b�O�J�����̍쐬
    m_debugCamera = std::make_unique<Imase::DebugCamera>(width, height);

    // �^�X�N�}�l�[�W���[�̍쐬
    m_taskManager = std::make_unique<Imase::TaskManager>();

    // �v���C���[�^�X�N��o�^
    m_playerTask = m_taskManager->AddTask<Player>(m_deviceResources.get(), m_states.get(), m_model.get());

 /*   m_playerTask->Kill();
    m_playerTask = nullptr;*/
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

    // �^�X�N�̍X�V
    m_taskManager->Update(elapsedTime);

    // �e�X�g�p���f���̕\���ʒu��ݒ�
    m_monkeyPos = SimpleMath::Vector3(2.0f, 1.0f, -3.0f);

    // �e�X�g�p���f���̃R���W������o�^
    for (size_t i = 0; i < m_monkeyModel->meshes.size(); i++)
    {
        // ��
        m_displayCollision->AddBoundingSphere(m_monkeyModel->meshes[i]->boundingSphere, m_monkeyPos);
        // �{�b�N�X
//        m_displayCollision->AddBoundingBox(m_monkeyModel->meshes[i]->boundingBox, m_monkeyPos);
    }

    // ���̃R���W������o�^
    for (size_t i = 0; i < m_shieldModel->meshes.size(); i++)
    {
        m_displayCollision->AddBoundingBox(m_shieldModel->meshes[i]->boundingBox, SimpleMath::Vector3(0, 0, 0));
    }

    // �e�X�g�p�̃t�H���g��o�^
    m_font3D->AddString(L"Monkey", m_monkeyPos, Colors::Yellow, 1.0f);
    m_font->AddString(L"IMASE", SimpleMath::Vector2(0.0f, 0.0f), Colors::Magenta);

    // �f�o�b�O�J�����̍X�V
    m_debugCamera->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    SimpleMath::Matrix world, view, proj;

    // �r���[�s��̍쐬
    view = m_debugCamera->GetCameraMatrix();

    // �ˉe�s��̍쐬
    int width, height;
    GetDefaultSize(width, height);
    proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    // ���̕`��
    m_floorModel->Draw(context, *m_states.get(), world, view, proj, false, [&]()
        {
            ID3D11SamplerState* samplers[] = { m_states->PointWrap() };
            context->PSSetSamplers(0, 1, samplers);
        }
    );

    // �e�X�g�p���f���̕`��
    world = SimpleMath::Matrix::CreateTranslation(m_monkeyPos);
    m_monkeyModel->Draw(context, *m_states.get(), world, view, proj);


    // ����`��
    world = SimpleMath::Matrix::Identity;
    m_shieldModel->Draw(context, *m_states.get(), world, view, proj);


    // �o�^���ꂽ�R���W�����̕`��
    m_displayCollision->DrawCollision(context, m_states.get(), view, proj);

    // �f�o�b�O�p�t�H���g�̕`��
    m_font3D->Render(context, m_states.get(), view, proj);
    m_font->Render(m_states.get());

    if (m_playerTask)
    {
        m_playerTask->SetViewMatrix(view);
        m_playerTask->SetProjectionMatrix(proj);
    }

    // �^�X�N�̕`��
    m_taskManager->Render();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    // ���ʃX�e�[�g�̍쐬
    m_states = std::make_unique<CommonStates>(device);

    // �����f���̓ǂݍ���
    m_EffectFactory = std::make_unique<EffectFactory>(device);
    m_floorModel = Model::CreateFromCMO(device, L"Resources/floor.cmo", *m_EffectFactory.get());

    // �e�X�g�p���f���̓ǂݍ���
    m_monkeyModel = Model::CreateFromCMO(device, L"Resources/monkey.cmo", *m_EffectFactory.get());

    // �����f���̓ǂݍ���
    m_DGSLEffectFactory = std::make_unique<DGSLEffectFactory>(device);
    m_shieldModel = Model::CreateFromCMO(device, L"Resources/Shield.cmo", *m_DGSLEffectFactory.get());

    // �R���W�������������邽�߂̃I�u�W�F�N�g���쐬
    m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

    // �f�o�b�O�p�t�H���g�o�̓I�u�W�F�N�g���쐬
    m_font = std::make_unique<Imase::DebugFont>(device, context, L"Resources/SegoeUI_18.spritefont");
    m_font3D = std::make_unique<Imase::DebugFont3D>(device, context, L"Resources/SegoeUI_18.spritefont");

    // ���f���̓ǂݍ���
    EffectFactory effectFactory(device);
    m_model = Model::CreateFromCMO(device, L"Resources/A.cmo", effectFactory);

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_states.reset();
    m_EffectFactory.reset();
    m_floorModel.reset();
    m_monkeyModel.reset();
    m_DGSLEffectFactory.reset();
    m_shieldModel.reset();
    m_displayCollision.reset();
    m_font.reset();
    m_font3D.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

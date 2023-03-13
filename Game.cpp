//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
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
    m_camera = std::make_unique<Imase::DebugCamera>(800, 600);
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

    m_pos = SimpleMath::Vector3(2, 0, 0);

    SimpleMath::Matrix world;

 //   for (size_t i = 0; i < m_monkeyModel->meshes.size(); i++)
 //   {
 //       //DirectX::BoundingSphere sphere = m_monkeyModel->meshes[i]->boundingSphere;

 //       //world = SimpleMath::Matrix::CreateTranslation(m_pos);
 //       //XMVECTOR center = XMLoadFloat3(&sphere.Center);
 //       //center = XMVector3Transform(center, world);
 //       //XMStoreFloat3(&sphere.Center, center);

 //       m_collision->AddBoundingSphere(m_monkeyModel->meshes[i]->boundingSphere, m_pos);
 //       m_collision->AddBoundingBox(m_monkeyModel->meshes[i]->boundingBox, m_pos);


 ///*       DirectX::BoundingBox box = m_monkeyModel->meshes[i]->boundingBox;
 //       world = SimpleMath::Matrix::CreateTranslation(m_pos);
 //       XMVECTOR center = XMLoadFloat3(&box.Center);
 //       center = XMVector3Transform(center, world);
 //       XMStoreFloat3(&box.Center, center);
 //       m_collision->AddBoxCollision(box);*/
 //   }

  /*  for (size_t i = 0; i < m_shieldModel->meshes.size(); i++)
    {
        m_collision->AddBoundingBox(m_shieldModel->meshes[i]->boundingBox, SimpleMath::Vector3(0, 0, 0));
    }*/
    BoundingSphere s = { XMFLOAT3(0,0,0), 1.0f };
    m_collision->AddBoundingSphere(s, SimpleMath::Vector3::Zero);

    m_font3D->AddString(L"IMASE", SimpleMath::Vector3(0, 1, 0), Colors::Black, 0.5f);
    m_font->AddString(L"IMASE", SimpleMath::Vector2(0, 0), Colors::Black);

    m_camera->Update();
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

 //   view = SimpleMath::Matrix::CreateLookAt(SimpleMath::Vector3(0.0f, 1.0f, 10.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3::UnitY);
    view = m_camera->GetCameraMatrix();
    proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_floorModel->Draw(context, *m_states.get(), world, view, proj, false, [&]()
        {
            ID3D11SamplerState* samplers[] = { m_states->PointWrap() };
            context->PSSetSamplers(0, 1, samplers);
        }
    );

    //world = SimpleMath::Matrix::CreateTranslation(m_pos);
    //m_monkeyModel->Draw(context, *m_states.get(), world, view, proj);


    //world = SimpleMath::Matrix::Identity;
    //m_shieldModel->Draw(context, *m_states.get(), world, view, proj);


    m_collision->DrawCollision(context, m_states.get(), view, proj);

    m_font3D->Render(context, m_states.get(), view, proj);
    m_font->Render(m_states.get());

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

    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    m_states = std::make_unique<CommonStates>(device);

    m_EffectFactory = std::make_unique<EffectFactory>(device);
    m_floorModel = Model::CreateFromCMO(device, L"Resources/floor.cmo", *m_EffectFactory.get());

    m_monkeyModel = Model::CreateFromCMO(device, L"Resources/monkey.cmo", *m_EffectFactory.get());


    m_DGSLEffectFactory = std::make_unique<DGSLEffectFactory>(device);
    m_shieldModel = Model::CreateFromCMO(device, L"Resources/Shield.cmo", *m_DGSLEffectFactory.get());


    auto context = m_deviceResources->GetD3DDeviceContext();
    m_collision = std::make_unique<Imase::DisplayCollision>(device, context);

    m_font = std::make_unique<Imase::DebugFont>(device, context, L"Resources/SegoeUI_18.spritefont");
    m_font3D = std::make_unique<Imase::DebugFont3D>(device, context, L"Resources/SegoeUI_18.spritefont");
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

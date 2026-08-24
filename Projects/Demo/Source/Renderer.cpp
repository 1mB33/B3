#include "MyGame.hpp"
#include "Renderer.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "Editor/EditorPipeline.hpp"
#include "MainWindow.hpp"
#include "Vulkan/BasicComponentImplementations/GraphicsComputeAdapter.hpp"
#include "Vulkan/BasicComponentImplementations/MinimalHardware.hpp"

using namespace B33;
using namespace B33::Math;

void Renderer::Initialize( ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"Renderer initialize" );
    auto windowHandle = bridge.QueryComponent<MainWindow>();
    auto gameHandle   = bridge.QueryComponent<MyGame>();

    if ( auto lockedInput = windowHandle->GetWindowInstance().GetInput().lock() )
    {
        m_RendererMaster.BindToInput(
            lockedInput,
            {
                { RendererMasterController::UseActionSwitchDebugMode,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_Z } } },
            } );
    }

    m_RendererInstance.Initialize<Rendering::MinimalHardware, Rendering::GraphicsComputeAdapter>(
        windowHandle->GetWindowInstance().GetWindowDesc() );

    m_RendererInstance.PushPipeline<::B33::Rendering::VoxelPipeline>( gameHandle->GetGameInstance().GetWorld() );
}

void Renderer::Update( float fDelta, ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"Renderer update" );
    auto  gameComonentHandle = bridge.QueryComponent<MyGame>();
    auto &characterHandle    = gameComonentHandle->GetMainCharacter().GetObject();
    auto &gameHandle         = gameComonentHandle->GetGameInstance();

    const Vec3   rot         = characterHandle.GetRotation();
    const Vec3   rotVec      = Normalize( RotateY( RotateX( Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );
    const Vec3   cameraRight = Normalize( Cross( rotVec, Vec3 { 0.f, -1.f, 0.f } ) );
    const Vec3   cameraUp    = Cross( cameraRight, rotVec );
    const size_t uWorldWidth = gameHandle.GetWorld()->GetGridWidth();

    B33::Rendering::VoxelPushConstants constants = {
        .CameraPos     = characterHandle.GetPosition(),
        .GridSize      = iVec3( uWorldWidth, uWorldWidth, uWorldWidth ),
        .CameraLookDir = rotVec,
        .CameraRight   = cameraRight,
        .CameraUp      = cameraUp,
        .fFov          = B33_DEG_TO_RAD( characterHandle.GetFov() ),
        .uMode         = m_RendererMaster.GetObject()->GetDebugMode(),
    };

    m_RendererInstance.GetPipeline<Rendering::VoxelPipeline>()->LoadPushConstants( constants );
    m_RendererInstance.Update( fDelta );
    m_RendererInstance.Render();
}

void Renderer::Destroy( ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"Renderer destroy" );
    m_RendererInstance.Destroy();
}

#include "MyGame.hpp"
#include "Renderer.hpp"
#include "Raycaster/VoxelPipeline.hpp"
#include "Editor/EditorPipeline.hpp"
#include "MainWindow.hpp"

using namespace B33;
using namespace B33::Math;

void Renderer::Initialize( ::B33::System::ComponentBridge &bridge )
{
    auto input = bridge.QueryComponent<MainWindow>().GetWindowInstance().GetInput();
    if ( auto locked = input.lock() )
    {
        m_RendererMaster.BindToInput(
            locked,
            {
                { RendererMasterController::UseActionSwitchDebugMode,
                  AbInputBind {
                      .Type     = Keyboard,
                      .Keyboard = AbKeyboardBind { .KeyState = EAbOnState::Press, .KeyCode = B33::App::B33_KEY_Z } } },
            } );
    }

    m_RendererInstance.Initialize( bridge.QueryComponent<MainWindow>().GetWindowInstance().GetWindowDesc() );

    m_RendererInstance.PushPipeline<::B33::Rendering::VoxelPipeline>(
        bridge.QueryComponent<MyGame>().GetGameInstance().GetWorld() );

    m_RendererInstance.PushPipeline<::B33::Rendering::EditorPipeline>();
}

void Renderer::Update( float fDelta, ::B33::System::ComponentBridge &bridge )
{
    B33_TRACE( L"Renderer is updated, delta = %f", fDelta );
    auto &characterHandle = bridge.QueryComponent<MyGame>().GetMainCharacter().GetObject();
    auto &gameHandle      = bridge.QueryComponent<MyGame>().GetGameInstance();

    const Vec3 rot         = characterHandle.GetRotation();
    const Vec3 rotVec      = Normalize( RotateY( RotateX( Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );
    const Vec3 cameraRight = Normalize( Cross( rotVec, Vec3 { 0.f, -1.f, 0.f } ) );
    const Vec3 cameraUp    = Cross( cameraRight, rotVec );

    B33::Rendering::VoxelPushConstants constants = {};
    constants.fFov                               = characterHandle.GetFov() * B33_DEG_TO_RAD;
    constants.CameraPos                          = characterHandle.GetPosition();
    constants.CameraLookDir                      = rotVec;
    constants.CameraRight                        = cameraRight;
    constants.CameraUp                           = cameraUp;
    const size_t uWorldWidth                     = gameHandle.GetWorld()->GetGridWidth();
    constants.GridSize                           = iVec3( uWorldWidth, uWorldWidth, uWorldWidth );
    constants.uMode                              = m_RendererMaster.GetObject().GetDebugMode();

    m_RendererInstance.GetPipeline<Rendering::VoxelPipeline>()->LoadPushConstants( constants, sizeof( constants ) );
    m_RendererInstance.GetPipeline<Rendering::EditorPipeline>()->LoadPushConstants( constants, sizeof( constants ) );
    m_RendererInstance.Update( fDelta );
    m_RendererInstance.Render();
}

void Renderer::Destroy( ::B33::System::ComponentBridge &bridge )
{
    m_RendererInstance.Destroy();
}

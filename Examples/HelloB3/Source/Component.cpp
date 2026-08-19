// Every executable needs exactly one entry point, which
// wires up the platform-specific startup code and hands control over to B3.
// Here we use the universal entry point, which compiles unmodified on
// Windows, Linux, and macOS.
#include <EntryPoints/DefaultSystemEntryPoint.hpp>

#include "Component.hpp"

void BasicComponent::Initialize( ComponentBridge & )
{
    B33_INFO( L"Hello B3!" ); // Log an info-level message through the global logger.

    // By default the application has no windows, and the app status is tied
    // to window lifetime: with no windows to keep it alive, the app would
    // exit immediately after Initialize() finishes.
    //
    // Since we rely on Update() being called every frame, we need the app to
    // keep running regardless of windows. LockInToService() overrides the
    // default behaviour and keeps the application status "running" until
    // something explicitly tells it to stop (see SendExitSignal() below).
    B33::App::AppStatus::Get().LockInToService();
}

void BasicComponent::Update( float, ComponentBridge & )
{
    // Per-frame game/application logic goes here.
    B33_INFO( L"Here goes some complex logic..." );

    // Example: request application shutdown. Call this whenever the
    // component decides the app should terminate (e.g. on some condition
    // being met). Because of LockInToService() above, this is the only way
    // the app will exit.
    B33::App::AppStatus::Get().SendExitSignal();
}

void BasicComponent::Destroy( ComponentBridge & )
{
    B33_INFO( L"Goodbye B3!" );
}

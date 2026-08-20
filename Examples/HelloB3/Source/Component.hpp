#pragma once
#include <B33System.hpp>
#include <ComponentsOrder.hpp>

// Base class for all components. Derive your own class from Component and
// place the B33_COMPONENT macro inside it to register the type.
//
// Components are split into a few groups, distinguished by how they are
// scheduled across threads:
//   - Component       : initialized, updated, and destroyed on the main thread.
//   - ComponentAsync  : initialized, updated, and destroyed on worker threads,
//                       dispatched through a job queue (each job is a call
//                       to one method of the ComponentAsync class).
//
// Every component type is registered under a string name, so it can later
// be looked up by name at runtime.
//
// Components may spawn additional components dynamically while the
// application is running.
class BasicComponent : public ::B33::System::Component
{
    using ComponentBridge = ::B33::System::ComponentBridge;
    B33_COMPONENT( BasicComponent );

  public:
    // Called once when the component is spawned.
    // Use this to set up initial state and acquire resources.
    virtual void Initialize( ComponentBridge &bridge ) override;

    // Called once per frame while the component is active.
    // fDelta is the time elapsed since the previous frame, in seconds.
    virtual void Update( float fDelta, ComponentBridge &bridge ) override;

    // Called once when the component is destroyed.
    // Use this to release resources acquired in Initialize().
    virtual void Destroy( ComponentBridge &bridge ) override;
};

// Declares which components are spawned when the application starts.
// The order components are listed here determines the order in which
// their Initialize(), Update(), and Destroy() calls run.
B33_CREATE_COMPONENTS( "BasicComponent" )

#ifndef B33_IBASEWINDOW_H
#define B33_IBASEWINDOW_H

#include "B33Core.h"

#include "AppStatus.hpp"
#include "Window/WindowDesc.hpp"
#include "Window/WindowEvents.h"
#include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#include <mutex>

namespace B33::App
{

/**
 * @brief Basic wrapper for window class.
 *
 * It's possible to create your own implementation of WindowPolicy
 * by overriding virtual methods of the base class or creating your new
 * policy from IWindowPolicy class, to handle different and custom os level logic.
 * */
template <typename Derived, typename WindowPolicy = DefaultSystemWindowPolicy>
class IBaseWindow
{
  public:
    IBaseWindow()
      : m_Policy( ::std::make_unique<WindowPolicy>() )
      , m_pWindowDesc( ::std::make_shared<WindowDesc>( CreateWindowDesc( L"IBaseWindow", 1280, 720 ) ) )
    {
    }

    template <class U>
    explicit IBaseWindow( U &&windowDesc = WindowDesc() )
      : m_Policy( ::std::make_unique<WindowPolicy>() )
      , m_pWindowDesc( ::std::make_shared<WindowDesc>( ::std::forward<U>( windowDesc ) ) )
    {
    }

    ~IBaseWindow()
    {
        if ( m_pWindowDesc.get() )
        {
            this->Destroy();
        }
    }

  public:
    IBaseWindow( const IBaseWindow & )                     = delete;
    IBaseWindow &operator=( const IBaseWindow & ) noexcept = delete;

    IBaseWindow( IBaseWindow &&other ) noexcept
      : m_Policy( ::std::move( other.m_Policy ) )
      , m_pWindowDesc( ::std::move( other.m_pWindowDesc ) )
    {
    }

    IBaseWindow &operator=( IBaseWindow &&other ) noexcept
    {
        this->m_Policy      = ::std::move( other.m_Policy );
        this->m_pWindowDesc = ::std::move( other.m_pWindowDesc );

        other.m_pWindowDesc = nullptr;
    }

  public:
    template <class NewPolicy>
    void ChangePolicy()
    {
        B33_ASSERT( m_pWindowDesc != nullptr );

        ::std::lock_guard lg( m_pWindowDesc->mUpdated );

        bool                                         bWasAlive  = this->m_pWindowDesc->Data.bIsAlive;
        ::std::unique_ptr<DefaultSystemWindowPolicy> pNewPolicy = ::std::make_unique<NewPolicy>();
        m_Policy.swap( pNewPolicy );

        // If the window wasn't created yet, there is nothing left to do
        if ( !bWasAlive )
        {
            return;
        }

        // Create structs for new state and to keep the old state of WindowDesc
        WindowDesc oldDesc = *( this->m_pWindowDesc.get() );
        WindowDesc newDesc = CreateWindowDesc( oldDesc.Data.Name, oldDesc.Data.Width, oldDesc.Data.Height );

        // Try to create the new window
        B33_TRACE( L"Creating new window for behavior change" );
        SetWindowDescBufferStateInternal( newDesc );
        try
        {
            B33_LOG( Core::Debug::Warning, L"Creating new window!" );
            this->Create();
        }
        catch ( ... )
        {
            SetWindowDescBufferStateInternal( oldDesc );
            m_Policy.swap( pNewPolicy );
            B33_LOG( Core::Debug::Error, L"We couldn't change this window policy!" );
            return;
        }

        // Load old state, try to destroy the old window
        newDesc.OS   = this->m_pWindowDesc->OS;
        newDesc.Data = this->m_pWindowDesc->Data;
        SetWindowDescBufferStateInternal( oldDesc );
        m_Policy.swap( pNewPolicy );
        try
        {
            B33_LOG( Core::Debug::Warning, L"Destroying the old window!" );
            this->Destroy();
        }
        catch ( ... )
        {
            B33_LOG( Core::Debug::Error, L"Old verison of window wasn't properly closed!" );
        }

        // Load our new policy and new state
        B33_TRACE( L"Swaping old window desc with new one" );
        SetWindowDescBufferStateInternal( newDesc );
        m_Policy.swap( pNewPolicy );

        B33_TRACE( L"Updating the new window desc" );
        try
        {
            this->Update( 0.f );
            if ( oldDesc.Data.bIsVisible )
            {
                this->Show();
            }
        }
        catch ( ::std::exception &e )
        {
            B33_ERROR( L"Fatal on updating the new behavior" );
            throw ::B33::Core::Exception( e );
        }
        m_pWindowDesc->Data.LastEvent = EAbWindowEvents::ChangedBehavior;
    }

  public:
    void Create()
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        if ( m_pWindowDesc->Data.bIsAlive )
        {
            B33_LOG( Core::Debug::Warning, L"Cannot create alive window" );
            return;
        }
        if ( m_Policy->WindowPolicyCreate( m_pWindowDesc.get() ) != 0 )
        {
            throw B33_EXCEPT( "Couldn't create the window" );
        }

        B33::App::AppStatus::Get().SendOpenWindowSignal( m_pWindowDesc );
        m_pWindowDesc->Data.bIsAlive = true;

        // Some OS need to go trough one queue of messages to fully create the window
        this->Update( 0.0 );
    }

    void Show()
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        m_Policy->WindowPolicyShow( m_pWindowDesc.get() );
        m_pWindowDesc->Data.bIsVisible = true;
    }

    void Hide()
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        m_Policy->WindowPolicyHide( m_pWindowDesc.get() );
        m_pWindowDesc->Data.bIsVisible = false;
    }

    void Destroy()
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        if ( !m_pWindowDesc->Data.bIsAlive )
        {
            B33_LOG( Core::Debug::Warning, L"Cannot destroy dead window" );
            return;
        }

        B33::App::AppStatus::Get().SendCloseWindowSignal( m_pWindowDesc );

        m_Policy->WindowPolicyDestroy( m_pWindowDesc.get() );

        m_pWindowDesc->Data.bIsAlive = false;
    }

    void Update( const float fDelta )
    {
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        if ( !m_pWindowDesc->Data.bIsAlive )
        {
            B33_WARNING( L"Dead window is being updated" );
            return;
        }

        // Don't reset the last event flags if the only flag that we have set is EAbWindowEvents::ChangedBehavior
        if ( this->m_pWindowDesc->Data.LastEvent & ~EAbWindowEvents::ChangedBehavior )
            m_pWindowDesc->Data.LastEvent &= 0;

        // Make sure that after EAbWindowEvents::ChangedBehavior propagation,
        // we are going to reset the events by setting EAbWindowEvents::NothingNew flag
        this->m_pWindowDesc->Data.LastEvent |= EAbWindowEvents::NothingNew;
        m_Policy->WindowPolicyUpdate( m_pWindowDesc.get() );

        if ( m_pWindowDesc->Data.LastEvent & EAbWindowEvents::Destroy )
        {
            B33_LOG( Core::Debug::Info, L"Window is being closed by user" );
            this->Destroy();
        }

        static_cast<Derived *>( this )->HandleMessageImpl( fDelta, m_pWindowDesc->Data.LastEvent );
    }

  public:
    const ::std::shared_ptr<WindowDesc> &GetWindowDesc() const
    {
        return m_pWindowDesc;
    }

    const DefaultSystemWindowPolicy *GetPolicy() const
    {
        return m_Policy.get();
    }

  private:
    void HandleMessage( const float fDelta, EAbWindowEventsFlags events )
    {
        static_cast<Derived *>( this )->HandleMessageImpl( fDelta, events );
    }

  private:
    void SetWindowDescBufferStateInternal( const WindowDesc &wd )
    {
        this->m_pWindowDesc.get()->Data = wd.Data;
        this->m_pWindowDesc.get()->OS   = wd.OS;
    }

  private:
    ::std::unique_ptr<DefaultSystemWindowPolicy> m_Policy;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;
};

} // namespace B33::App
#endif // !B33_IBASEWINDOW_H

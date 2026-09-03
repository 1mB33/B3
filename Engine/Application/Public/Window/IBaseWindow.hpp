#if !defined( B33_IBASEWINDOW_H )
#    define B33_IBASEWINDOW_H

#    include <B33Core.h>
#    include "AppResources.hpp"
#    include "AppStatus.hpp"
#    include "Window/WindowDesc.hpp"
#    include "Window/WindowEvents.h"
#    include "Window/WindowPolicy/BasicSystemPolicy.hpp"

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
    template <typename T>
    constexpr decltype( auto ) Forward( T &arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

    template <typename T>
    constexpr decltype( auto ) Forward( T &&arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

    template <typename T>
    constexpr decltype( auto ) MakeShared() noexcept
    {
        return ::std::make_shared<T>();
    }

    template <typename T>
    constexpr decltype( auto ) MakeUnique() noexcept
    {
        return ::std::make_unique<T>();
    }

    template <typename T, typename U>
    constexpr decltype( auto ) MakeShared( U &&arg ) noexcept
    {
        return ::std::make_shared<T>( Forward<U>( arg ) );
    }

    template <typename T, typename U>
    constexpr decltype( auto ) MakeUnique( U &&arg ) noexcept
    {
        return ::std::make_unique<T>( Forward<U>( arg ) );
    }

    template <typename T>
    using UniquePtr = ::std::unique_ptr<T>;

    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

  public:
    IBaseWindow()
      : m_Policy( MakeUnique<WindowPolicy>() )
      , m_pWindowDesc( MakeShared<WindowDesc>( CreateWindowDesc( L"IBaseWindow", 1280, 720 ) ) )
    {
        using ::std::this_thread::get_id;

        B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
    }

    template <class U>
    explicit IBaseWindow( U &&windowDesc = WindowDesc() )
      : m_Policy( MakeUnique<WindowPolicy>() )
      , m_pWindowDesc( MakeShared<WindowDesc>( ::std::forward<U>( windowDesc ) ) )
    {
    }

    ~IBaseWindow() noexcept
    {
        if ( m_pWindowDesc.get() )
        {
            this->Destroy();
        }
    }

  public:
    IBaseWindow( const IBaseWindow & ) noexcept            = delete;
    IBaseWindow &operator=( const IBaseWindow & ) noexcept = delete;

    IBaseWindow( IBaseWindow &&other ) noexcept
      : m_Policy( ::std::move( other.m_Policy ) )
      , m_pWindowDesc( ::std::move( other.m_pWindowDesc ) )
    {
    }

    IBaseWindow &operator=( IBaseWindow &&other ) noexcept
    {
        this->Destroy();

        this->m_Policy      = ::std::move( other.m_Policy );
        this->m_pWindowDesc = ::std::move( other.m_pWindowDesc );

        other.m_pWindowDesc = nullptr;

        return *this;
    }

  public:
    template <class NewPolicy>
    void ChangePolicy() noexcept
    {
        using ::std::exception;
        using ::std::lock_guard;

        try
        {
            B33_ASSERT( m_pWindowDesc != nullptr );
        }
        catch ( exception &e )
        {
            B33_ERROR( L"Error on destroy %s", e.what() );
            return;
        }

        lock_guard lg( m_pWindowDesc->mUpdated );

        bool                                 bWasAlive  = this->m_pWindowDesc->Data.bIsAlive;
        UniquePtr<DefaultSystemWindowPolicy> pNewPolicy = nullptr;

        // Create structs for new state and to keep the old state of WindowDesc
        WindowDesc oldDesc = {};
        WindowDesc newDesc = {};
        try
        {
            oldDesc = *( this->m_pWindowDesc.get() );
            newDesc = CreateWindowDesc( oldDesc.Data.Name, oldDesc.Data.Width, oldDesc.Data.Height );

            pNewPolicy = MakeUnique<NewPolicy>();
            m_Policy.swap( pNewPolicy );

            // If the window wasn't created yet, there is nothing left to do
            if ( !bWasAlive )
            {
                return;
            }
        }
        catch ( ... )
        {
            B33_ERROR( L"We couldn't create resources" );
            return;
        }

        // Try to create the new window
        B33_TRACE( L"Creating new window for behavior change" );
        SetWindowDescBufferStateInternal( newDesc );
        try
        {
            B33_WARNING( L"Creating new window!" );
            this->Create();
        }
        catch ( ... )
        {
            SetWindowDescBufferStateInternal( oldDesc );
            m_Policy.swap( pNewPolicy );
            B33_ERROR( L"We couldn't change this window policy!" );
            return;
        }

        // Load old state, try to destroy the old window
        newDesc.OS   = this->m_pWindowDesc->OS;
        newDesc.Data = this->m_pWindowDesc->Data;
        SetWindowDescBufferStateInternal( oldDesc );
        m_Policy.swap( pNewPolicy );
        try
        {
            B33_WARNING( L"Destroying the old window!" );
            this->Destroy();
        }
        catch ( ... )
        {
            B33_ERROR( L"Old verison of window wasn't properly closed!" );
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
        catch ( exception &e )
        {
            B33_ERROR( L"Error on destroy %s", e.what() );
        }
        m_pWindowDesc->Data.LastEvent |= EB33WindowEvents::ChangedBehavior;
    }

  public:
    void Create()
    {
        using ::std::this_thread::get_id;

        B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
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
        using ::std::this_thread::get_id;

        B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        m_Policy->WindowPolicyShow( m_pWindowDesc.get() );
        m_pWindowDesc->Data.bIsVisible = true;
    }

    void Hide()
    {
        using ::std::this_thread::get_id;

        B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        m_Policy->WindowPolicyHide( m_pWindowDesc.get() );
        m_pWindowDesc->Data.bIsVisible = false;
    }

    void Destroy() noexcept
    {
        using ::B33::App::AppStatus;
        using ::std::exception;
        using ::std::this_thread::get_id;

        try
        {
            B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
            B33_ASSERT( m_pWindowDesc != nullptr );
            B33_ASSERT( m_Policy != nullptr );
        }
        catch ( exception &e )
        {
            B33_ERROR( L"Error on destroy %s", e.what() );
            return;
        }

        if ( !m_pWindowDesc->Data.bIsAlive )
        {
            B33_WARNING( L"Cannot destroy dead window" );
            return;
        }

        try
        {
            AppStatus::Get().SendCloseWindowSignal( m_pWindowDesc );
            m_Policy->WindowPolicyDestroy( m_pWindowDesc.get() );
        }
        catch ( exception &e )
        {
            B33_ERROR( L"Error on destroy %s", e.what() );
        }

        m_pWindowDesc->Data.bIsAlive = false;
    }

    void Update( const float fDelta )
    {
        using ::std::this_thread::get_id;

        B33_ASSERT( get_id() == AppResources::Get().GetMainThreadID() );
        B33_ASSERT( m_pWindowDesc != nullptr );
        B33_ASSERT( m_Policy != nullptr );

        if ( !m_pWindowDesc->Data.bIsAlive )
        {
            B33_WARNING( L"Dead window is being updated" );
            return;
        }

        m_Policy->WindowPolicyUpdate( m_pWindowDesc.get() );

        if ( m_pWindowDesc->Data.LastEvent & EB33WindowEvents::Destroy )
        {
            B33_LOG( Core::Debug::Info, L"Window is being closed by user" );
            this->Destroy();
        }

        static_cast<Derived *>( this )->HandleMessageImpl( fDelta, m_pWindowDesc->Data.LastEvent );
    }

  public:
    const SharedPtr<WindowDesc> &GetWindowDesc() const
    {
        return m_pWindowDesc;
    }

    const DefaultSystemWindowPolicy *GetPolicy() const
    {
        return m_Policy.get();
    }

  private:
    void HandleMessage( const float fDelta, EB33WindowEventsFlags events )
    {
        static_cast<Derived *>( this )->HandleMessageImpl( fDelta, events );
    }

  private:
    void SetWindowDescBufferStateInternal( const WindowDesc &wd ) noexcept
    {
        this->m_pWindowDesc.get()->Data = wd.Data;
        this->m_pWindowDesc.get()->OS   = wd.OS;
    }

  private:
    UniquePtr<DefaultSystemWindowPolicy> m_Policy;

    SharedPtr<WindowDesc> m_pWindowDesc;
};

} // namespace B33::App
#endif // !B33_IBASEWINDOW_H

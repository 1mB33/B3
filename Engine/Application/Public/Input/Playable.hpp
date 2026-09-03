#if !defined(B33_PLAYABLE_HPP)
#define B33_PLAYABLE_HPP

#include "B33Core.h"

#include "Bind.h"
#include "Input/UserInput.hpp"
#include "Action.hpp"

namespace B33::App
{
template <class PLAYABLE, class OWNER>
class BorrowedPlayable
{
  public:
    BorrowedPlayable( PLAYABLE &component, OWNER *pOwner )
      : m_Component( component )
      , m_pOwner( pOwner )
    {
        m_pOwner->Lock();
    }

    ~BorrowedPlayable()
    {
        m_pOwner->Unlock();
    }

  public:
    BorrowedPlayable( BorrowedPlayable<PLAYABLE, OWNER> && )            = default;
    BorrowedPlayable &operator=( BorrowedPlayable<PLAYABLE, OWNER> && ) = default;

    BorrowedPlayable( const BorrowedPlayable<PLAYABLE, OWNER> & )            = delete;
    BorrowedPlayable &operator=( const BorrowedPlayable<PLAYABLE, OWNER> & ) = delete;

  public:
    PLAYABLE *operator->()
    {
        return &m_Component;
    }

    PLAYABLE &Get()
    {
        return m_Component;
    }

  private:
    PLAYABLE &m_Component;
    OWNER    *m_pOwner;
};

template <class OBJECT_CLASS, class CONTROLLER>
class Playable
{
    template <typename T>
    constexpr decltype( auto ) forward( T &arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

    template <typename T>
    constexpr decltype( auto ) forward( T &&arg ) noexcept
    {
        return ::std::forward<T>( arg );
    }

  public:
    Playable()
      : m_Object()
      , m_Controller()
      , m_Used()
    {
    }

    template <typename... ARGS>
    Playable( ARGS &&...args )
      : m_Object( forward<ARGS>( args )... )
      , m_Controller()
      , m_Used()
    {
    }

    ~Playable() noexcept = default;

    // Getters // -----------------------------------------------------------------------------------------------------
  public:
    const OBJECT_CLASS &GetObject() const
    {
        return m_Object;
    }

    BorrowedPlayable<OBJECT_CLASS, Playable> GetObject()
    {
        return BorrowedPlayable( m_Object, this );
    }

    const CONTROLLER &GetController() const
    {
        return m_Controller;
    }

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    using UserInputPtr      = ::std::shared_ptr<::B33::App::UserInput>;
    using ActionInputVector = ::std::vector<::std::pair<::B33::App::Action, AbInputBind>>;

    void Lock()
    {
        m_Used.lock();
    }

    void Unlock()
    {
        m_Used.unlock();
    }

    void BindToInput( const UserInputPtr &pInput, const ActionInputVector &binds )
    {
        m_Controller.SignObject( pInput );
        for ( const auto &bind : binds )
        {
            switch ( bind.first.Type )
            {
                case EAbBindType::Keyboard:
                    pInput->Bind( &m_Object, &m_Controller, bind.first.ButtonAction, nullptr, bind.second );
                    break;
                case EAbBindType::Mouse:
                    pInput->Bind( &m_Object, &m_Controller, nullptr, bind.first.MouseAction, bind.second );
                    break;
                case EAbBindType::MouseButton:
                    pInput->Bind( &m_Object, &m_Controller, bind.first.ButtonAction, nullptr, bind.second );
                    break;
            }
        }
    }

  private:
    OBJECT_CLASS m_Object;
    CONTROLLER   m_Controller;
    ::std::mutex m_Used;
};

} // namespace B33::App

#endif // !B33_PLAYABLE_HPP

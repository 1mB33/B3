#if !defined( B33_EXCEPT_HPP )
#    define B33_EXCEPT_HPP

#    include "B33CoreMinimal.h"
#    include <exception>

namespace B33::Core
{

class __B33_API Exception : public ::std::exception
{
    static constexpr i32 InvalidLine = -1;

  public:
    explicit Exception( const char *szMessage    = nullptr,
                        usize       uMesLen      = 0,
                        i32         uLine        = InvalidLine,
                        const char *szFileName   = nullptr,
                        usize       uFileNameLen = 0 ) noexcept;

    explicit Exception( ::std::exception e ) noexcept
      : std::exception( e )
      , m_pszMessage( nullptr )
      , m_uMesLen( 0 )
      , m_Line( InvalidLine )
      , m_pszFileName( nullptr )
      , m_uFileNameLen( 0 )
    {
    }

    template <usize uMesLen, usize uFileNameLen>
    constexpr Exception( const char ( &pszMessage )[ uMesLen ],
                         i32 uLine                                   = 0,
                         const char ( &pszFileName )[ uFileNameLen ] = nullptr ) noexcept
      : Exception( pszMessage, uMesLen, uLine, pszFileName, uFileNameLen )
    {
    }

    ~Exception() noexcept;

  public:
    Exception( const Exception & ) noexcept;
    Exception &operator=( const Exception & ) noexcept = delete;

    Exception( Exception && ) noexcept            = default;
    Exception &operator=( Exception && ) noexcept = delete;

  public:
    virtual const char *what() const noexcept final override;

  private:
    void LogAndReturnMessage( const char *pszMessage, usize uMesLen ) const noexcept;

  private:
    const char *m_pszMessage;
    i32         m_uMesLen;
    const i32   m_Line;
    const char *m_pszFileName;
    const usize m_uFileNameLen;
};

} // namespace B33::Core
#endif // !B33_EXCEPT_HPP

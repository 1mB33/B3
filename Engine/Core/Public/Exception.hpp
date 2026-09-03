#if !defined(B33_EXCEPT_H)
#define B33_EXCEPT_H

#include "B33CoreMinimal.h"
#include <exception>

namespace B33::Core
{

class __B33_API Exception : public ::std::exception
{
    static constexpr int32_t InvalidLine = -1;

  public:
    explicit Exception( const char *szMessage    = nullptr,
                        size_t      uMesLen      = 0,
                        int32_t     uLine        = InvalidLine,
                        const char *szFileName   = nullptr,
                        size_t      uFileNameLen = 0 ) noexcept;

    explicit Exception( ::std::exception e ) noexcept
      : std::exception( e )
      , m_pszMessage( nullptr )
      , m_uMesLen( 0 )
      , m_Line( InvalidLine )
      , m_pszFileName( nullptr )
      , m_uFileNameLen( 0 )
    {
    }

    template <size_t uMesLen, size_t uFileNameLen>
    constexpr Exception( const char ( &pszMessage )[ uMesLen ],
                         int32_t uLine                               = 0,
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
    void LogAndReturnMessage( const char *pszMessage, size_t uMesLen ) const noexcept;

  private:
    const char   *m_pszMessage;
    int32_t       m_uMesLen;
    const int32_t m_Line;
    const char   *m_pszFileName;
    const size_t  m_uFileNameLen;
};

} // namespace B33::Core
#endif // !B33_EXCEPT_H

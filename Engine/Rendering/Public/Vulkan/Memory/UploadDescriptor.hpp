#if !defined( B33_UPLOAD_DESCRIPTOR_HPP )
#    define B33_UPLOAD_DESCRIPTOR_HPP

#    include "Vulkan/Buffers/GPUBuffer.hpp"

namespace B33::Rendering
{

enum EUploadType
{
    LocalBuffer,
    StreamBuffer,
};

template <class BUFFER_T>
struct UploadDescriptor
{
    template <typename T>
    using SharedPtr = ::std::shared_ptr<T>;

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

  public:
    UploadDescriptor() = delete;

    template <typename U, typename L, class T>
    UploadDescriptor( U &&bufferInfo, L &&write, EUploadType type, const T &buffer )
      : BufferInfo( Forward<U>( bufferInfo ) )
      , Write( Forward<L>( write ) )
      , Type( type )
    {
        Write.pBufferInfo = &BufferInfo;
        Buffer            = buffer;
    }

    ~UploadDescriptor() = default;

  public:
    UploadDescriptor( const UploadDescriptor &other ) noexcept
      : BufferInfo( other.BufferInfo )
      , Write( other.Write )
      , Type( other.Type )
    {
        Write.pBufferInfo = &BufferInfo;
        Buffer            = other.Buffer;
    }

    UploadDescriptor &operator=( const UploadDescriptor &other ) noexcept
    {
        BufferInfo        = other.BufferInfo;
        Write             = other.Write;
        Type              = other.Type;
        Write.pBufferInfo = &BufferInfo;
        Buffer            = other.Buffer;

        return *this;
    }

    UploadDescriptor( UploadDescriptor &&other ) noexcept
      : BufferInfo( std::move( other.BufferInfo ) )
      , Write( std::move( other.Write ) )
      , Type( other.Type )
    {
        Write.pBufferInfo = &BufferInfo;
        Buffer            = std::move( other.Buffer );
    }

    UploadDescriptor &operator=( UploadDescriptor &&other ) noexcept
    {
        BufferInfo        = std::move( other.BufferInfo );
        Write             = std::move( other.Write );
        Type              = std::move( other.Type );
        Write.pBufferInfo = &BufferInfo;
        Buffer            = other.Buffer;

        return *this;
    }

  public:
    ::VkDescriptorBufferInfo BufferInfo;
    ::VkWriteDescriptorSet   Write;
    EUploadType              Type;
    SharedPtr<BUFFER_T>      Buffer;
};

} // namespace B33::Rendering
#endif //! B33_UPLOAD_DESCRIPTOR_HPP

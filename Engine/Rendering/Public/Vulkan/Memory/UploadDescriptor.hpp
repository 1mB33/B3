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
    __B33_API                   UploadDescriptor( const UploadDescriptor &other ) noexcept;
    __B33_API UploadDescriptor &operator=( const UploadDescriptor &other ) noexcept;

    __B33_API                   UploadDescriptor( UploadDescriptor &&other ) noexcept;
    __B33_API UploadDescriptor &operator=( UploadDescriptor &&other ) noexcept;

  public:
    ::VkDescriptorBufferInfo BufferInfo;
    ::VkWriteDescriptorSet   Write;
    EUploadType              Type;
    SharedPtr<GPUBuffer>     Buffer;
};

} // namespace B33::Rendering
#endif //! B33_UPLOAD_DESCRIPTOR_HPP

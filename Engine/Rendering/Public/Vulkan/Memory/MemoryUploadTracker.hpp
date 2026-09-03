#if !defined(B33_MEMORY_UPLOAD_TRACKER_H)
#define B33_MEMORY_UPLOAD_TRACKER_H

#include "B33CoreMinimal.h"

namespace B33::Rendering
{

enum EReupload
{
    NoAction         = 1,
    RequestStaging   = NoAction << 1,
    RequestGpuUpload = RequestStaging << 1,
};

class MemoryUploadTracker
{
  public:
    __B33_API MemoryUploadTracker();

    ~MemoryUploadTracker() = default;

  public:
    MemoryUploadTracker( const MemoryUploadTracker & ) noexcept            = default;
    MemoryUploadTracker &operator=( const MemoryUploadTracker & ) noexcept = default;

    MemoryUploadTracker( MemoryUploadTracker && ) noexcept            = default;
    MemoryUploadTracker &operator=( MemoryUploadTracker && ) noexcept = default;

    // Methods // -----------------------------------------------------------------------------------------------------
  public:
    /**
     * @brief Returns state that the upload is on, shifts the value to next state
     *
     * @return EReupload enumerator that descirbes the state
     */
    __B33_API EReupload ReuploadStatus();

    /**
     * @brief Returns state that the upload is on
     *
     * @return EReupload enumerator that descirbes the state
     */
    __B33_API EReupload PeekStatus();

    __B33_API void ForceUpload();

  private:
    EReupload m_Reupload = NoAction;
};

} // namespace B33::Rendering
#endif //! B33_MEMORY_UPLOAD_TRACKER_H

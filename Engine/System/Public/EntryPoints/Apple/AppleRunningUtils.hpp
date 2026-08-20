#if defined( __APPLE__ )
#    ifndef B33_APPLE_RUNNING_UTILS_H
#        define B33_APPLE_RUNNING_UTILS_H

namespace B33::System
{

class __B33_API RunningUtils
{
  public:
    static void StartUpNSApp();
};

} // namespace B33::System
#    endif // !B33_APPLE_RUNNING_UTILS_H
#endif     // __APPLE__

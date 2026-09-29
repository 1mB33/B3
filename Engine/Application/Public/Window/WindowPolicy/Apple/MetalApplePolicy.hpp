#if defined( __APPLE__ )
#    ifndef B33_METAL_WINDOW_POLICY_H
#        define B33_METAL_WINDOW_POLICY_H

#        include "BasicApplePolicy.hpp"

namespace B33::App
{

class __B33_API MetalAppleWindowPolicy : public BasicAppleWindowPolicy
{
  public:
    virtual void OnCreate( WindowDesc *pWd ) override;
};

} // namespace B33::App
#    endif // !B33_METAL_WINDOW_POLICY_H
#endif     // __APPLE__

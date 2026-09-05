#if !defined( B33_COMPONENTS_ORDER_HPP )
#    define B33_COMPONENTS_ORDER_HPP

#    include <B33Core.h>

namespace B33::System
{

struct ComponentOrderRegister
{
    using StringView = ::std::string_view;
    template <typename T>
    using Vector = ::std::vector<T>;

  private:
    ComponentOrderRegister() noexcept = default;

  public:
    ~ComponentOrderRegister() noexcept = default;

    ComponentOrderRegister( ComponentOrderRegister && ) noexcept      = default;
    ComponentOrderRegister( const ComponentOrderRegister & ) noexcept = default;

    ComponentOrderRegister &operator=( ComponentOrderRegister && ) noexcept      = default;
    ComponentOrderRegister &operator=( const ComponentOrderRegister & ) noexcept = default;

  public:
    __B33_API static ComponentOrderRegister RegisterOrder( Vector<StringView> order );
};

/**
* @brief Creates a global instance of ComponentOrderRegister.
* Structure registers that order in to every EngineLoop object.
* Asures that every EngineLoop object will have access to listed components after construction.
* Every operation called on EngineLoop (InitializeComponents, UpdateCompononts, etc...) will loop through
* components in sequence provided in this macro.
*/
#    define B33_CREATE_COMPONENTS( ... )                                                                               \
        static ::B33::System::ComponentOrderRegister g_ComponentOrder =                                                \
            ::B33::System::ComponentOrderRegister::RegisterOrder( { __VA_ARGS__ } );

} // namespace B33::System
#endif // !B33_COMPONENTS_ORDER_HPP

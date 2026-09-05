#if !defined( B33_ENGINE_LOOP_H )
#    define B33_ENGINE_LOOP_H

#    include <B33Core.h>
#    include "IComponent.hpp"
#    include "ComponentsOrder.hpp"
#    include "ComponentBridge.hpp"
#    include "Synchronization/JobSystem.hpp"

namespace B33::System
{

class EngineLoop
{
    friend class ComponentInstanceRegister;
    friend struct ComponentOrderRegister;

    using StringView = ::std::string_view;
    template <typename T>
    using Vector             = ::std::vector<T>;
    using ComponentsRegistry = ::std::unordered_map<::std::string_view, ComponentFactory>;
    using ComponentsMap      = ::std::map<::B33::System::EComponentType, ::std::vector<class ComponentAbstractBase *>>;

  public:
    EngineLoop()
      : m_Components()
      , m_ComponentBridge()
      , m_JobSystem()
      , m_bInitialized( false )
    {
    }

    ~EngineLoop() noexcept = default;

    EngineLoop( EngineLoop && )      = delete;
    EngineLoop( const EngineLoop & ) = delete;


  public:
    template <class COMPONENT_DERIVED>
    void AddComponent()
    {
        AddComponentInternal( COMPONENT_DERIVED::GetComponentName(), COMPONENT_DERIVED::GetComponentType() );
    }

  public:
    /**
     * @brief Initializes components in the order described in B33_ORDER_COMPONENTS
     */
    __B33_API void InitializeComponents();

    /**
     * @brief Updates components in the order described in B33_ORDER_COMPONENTS, every component gets last fDelta
     *
     * @param fDelta Time diffrence in ms between two subsequent calls
     */
    __B33_API void UpdateComponents( float fDelta );

    /**
     * @brief Destroys components in order reversed to the initialization order
     */
    __B33_API void DestroyComponents();

  private:
    __B33_API void AddComponentInternal( StringView componentName );

  private:
    static inline ComponentsRegistry m_ComponentRegistry      = {};
    static inline Vector<StringView> m_ComponentOrderRegistry = {};

    ComponentsMap   m_Components      = {};
    ComponentBridge m_ComponentBridge = {};

    Core::JobSystem m_JobSystem = {};


    bool m_bInitialized = false;
};

} // namespace B33::System
#endif // !B33_ENGINE_LOOP_H

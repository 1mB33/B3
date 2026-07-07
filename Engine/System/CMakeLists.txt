SET(BEE_SYSTEM_SOURCE_OBJECTIVE_CPP
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/EntryPoints/Apple/AppleRunningUtils.mm"
)
SET(BEE_SYSTEM_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/IComponent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/EngineLoop.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/ComponentsOrder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/B33System.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Events/EventHandle.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Events/EventDispatcher.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Events/EventDispatcherInstance.cpp"
)
SET(BEE_SYSTEM_HEADRES
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/IComponent.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ComponentBridge.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ComponentsOrder.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EngineLoop.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33System.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/Call.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/Event.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/EventDispatcherInstance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/EventHandle.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/EventType.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Events/ICall.hpp"
)
IF(B33_RUN_TESTS STREQUAL "ON")

    MESSAGE("System includes tests")

    SET(BEE_SYSTEM_TESTS
        "${CMAKE_CURRENT_SOURCE_DIR}/Tests/Run.cpp"
    )

ENDIF()

IF (MSVC)

    ADD_LIBRARY(B33System SHARED
        "${BEE_SYSTEM_SOURCE}"
        "${BEE_SYSTEM_HEADRES}"
        "${BEE_SYSTEM_TESTS}"
    )

    TARGET_COMPILE_OPTIONS(B33System PUBLIC "/permissive-")
    TARGET_COMPILE_OPTIONS(B33System PUBLIC "/GR")

    IF (CMAKE_BUILD_TYPE!="Debug")

        TARGET_COMPILE_OPTIONS(B33System PUBLIC "/O2")

    ENDIF()

ENDIF()

IF (UNIX AND NOT APPLE)

    ADD_LIBRARY(B33System SHARED
        "${BEE_SYSTEM_SOURCE}"
        "${BEE_SYSTEM_HEADRES}"
        "${BEE_SYSTEM_TESTS}"
    )
    TARGET_COMPILE_OPTIONS(B33System PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )

    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")

        TARGET_COMPILE_OPTIONS(B33System PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33System PRIVATE "-O3")

    ENDIF()

ENDIF()

IF (APPLE)

    ADD_LIBRARY(B33System SHARED
        "${BEE_SYSTEM_SOURCE_OBJECTIVE_CPP}"
        "${BEE_SYSTEM_SOURCE}"
        "${BEE_SYSTEM_HEADRES}"
        "${BEE_SYSTEM_TESTS}"
    )
    TARGET_COMPILE_OPTIONS(B33System PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )

    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")

        TARGET_COMPILE_OPTIONS(B33System PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33System PUBLIC "-O3")

    ENDIF()

    TARGET_LINK_LIBRARIES(B33System
        PRIVATE "-framework Cocoa"
        PRIVATE "-framework Foundation"
        PRIVATE "-framework AppKit"
    )

    SET_TARGET_PROPERTIES(B33System PROPERTIES
        LINKER_LANGUAGE "CXX"
        XCODE_ATRIBUTE_CLANG_ENABLE_OBJC_ARC YES
        XCODE_ATRIBUTE_CLANG_ENABLE_MODULES YES
        XCODE_ATRIBUTE_CLANG_ENABLE_OBJC_WEAK YES
    )

    SET_SOURCE_FILES_PROPERTIES(
        "${BEE_SYSTEM_SOURCE_OBJECTIVE_CPP}"
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON
    )

ENDIF()

TARGET_INCLUDE_DIRECTORIES(B33System PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(B33System PUBLIC
    B33Core
    B33App
)

TARGET_PRECOMPILE_HEADERS(B33System PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33System.hpp")

TARGET_COMPILE_DEFINITIONS(B33System PRIVATE _BEAST_EXPORTS _UNICODE UNICODE)

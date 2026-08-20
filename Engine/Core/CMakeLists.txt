SET(BEE_CORE_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/B33Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Debug/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/DeltaTime.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/FpsLimiter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/JobSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/TerminalColors.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Unknown.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Tests/TestInstance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Tests/TestMaster.cpp"
)
SET(BEE_CORE_HEADRES
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CStandard.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CPlusPlusStandard.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ExportImport.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalLinuxIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalWindowsIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/MinimalSystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Assert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33CoreMinimal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Attributes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/FpsLimiter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/DeltaTime.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Utility.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/TerminalColors.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Unknown.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Tests/TestInstance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Tests/TestMaster.hpp"
)
IF(B33_RUN_TESTS STREQUAL "ON")

    MESSAGE("Core includes tests")

    SET(BEE_CORE_TESTS
        "${CMAKE_CURRENT_SOURCE_DIR}/Tests/Run.cpp"
    )

ENDIF()

ADD_LIBRARY(B33Core SHARED
    "${BEE_CORE_SOURCE}"
    "${BEE_CORE_HEADRES}"
    "${BEE_CORE_TESTS}"
)

IF ( UNIX AND NOT APPLE )

    FIND_PACKAGE(X11)

    IF ( X11_FOUND )

        TARGET_COMPILE_DEFINITIONS(B33Core PUBLIC _X11)
        TARGET_INCLUDE_DIRECTORIES(B33Core PUBLIC ${X11_INCLUDE_DIR})
        TARGET_LINK_LIBRARIES(B33Core PUBLIC ${X11_LIBRARIES})

    ENDIF()

ENDIF()

IF (MSVC)

    TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/permissive-")
    TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/GR")

    IF (B33_DEBUG_BUILD STREQUAL "ON")

        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/Z7")
        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/Zi")
        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/ZI")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "/O2")

    ENDIF()

ELSE()

    TARGET_COMPILE_OPTIONS(B33Core PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )
    IF (B33_DEBUG_BUILD STREQUAL "ON")

        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33Core PRIVATE "-O3")

    ENDIF()

ENDIF()

TARGET_INCLUDE_DIRECTORIES(B33Core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(B33Core PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Core.h")

TARGET_COMPILE_DEFINITIONS(B33Core PRIVATE _BEAST_EXPORTS _UNICODE UNICODE)

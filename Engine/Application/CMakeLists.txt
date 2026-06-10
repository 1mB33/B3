SET(BEE_APPLICATION_SOURCE_OBJECTIVE_CPP
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Apple/BasicApplePolicy.mm"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Apple/MetalApplePolicy.mm"
)
SET(BEE_APPLICATION_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppResources.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/BaseWindowDetails.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/X11ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/BasicLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/GameLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/BasicWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/WindowModeGameWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/BorderlessGameWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/UserInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/IBindMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/KeysMap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/KeysMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/MouseMap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/MouseMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/ControllerObject.cpp"
)
SET(BEE_APPLICATION_HEADRES
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33App.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/IWindowPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/BorderlessGameLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Apple/BasicApplePolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Apple/MetalApplePolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/WindowModeGameWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/BorderlessGameWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/BasicSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/GameSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/BorderlessGameSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/BaseWindowDetails.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/IBaseWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowListener.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowDesc.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/UserInput.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/Bind.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/InputEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/KeyList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/MouseButtonList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/ControllerObject.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/Action.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/Playable.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
)

IF (MSVC)

    ADD_LIBRARY(B33App SHARED
        "${BEE_APPLICATION_SOURCE}"
        "${BEE_APPLICATION_HEADRES}"
    )

    TARGET_COMPILE_OPTIONS(B33App PRIVATE "/permissive-")
    TARGET_COMPILE_OPTIONS(B33App PRIVATE "/GR")

    IF (CMAKE_BUILD_TYPE!="Debug")

        TARGET_COMPILE_OPTIONS(B33App PUBLIC "/O2")

    ENDIF()

ENDIF()

IF (UNIX AND NOT APPLE)

    ADD_LIBRARY(B33App SHARED
        "${BEE_APPLICATION_SOURCE}"
        "${BEE_APPLICATION_HEADRES}"
    )
    TARGET_COMPILE_OPTIONS(B33App PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )

    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")

        TARGET_COMPILE_OPTIONS(B33App PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33App PRIVATE "-O3")

    ENDIF()

    FIND_PACKAGE(X11 REQUIRED)
    FIND_LIBRARY(XI_LIB Xi REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC ${X11_INCLUDE_DIR})
    TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC ${XI_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(B33App PUBLIC ${X11_LIBRARIES})
    TARGET_LINK_LIBRARIES(B33App PUBLIC ${XI_LIB})

ENDIF()

IF (APPLE)

    ADD_LIBRARY(B33App SHARED
        "${BEE_APPLICATION_SOURCE_OBJECTIVE_CPP}"
        "${BEE_APPLICATION_SOURCE}"
        "${BEE_APPLICATION_HEADRES}"
    )
    TARGET_COMPILE_OPTIONS(B33App PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )

    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")

        TARGET_COMPILE_OPTIONS(B33System PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33App PRIVATE "-O3")

    ENDIF()

    TARGET_LINK_LIBRARIES(B33App
        PRIVATE "-framework Cocoa"
        PRIVATE "-framework Foundation"
        PRIVATE "-framework AppKit"
        PRIVATE "-framework Metal"
        PRIVATE "-framework QuartzCore"
    )

    SET_TARGET_PROPERTIES(B33App PROPERTIES
        LINKER_LANGUAGE "CXX"
        XCODE_ATRIBUTE_CLANG_ENABLE_OBJC_ARC YES
        XCODE_ATRIBUTE_CLANG_ENABLE_MODULES YES
        XCODE_ATRIBUTE_CLANG_ENABLE_OBJC_WEAK YES
    )

    SET_SOURCE_FILES_PROPERTIES(
        ${BEE_APPLICATION_SOURCE_OBJECTIVE_CPP}
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON
    )

ENDIF()

TARGET_INCLUDE_DIRECTORIES(B33App PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(B33App PUBLIC
    B33Core
)

TARGET_PRECOMPILE_HEADERS(B33App PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33App.h")

TARGET_COMPILE_DEFINITIONS(B33App PRIVATE _BEAST_EXPORTS _UNICODE UNICODE)

SET(BEE_RENDERING_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/WrapperAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/BasicComponentImplementations/MinimalHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/BasicComponentImplementations/RTXHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/BasicComponentImplementations/GraphicsComputeAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/BasicComponentImplementations/RTXDeviceAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/BasicComponentImplementations/PureGraphicsAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Buffers/GPUBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Buffers/GPUStreamBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Memory/Memory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Memory/UploadDescriptor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Memory/MemoryUploadTracker.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Utility.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Rays.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelPipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelGrid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/2DSprites/SpritesPipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Editor/EditorPipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/B33Rendering.cpp"
)
SET(BEE_RENDERING_HEADRES
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Primitives/Camera.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/BasicComponentImplementations/MinimalHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/BasicComponentImplementations/RTXHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/BasicComponentImplementations/GraphicsComputeAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/BasicComponentImplementations/RTXDeviceAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/BasicComponentImplementations/PureGraphicsAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Buffers/GPUBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Buffers/GPUStreamBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Memory/Memory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Memory/UploadDescriptor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Memory/MemoryUploadTracker.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/FrameResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/WrapperAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/WrapperHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/WrapperPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Utility.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/PushConstants.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Voxel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelGrid.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Rays.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/2DSprites/SpritesPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Editor/EditorPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Rendering.hpp"
)

ADD_LIBRARY(B33Rendering SHARED
    "${BEE_RENDERING_SOURCE}"
    "${BEE_RENDERING_HEADRES}"
)

IF (MSVC)

    TARGET_COMPILE_OPTIONS(B33Rendering PUBLIC "/permissive-")
    TARGET_COMPILE_OPTIONS(B33Rendering PUBLIC "/GR")

    IF (B33_DEBUG_BUILD STREQUAL "ON")

        TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE "/Z7")
        TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE "/Zi")
        TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE "/ZI")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE "/O2")

    ENDIF()


ELSE()

    TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE
        "-Wall"
        "-Werror"
        "-Wextra"
    )
    IF (B33_DEBUG_BUILD STREQUAL "ON")

        TARGET_COMPILE_OPTIONS(B33Rendering PRIVATE "-g")

    ELSE()

        TARGET_COMPILE_OPTIONS(B33Rendering PUBLIC "-O3")

    ENDIF()

ENDIF()

IF (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Windows")

    FIND_PACKAGE(Vulkan REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(B33Rendering PUBLIC
        "${Vulkan_INCLUDE_DIRS}"
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
    )

    TARGET_LINK_LIBRARIES(B33Rendering PUBLIC
        "${Vulkan_LIBRARIES}"
        B33Core
        B33Math
        B33App
    )

ENDIF()

# Macos uses molten-vk
IF (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    FIND_PATH(MOLTENVK_INCLUDE_DIR vulkan/vulkan.h HINTS "/usr/local/include" "/usr/local/opt/molten-vk/include")
    FIND_LIBRARY(MOLTENVK_LIB NAMES MoltenVK PATHS "/usr/local/lib" "/usr/local/opt/molten-vk/lib")

    IF (NOT MOLTENVK_INCLUDE_DIR OR NOT MOLTENVK_LIB)

        MESSAGE(FATAL_ERROR "Could not find MoltenVK. Please make sure it is installed.")

    ENDIF()

    TARGET_INCLUDE_DIRECTORIES(B33Rendering PUBLIC
        ${MOLTENVK_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Public/
    )

    TARGET_LINK_LIBRARIES(B33Rendering PUBLIC
        "-framework QuartzCore"
        "-framework Metal"
        "-framework Foundation"
        "-framework IOSurface"
        "-framework CoreGraphics"
        "-framework IOKit"
        "-framework AppKit"
        ${MOLTENVK_LIB}
        B33Core
        B33Math
        B33App
    )

ENDIF()


TARGET_PRECOMPILE_HEADERS(B33Rendering PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/B33Rendering.hpp")

TARGET_COMPILE_DEFINITIONS(B33Rendering PRIVATE _BEAST_EXPORTS _UNICODE UNICODE)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/shaders.cmake)


IF ( WIN32 )

    FIND_PROGRAM(
        DXC_PATH
        dxc
        HINTS "$ENV{VULKAN_SDK}/Bin"
        NO_DEFAULT_PATH
    )

ELSE ()

    FIND_PROGRAM(
        DXC_PATH
        dxc
    )

ENDIF ()

# Prefer HLSL shaders over GLSL
IF ( DXC_PATH STREQUAL "" )

    compileShadersGlslang("${CMAKE_CURRENT_SOURCE_DIR}/Public/Shaders" CompileShadersForVoxels)

ELSE ()

    compileShadersDXC("${DXC_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/Public/Shaders" CompileShadersForVoxels)

ENDIF ()

ADD_DEPENDENCIES(B33Rendering CompileShadersForVoxels)

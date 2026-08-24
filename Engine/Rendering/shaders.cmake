function(compileShadersGlslang SHADERS_PATH COMPILE_TARGET)

    SET(SHADER_SRC_DIR "${SHADERS_PATH}")
    SET(SHADER_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

    FILE(GLOB SHADERS "${SHADER_SRC_DIR}/glsl/*.comp.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_SHADERS)

    IF (SHADERS)

        FOREACH(SHADER_PATH ${SHADERS})

            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND glslangValidator -e main -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}")

            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND glslangValidator -e main -V -I${SHADER_SRC_DIR}/ ${SHADER_PATH} -o ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )

            LIST(APPEND COMPILED_SHADERS ${SPIRV_PATH})

        ENDFOREACH()

        ADD_CUSTOM_TARGET(${COMPILE_TARGET} ALL
            DEPENDS ${COMPILED_SHADERS}
            COMMENT "Compiling shaders"
        )

    ELSE()

        MESSAGE(WARNING "No .comp shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

endfunction()

function(compileShadersDXC DXC_PATH SHADERS_PATH COMPILE_TARGET)

    SET(SHADER_SRC_DIR "${SHADERS_PATH}")
    SET(SHADER_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

    FILE(GLOB COMPUTE_SHADERS "${SHADER_SRC_DIR}/hlsl/*.comp.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_COMPUTE_SHADERS)

    IF (COMPUTE_SHADERS)

        FOREACH(SHADER_PATH ${COMPUTE_SHADERS})

            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND ${DXC_PATH} -T cs_6_0 -E main -spirv ${SHADER_PATH} -Fo ${SPIRV_PATH}")

            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND ${DXC_PATH} -T cs_6_0 -E main -spirv -D VULKAN ${SHADER_PATH} -Fo ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )

            LIST(APPEND COMPILED_COMPUTE_SHADERS ${SPIRV_PATH})

        ENDFOREACH()


    ELSE()

        MESSAGE(WARNING "No .comp shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

    FILE(GLOB VERTEX_SHADERS "${SHADER_SRC_DIR}/hlsl/*.vert.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_VERTEX_SHADERS)

    IF (VERTEX_SHADERS)

        FOREACH(SHADER_PATH ${VERTEX_SHADERS})

            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND ${DXC_PATH} -T vs_6_0 -E main -spirv ${SHADER_PATH} -Fo ${SPIRV_PATH}")

            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND ${DXC_PATH} -T vs_6_0 -E main -spirv -D VULKAN ${SHADER_PATH} -Fo ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )

            LIST(APPEND COMPILED_VERTEX_SHADERS ${SPIRV_PATH})

        ENDFOREACH()

    ELSE()

        MESSAGE(WARNING "No .vert shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

    FILE(GLOB FRAG_SHADERS "${SHADER_SRC_DIR}/hlsl/*.frag.*")
    FILE(MAKE_DIRECTORY "${SHADER_BIN_DIR}")

    SET(COMPILED_FRAG_SHADERS)

    IF (FRAG_SHADERS)

        FOREACH(SHADER_PATH ${FRAG_SHADERS})

            GET_FILENAME_COMPONENT(SHADER_NAME ${SHADER_PATH} NAME_WE)
            SET(SPIRV_PATH "${SHADER_BIN_DIR}/${SHADER_NAME}.spv")
            MESSAGE("COMMAND ${DXC_PATH} -T ps_6_0 -E main -spirv ${SHADER_PATH} -Fo ${SPIRV_PATH}")

            ADD_CUSTOM_COMMAND(
                OUTPUT ${SPIRV_PATH}
                COMMAND ${DXC_PATH} -T ps_6_0 -E main -spirv -D VULKAN ${SHADER_PATH} -Fo ${SPIRV_PATH}
                DEPENDS ${SHADER_PATH}
                COMMENT "Compiling shader ${SHADER_NAME}"
                VERBATIM
            )

            LIST(APPEND COMPILED_FRAG_SHADERS ${SPIRV_PATH})

        ENDFOREACH()

    ELSE()

        MESSAGE(WARNING "No .frag shaders found in: ${SHADER_SRC_DIR}")

    ENDIF()

    ADD_CUSTOM_TARGET(${COMPILE_TARGET} ALL
        DEPENDS ${COMPILED_FRAG_SHADERS} 
                ${COMPILED_VERTEX_SHADERS} 
                ${COMPILED_COMPUTE_SHADERS} 
        COMMENT "Compiling shaders"
    )

endfunction()

# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(contadorBinario_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(contadorBinario_default_default_XC8_FILE_TYPE_assemble)
add_library(contadorBinario_default_default_XC8_assemble OBJECT ${contadorBinario_default_default_XC8_FILE_TYPE_assemble})
    contadorBinario_default_default_XC8_assemble_rule(contadorBinario_default_default_XC8_assemble)
    list(APPEND contadorBinario_default_library_list "$<TARGET_OBJECTS:contadorBinario_default_default_XC8_assemble>")
endif()

# Handle files with suffix S, for group default-XC8
if(contadorBinario_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(contadorBinario_default_default_XC8_assemblePreprocess OBJECT ${contadorBinario_default_default_XC8_FILE_TYPE_assemblePreprocess})
    contadorBinario_default_default_XC8_assemblePreprocess_rule(contadorBinario_default_default_XC8_assemblePreprocess)
    list(APPEND contadorBinario_default_library_list "$<TARGET_OBJECTS:contadorBinario_default_default_XC8_assemblePreprocess>")
endif()

# Handle files with suffix [cC], for group default-XC8
if(contadorBinario_default_default_XC8_FILE_TYPE_compile)
add_library(contadorBinario_default_default_XC8_compile OBJECT ${contadorBinario_default_default_XC8_FILE_TYPE_compile})
    contadorBinario_default_default_XC8_compile_rule(contadorBinario_default_default_XC8_compile)
    list(APPEND contadorBinario_default_library_list "$<TARGET_OBJECTS:contadorBinario_default_default_XC8_compile>")
endif()


add_executable(${contadorBinario_default_image_name} ${contadorBinario_default_library_list})
set_target_properties(${contadorBinario_default_image_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${contadorBinario_default_output_dir})

target_link_libraries(${contadorBinario_default_image_name} PRIVATE ${contadorBinario_default_default_XC8_FILE_TYPE_link})

# Add the link options from the rule file.
contadorBinario_default_link_rule(${contadorBinario_default_image_name})





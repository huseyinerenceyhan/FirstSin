cmake_minimum_required(VERSION 3.14)
project(FirstSin)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG}")

if(WIN32)
    set(SDL2_PATH "C:/SDL2/x86_64-w64-mingw32")
    set(SDL2_IMAGE_PATH "C:/SDL2_image/x86_64-w64-mingw32")
    set(SDL2_TTF_PATH "C:/SDL2_ttf/x86_64-w64-mingw32")
    set(SDL2_MIXER_PATH "C:/SDL2_mixer/x86_64-w64-mingw32")

    set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include/SDL2")
    set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_PATH}/include/SDL2")
    set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_PATH}/include/SDL2")
    set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_PATH}/include/SDL2")

    set(SDL2_LIBRARIES
                "${SDL2_PATH}/lib/libSDL2.dll.a"
                "${SDL2_PATH}/lib/libSDL2main.a"
            "${SDL2_IMAGE_PATH}/lib/libSDL2_image.dll.a"
                "${SDL2_TTF_PATH}/lib/libSDL2_ttf.dll.a"
                "${SDL2_MIXER_PATH}/lib/libSDL2_mixer.dll"
    )
    set(EXTERNAL_LIB_FILES
                "${SDL2_PATH}/bin/SDL2.dll"
                "${SDL2_IMAGE_PATH}/bin/SDL2_image.dll"
                "${SDL2_TTF_PATH}/bin/SDL2_ttf.dll"
                "${SDL2_MIXER_PATH}/bin/SDL2_mixer.dll"
    )
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_INCLUDE_DIRS})
    set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_INCLUDE_DIRS})
    set(SDL2_MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIRS})

    set(SDL2_LIBRARIES
                SDL2::SDL2
                SDL2_image::SDL2_image
                SDL2_ttf::SDL2_ttf
                SDL2_mixer::SDL2_mixer
    )

    set(EXTERNAL_LIB_FILES
        "/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0"
        "/usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so.0"
        "/usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0"
        "/usr/lib/x86_64-linux-gnu/libSDL2_mixer-2.0.so.0"
    )
endif()

add_executable(FirstSin WIN32
        src/main.cpp
        src/Snake.cpp
        src/SpriteSheet.cpp
        src/Timer.cpp
        src/Apple.cpp
        src/Custom_Math.cpp
        src/Grid.cpp
        src/Game.cpp
)

if(NOT WIN32)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
    pkg_check_modules(SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf)
    pkg_check_modules(SDL2_mixer REQUIRED IMPORTED_TARGET SDL2_mixer)

    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_image_INCLUDE_DIRS})
    set(SDL2_TTF_INCLUDE_DIR ${SDL2_ttf_INCLUDE_DIRS})
    set(SDL2_MIXER_INCLUDE_DIR ${SDL2_mixer_INCLUDE_DIRS})

    set(SDL2_LIBRARIES
        PkgConfig::SDL2
        PkgConfig::SDL2_image
        PkgConfig::SDL2_ttf
        PkgConfig::SDL2_mixer
    )

    set_property(TARGET FirstSin PROPERTY BUILD_RPATH "$ORIGIN/lib")
endif()


target_include_directories(FirstSin PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(FirstSin PRIVATE ${SDL2_LIBRARIES})

add_custom_command(TARGET FirstSin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:FirstSin>/assets"
)

add_custom_command(TARGET FirstSin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:FirstSin>/lib"
)

foreach(lib ${EXTERNAL_LIB_FILES})
    if(EXISTS "${lib}")
        add_custom_command(TARGET FirstSin POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${lib}" "$<TARGET_FILE_DIR:FirstSin>/lib/"
        )
    else()
        message(WARNING "Skipping copy of non-existent library: ${lib}")
    endif()
endforeach()

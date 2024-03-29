# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_library(
        native_app_glue
        STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c

)
set( lib_ndk_helper_DIR ${ANDROID_NDK}/sources/android/ndk_helper )
add_library(
        ndk_helper
        STATIC
        ${lib_ndk_helper_DIR}/JNIHelper.cpp
        ${lib_ndk_helper_DIR}/interpolator.cpp
        ${lib_ndk_helper_DIR}/tapCamera.cpp
        ${lib_ndk_helper_DIR}/gestureDetector.cpp
        ${lib_ndk_helper_DIR}/perfMonitor.cpp
        ${lib_ndk_helper_DIR}/vecmath.cpp
        ${lib_ndk_helper_DIR}/GLContext.cpp
        ${lib_ndk_helper_DIR}/shader.cpp
        ${lib_ndk_helper_DIR}/gl3stub.c

)




# Sets lib_src_DIR to the path of the target CMake project.
set( lib_src_DIR ${ANDROID_NDK}/sources/libpng )
target_include_directories(
        native_app_glue
        PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue

        ${ANDROID_NDK}/sources/glm
       # ${lib_src_DIR}/jni

)

target_include_directories(
        ndk_helper
        PUBLIC
        #${ANDROID_NDK}/sources/android/native_app_glue
        ${ANDROID_NDK}/sources/android/ndk_helper

        # ${lib_src_DIR}/jni

)

include_directories(
        ${lib_src_DIR}/jni
        ${ANDROID_NDK}/sources/android/native_app_glue
        ${ANDROID_NDK}/sources/android/ndk_helper)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")



# library and names it lib_gmath.
add_library( libpng STATIC IMPORTED )
set_target_properties( # Specifies the target library.
        libpng

        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION

        # Provides the path to the library you want to import.
        ${lib_src_DIR}/obj/local/${ANDROID_ABI}/libpng.a )


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


add_library( # Sets the name of the library.
        horse-engine

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        act/main.cpp
        act/Windows.cpp
        act/ShadersManager.cpp
        act/FileRead.cpp
        act/Asset.cpp
        act/GLHelper.cpp
        )



add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp

        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
target_link_libraries( # Specifies the target library.
        horse-engine
        android
        native_app_glue
        ndk_helper
        EGL
        GLESv2
        libpng
        z
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
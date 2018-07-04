cmake_minimum_required (VERSION 3.0)
project (VWSI)

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(VWSI_INSTALL "Generate installation target" OFF)
option(VWSI_BUILD_TESTING "Build the WSI test programs" ON)
option(VWSI_SETUP_OUTPUT "If WSI will set it's own outputs" ON)

set(LIB_TYPE STATIC)

if(BUILD_SHARED_LIBS)
	set(LIB_TYPE SHARED)
endif()

message(STATUS "Shared Libs: ${BUILD_SHARED_LIBS}")

# Cmake Modules
list(APPEND CMAKE_MODULE_PATH "${VWSI_SOURCE_DIR}/cmake")

# Preprocessor
if (VWSI_SETUP_OUTPUT)
    find_package(Vulkan REQUIRED)
    set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${VULKAN_INCLUDE_DIR})
    set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${VULKAN_LIBRARY}")
endif()

if(WIN32)
	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX AND NOT APPLE)
	find_package(XCB REQUIRED)
    set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${XCB_INCLUDES})
    set(LIBRARIES_LINKS ${LIBRARIES_LINKS} ${XCB_LIBRARIES})
	add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
elseif(UNIX AND APPLE)
	add_definitions(-DVK_USE_PLATFORM_COCOA_KHR)
elseif(ANDROID)
	add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
endif()

# Compiler Options
if (VWSI_SETUP_OUTPUT)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED ON)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

	if(MSVC)
		add_definitions(-D_SCL_SECURE_NO_WARNINGS)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /std:c++latest /Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /arch:SSE2 /MP /W3 /wd4251")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /RTC1 /MDd -DDEBUG")

		if (CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /OPT:ICF")
		else()
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
		endif()
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -Wno-unused-variable")
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -mavx -msse3 -mfpmath=sse") # -Wextra -Werror -pedantic
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -DNDEBUG -Wno-unused-variable")
	endif()

	if(ANDROID)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
		list(APPEND LIBRARIES_INCLUDES ${ANDROID_NDK}/sources/android/native_app_glue/)
		list(APPEND LIBRARIES_LINKS "android log")
	endif()

	# Output Directories
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/bin)

	set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_BINDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	set(CMAKE_INSTALL_INCLUDEDIR ${PROJECT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/include)
endif()

# Sources
add_subdirectory(src)

# Test
if (VWSI_BUILD_TESTING)
	add_subdirectory(test)
endif()
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

cmake_minimum_required (VERSION 3.8)
project (PhysicsSim)
configure_file(${CMAKE_SOURCE_DIR}/res/ResourceDir.h.in ${CMAKE_SOURCE_DIR}/src/h/ResourceDir.h)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/h/*.h
	${CMAKE_SOURCE_DIR}/src/h/*.hpp
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${CMAKE_SOURCE_DIR}/include/imgui/*.h
	${CMAKE_SOURCE_DIR}/include/imgui/*.cpp
	${CMAKE_SOURCE_DIR}/vendor/glad.h
	${CMAKE_SOURCE_DIR}/vendor/glad.c
)

add_executable(PhysicsSim ${SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/vendor)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8)

target_link_libraries(PhysicsSim
	glfw
	OpenGL::GL
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PhysicsSim PROPERTY CXX_STANDARD 20)
endif()


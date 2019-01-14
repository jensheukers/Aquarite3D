cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_SUPPRESS_REGENERATION true) 

project (Aquarite3D)

# Main engine files
file(GLOB MAIN "aquarite/*.cpp" "aquarite/*.h" )
file(GLOB MATH "aquarite/math/*.cpp" "aquarite/math/*.h")
file(GLOB GRAPHICS "aquarite/graphics/*.cpp" "aquarite/graphics/*.h")
file(GLOB GAME "game/*.cpp" "game/*.h") 

# Includes
set(GLFW_DIR "external/glfw")
set(GLEW_DIR "external/glew")
set(GLM_DIR "external/glm")
set(OPENAL_DIR "external/openal")
set(ALUT_DIR "external/alut")
set(VORBIS_DIR "external/oggvorbis")

include_directories(${GLFW_DIR}/include ${GLEW_DIR}/include ${GLM_DIR} ${OPENAL_DIR}/include ${VORBIS_DIR}/include)
link_directories(${GLFW_DIR}/lib-vc2015 ${GLEW_DIR}/lib/Win32 ${OPENAL_DIR}/libs/Win32
				 ${VORBIS_DIR}/lib/Win32)

# Add Executable

add_executable(Aquarite3D ${MAIN} ${MATH} ${GRAPHICS} ${GAME})

set(CMAKE_CXX_FLAGS_RELEASE "/MD")
set(CMAKE_CXX_FLAGS_DEBUG "/MD")

# Link libraries
target_link_libraries(Aquarite3D glfw3.lib glfw3dll.lib opengl32.lib glew32.lib glew32s.lib OpenAL32.lib libogg.lib libvorbis.lib libvorbisfile.lib)
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

# TODO: Set Commands
add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2015/glfw3.dll $<TARGET_FILE_DIR:Aquarite3D>)

add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glew/bin/Win32/glew32.dll $<TARGET_FILE_DIR:Aquarite3D>)
						
add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/openal/bin/Win32/OpenAL32.dll $<TARGET_FILE_DIR:Aquarite3D>)

add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/external/oggvorbis/bin $<TARGET_FILE_DIR:Aquarite3D>)
						
						
add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Aquarite3D>/shaders)

add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/aquarite/shaders $<TARGET_FILE_DIR:Aquarite3D>/shaders)
						
add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Aquarite3D>/res)

add_custom_command(TARGET Aquarite3D POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/game/res $<TARGET_FILE_DIR:Aquarite3D>/res)
# Filter groups
source_group("aquarite" FILES ${MAIN})
source_group("math" FILES ${MATH})
source_group("graphics" FILES ${GRAPHICS})
source_group("game" FILES ${GAME})





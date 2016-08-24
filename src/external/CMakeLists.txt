add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
	)

# GLFW
add_subdirectory(glfw)

include_directories(
	glfw/include/GLFW/
	glew/include/
	)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(OPENGL_LIBRARIES
		${OPENGL_LIBRARY}
		-lGL -lGLU -lXrandr -lXext -lX11 -lrt
		${CMAKE_DL_LIBS}
		#glfw
		#${OPENGL_glu_LIBRARY}
		${GLFW_LIBRARIES}
		)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(OPENGL_LIBRARIES
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
		${GLFW_LIBRARIES}
		)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#GLEW
set(GLEW_SOURCE 
	glew/src/glew.c)

set(GLEW_HEADERS
	)

add_library(GLEW STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
	)

target_link_libraries(GLEW
	${OPENGL_LIBRARIES}
	${EXTRA_LIBS}
	)

# SOIL
file(GLOB_RECURSE SOIL_SOURCE "soil/*.c")

set(SOIL_HEADERS
	)

add_library(SOIL STATIC
	${SOIL_SOURCE}
	)

target_link_libraries(SOIL
	${OPENGL_LIBRARIES}
	${EXTRA_LIBS}
	)


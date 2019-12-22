cmake_minimum_required(VERSION 3.1)

project(ayla_gl_sample_trackball_camera)

include_directories(
	${AYLA_GL_INCLUDE_DIR}
	${3RD_PARTY_DIR}/glfw/include
)

add_executable(ayla_gl_sample_trackball_camera ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
target_link_libraries(ayla_gl_sample_trackball_camera glfw ${GLFW_LIBRARIES} ${AYLA_GL_LIBRARY})

TARGET_DYNAMIC_LIBRARIES(ayla_gl_sample_trackball_camera
	${AYLA_GL_LIBRARY}
	glfw
)
TARGET_BOOST_DYNAMIC_LIBRARIES(ayla_gl_sample_trackball_camera)

install(TARGETS ayla_gl_sample_trackball_camera
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

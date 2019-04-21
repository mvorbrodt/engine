add_executable(main
	main.cpp
	opengl.cpp
	shader.cpp
	texture.cpp
	vertex_array.cpp
	loader.cpp
	../3rd_party/glad/src/glad.c)
target_link_libraries(main ${OPENGL_gl_LIBRARY} -lglfw -lassimp -ldl)

add_executable(asset_info asset_info.cpp)
target_link_libraries(asset_info -lassimp -ldl)

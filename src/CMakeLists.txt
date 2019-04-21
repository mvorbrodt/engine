add_executable(main
	main.cpp
	opengl.cpp
	shader.cpp
	texture.cpp
	vertex_array.cpp
	loader.cpp
	../3rd_party/glad/src/glad.c)
set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS -DGL_SILENCE_DEPRECATION)
target_link_libraries(main ${OPENGL_gl_LIBRARY} -lglfw -lassimp -ldl)

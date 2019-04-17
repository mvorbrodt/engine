add_executable(main
	main.cpp
	opengl.cpp
	shader.cpp
	texture.cpp
	vertex_array.cpp
	../3rd_party/stb/stb_image.cpp
	../3rd_party/tinyobjloader/tiny_obj_loader.cc)
set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS -DGL_SILENCE_DEPRECATION)
target_link_libraries(main ${OPENGL_gl_LIBRARY} -lglfw -lglad -ldl -lassimp)

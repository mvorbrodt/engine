add_compile_definitions(GL_SILENCE_DEPRECATION)
add_executable(main main.cpp)
target_link_libraries(main ${OPENGL_gl_LIBRARY} ${GLUT_glut_LIBRARY} -lglfw -lglad)

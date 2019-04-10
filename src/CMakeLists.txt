add_executable(main main.cpp)
target_link_libraries(main ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_glut_LIBRARY} ${GLEW_LIBRARY})

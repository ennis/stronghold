cmake_minimum_required(VERSION 2.6)

PROJECT(stronghold)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CC_COMPILER g++)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_PACKAGE(OpenGL REQUIRED)
    FIND_PACKAGE(GLFW REQUIRED)
    FIND_PACKAGE(GLEW REQUIRED)
    include("cmake/FindAssimp.cmake")
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include("cmake/FindGLEW.cmake")
    include("cmake/FindGLFW.cmake")
    include("cmake/FindAssimp.cmake")
    set(GLM_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/local/include/")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

FIND_PACKAGE(OpenGL REQUIRED)

ADD_DEFINITIONS(-DGLM_FORCE_RADIANS)

SET(GRAMMAR_LEXER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/code/antlr/GrammarLexer.cpp)
SET(GRAMMAR_PARSER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/code/antlr/GrammarParser.cpp)

# antlr
IF(WIN32)
IF($ENV{USERNAME} STREQUAL "Okami_no_Shu")
ADD_CUSTOM_COMMAND(
    OUTPUT ${GRAMMAR_LEXER_SOURCE} ${GRAMMAR_PARSER_SOURCE}
    COMMAND "/Program Files/Java/jre7/bin/java" -cp "${PROJECT_SOURCE_DIR}/local/bin/antlr-3.4-with-cpp.jar;\$CLASSPATH" org.antlr.Tool ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/antlr
    DEPENDS ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
)
ELSE()
ADD_CUSTOM_COMMAND(
    OUTPUT ${GRAMMAR_LEXER_SOURCE} ${GRAMMAR_PARSER_SOURCE}
	COMMAND java -cp "${PROJECT_SOURCE_DIR}/local/bin/antlr-3.4-with-cpp.jar;\$CLASSPATH" org.antlr.Tool ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/antlr
    DEPENDS ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
)
ENDIF()
ELSE()
ADD_CUSTOM_COMMAND(
    OUTPUT ${GRAMMAR_LEXER_SOURCE} ${GRAMMAR_PARSER_SOURCE}
    COMMAND java -cp "${PROJECT_SOURCE_DIR}/local/bin/antlr-3.4-with-cpp.jar:\$CLASSPATH" org.antlr.Tool ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/antlr
    DEPENDS ${PROJECT_SOURCE_DIR}/code/antlr/Grammar.g
)
ADD_DEFINITIONS(-g -ggdb)
ENDIF()

#includes pour nos headers
INCLUDE_DIRECTORIES(
    code/inc
    code/inc/grammar
    code/antlr/
    code/inc/shape
    code/inc/renderable
    local/include
    )

LINK_DIRECTORIES(
    ${GLFW_LIBRARY_DIR}
    )

file(
    GLOB
    stronghold_SOURCES
    code/src/*.cpp
    code/src/grammar/*.cpp
    code/src/shape/*.cpp
    code/src/renderable/*.cpp
    )

file(
    GLOB
    stronghold_HEADERS
    code/inc/*.hpp
    code/inc/grammar/*.hpp
    code/inc/shape/*.hpp
    code/inc/renderable/*.hpp
    )

INCLUDE_DIRECTORIES(
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    )

ADD_EXECUTABLE(
    main
    ${stronghold_SOURCES}
    ${stronghold_HEADERS}
    ${GRAMMAR_LEXER_SOURCE} 
    ${GRAMMAR_PARSER_SOURCE}
    )

# 'make run'
ADD_CUSTOM_TARGET(run
    COMMAND main grammars/step3.in
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/code/
)

# MacOS (Jocelyn)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    TARGET_LINK_LIBRARIES(
        main
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${ASSIMP_LIBRARY_RELEASE}
        )
# Windows
ELSEIF(WIN32)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
     TARGET_LINK_LIBRARIES(
        main
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${ASSIMP_LIBRARY_RELEASE}
        )
# Linux (PC imag)
ELSE()
    TARGET_LINK_LIBRARIES(
        main
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${ASSIMP_LIBRARY_RELEASE}
        -lX11
        -lXxf86vm
        -lXrandr
        -lpthread
        -lXi
        -lXcursor
        -lassimp
        )
ENDIF()

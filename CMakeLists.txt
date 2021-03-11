# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(test)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

list(APPEND SRC
	align.h
	test.cpp
  )
list(APPEND INCL
  ${CMAKE_CURRENT_LIST_DIR}/.
  lib/STFT/cpp
  lib/WAV
  )
if(MSVC)
list(APPEND LINKLIBS
  )
else()
list(APPEND LINKLIBS
	-fopenmp
	)
endif()
if(MSVC)
  list(APPEND COMPILE_OPTION
	/MP
	/openmp
    )
else()
  list(APPEND COMPILE_OPTION
		m
		-fopenmp
		)
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 17 )
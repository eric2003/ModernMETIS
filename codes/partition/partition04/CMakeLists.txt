cmake_minimum_required(VERSION 3.20)

project ( testprj )

set ( PRJ_INCLUDE_DIRS )
set ( PRJ_LIBRARIES )
set ( PRJ_LINK_DIRS )
set ( PRJ_COMPILE_FEATURES )

set ( METIS_INCLUDE_DIRS $ENV{METIS_HOME_INC} )
set ( METIS_LIBRARIES $ENV{METIS_HOME_LIB} )

list ( APPEND PRJ_LIBRARIES ${METIS_LIBRARIES} )
list ( APPEND PRJ_INCLUDE_DIRS ${METIS_INCLUDE_DIRS} )
list ( APPEND PRJ_COMPILE_FEATURES cxx_std_17 )

set( MAIN_SRC
    main.cpp
)

add_executable( ${PROJECT_NAME} ${MAIN_SRC} )

target_include_directories ( ${PROJECT_NAME}
    PRIVATE 
        ${PRJ_INCLUDE_DIRS}
)

target_link_libraries ( ${PROJECT_NAME} 
    PRIVATE 
        ${PRJ_LIBRARIES}
)

target_link_directories ( ${PROJECT_NAME} 
    PRIVATE 
        ${PRJ_LINK_DIRS}
)

target_compile_features ( ${PROJECT_NAME} 
    PRIVATE 
        ${PRJ_COMPILE_FEATURES}
)

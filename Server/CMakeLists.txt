project(Server LANGUAGES CXX VERSION 0.9.4 DESCRIPTION "Burury server daemon")

set(CMAKE_INSTALL_RPATH "${BINARY_DIR}/Server/lib")

#####################
#  finding sources  #
#####################

set(SRCS "")

# Sources
file(GLOB_RECURSE SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.hpp")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

#########################
#  building executable  #
#########################

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt6_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${SRCS}
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${SRCS}
        )
    else()
        add_executable(${PROJECT_NAME}
            ${SRCS}
        )
    endif()
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        iiServer
        Logger
        Network
        API
        Crypto
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

################################
#  setting project properties  #
################################

set(OUTPUT_NAME "bururyd")

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME    ${OUTPUT_NAME}
    SUFFIX         ""
    PREFIX         ""
    VERSION        "${PROJECT_VERSION}"
    PUBLIC_HEADER  "${HDRS_G}"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

######################
#  installing files  #
######################

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${BINARY_DIR}/Server
)

get_target_property(DEPS ${PROJECT_NAME} LINK_LIBRARIES)
foreach (DEP ${DEPS})
    install(
        FILES $<TARGET_FILE:${DEP}>
        DESTINATION ${BINARY_DIR}/Server/lib)
endforeach()

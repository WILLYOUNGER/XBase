cmake_minimum_required(VERSION 3.5)

############################################################
# Create a library
############################################################

message("--------Cur Src Dir: " ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE XPUBLIC_SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/*/*.cpp)
message("--------TEST RESULT: " ${XPUBLIC_SRC_LIST})


add_library(publib SHARED 
    ${XPUBLIC_SRC_LIST}
)
add_library(X::PUBLIB ALIAS publib)

target_link_libraries(publib PUBLIC pthread)
target_link_libraries(publib
    PUBLIC 
        X::THIRD
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XFrames
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XFrames/XServlet
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XGame
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XGame/XDesignBase
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XGame/XStatistics
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XStruct
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XHTTP
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XLog
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XNet
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XPthread
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XUtils
)

target_include_directories(publib
    PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/XTools/XServer
)
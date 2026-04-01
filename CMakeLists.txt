cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(deps)

set(TARGET YetAnotherPvPFix)
project(${TARGET})

file(GLOB SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dllmain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ModConfig.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/Classes/PalMapObjectDamageReactionComponent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/Classes/PalMapObjectUtility.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/Classes/PalOptionSubsystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/Classes/PalUtility.cpp"
)

add_library(${TARGET} SHARED ${SRC_FILES})
target_include_directories(${TARGET} PRIVATE 
    include/
)
target_link_libraries(${TARGET} PRIVATE Zydis Zycore safetyhook UE4SS)
include(FetchContent)

FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY https://github.com/cursey/safetyhook.git
    GIT_TAG 629558c64009a7291ba6ed5cfb49187086a27a47
)

FetchContent_MakeAvailable(safetyhook)
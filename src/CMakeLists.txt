
set(INCLUDED_PATCH_SOURCE "${NATIVEBRIDGE}_${NB_VERSION}_patches.cpp")

add_library(nb SHARED main.cpp ${INCLUDED_PATCH_SOURCE})

target_link_libraries(nb log)


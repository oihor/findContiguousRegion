include_directories(
  inc/
)
set(findcontiguousregion_src_files
  src/findcontiguousregion.cpp
)
add_library(findcontiguousregion ${findcontiguousregion_src_files})
target_include_directories(findcontiguousregion PUBLIC inc/)
target_link_libraries(findcontiguousregion opencv_core)
#install(TARGETS findcontiguousregion DESTINATION /lib)
#install(FILES inc/findcontiguousregion.h DESTINATION /inc)

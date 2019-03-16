include_directories(
)
set(cli_libs
  findcontiguousregion
)
set(cli_src_files
  src/maincli.cpp
)
add_executable(findcontiguousregioncli ${cli_src_files})
target_link_libraries(findcontiguousregioncli ${cli_libs})
#install(TARGETS findcontiguousregioncli DESTINATION /bin)

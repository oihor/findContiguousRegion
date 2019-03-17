include_directories(
)
set(cli_libs
  findcontiguousregion
)
set(cli_src_files
  src/maincli.cpp
)
add_executable(findcontiguousregioncli ${cli_src_files})
target_link_libraries(findcontiguousregioncli 
  ${cli_libs}
  boost_program_options
  boost_system
  boost_filesystem
  opencv_imgcodecs
  opencv_highgui
)
#install(TARGETS findcontiguousregioncli DESTINATION /bin)

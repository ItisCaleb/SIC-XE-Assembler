add_executable(sic-loader
 loader.cc
 sic_program.cc)

target_link_libraries(sic-loader PRIVATE fmt::fmt)
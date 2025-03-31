add_executable(sic-assembler 
    main.cc 
    sic_parser.cc 
    sic_table.cc
    sic_assembler.cc)

target_link_libraries(sic-assembler fmt::fmt)
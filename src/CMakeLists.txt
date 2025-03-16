add_executable(assembler 
    main.cc 
    sic_parser.cc 
    sic_table.cc
    sic_assembler.cc)

target_link_libraries(assembler fmt::fmt)
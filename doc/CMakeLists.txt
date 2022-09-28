message(STATUS "Doxygen configuration start")
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in @ONLY)
    add_custom_target(doc_panoramyx
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
else()
    message(STATUS "Doxygen not found, not building docs")
endif (DOXYGEN_FOUND)
message(STATUS "Doxygen configuration end")
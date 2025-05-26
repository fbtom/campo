add_library(image_process INTERFACE)
target_sources(image_process INTERFACE   
                                ${CMAKE_CURRENT_SOURCE_DIR}/image_processor.hpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/image_processor_manager.hpp
                                )

target_include_directories(image_process INTERFACE 
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_process INTERFACE ImGui image_history image_decorator)

target_open_cv(image_process)

add_library(Operations 
            Extract.cpp
            Correction.cpp
            Create.cpp
            List.cpp
            Append.cpp
            Delete.cpp
            Merge.cpp
            Hamming,Read,Write.cpp
            )

target_include_directories(Operations PUBLIC ${PROJECT_SOURCE_DIR}/lib)
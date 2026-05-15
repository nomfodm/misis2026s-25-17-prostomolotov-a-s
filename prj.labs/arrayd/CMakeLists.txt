add_library(arrayd arrayd.hpp)
target_sources(arrayd
    PRIVATE
        arrayd.cpp
    PUBLIC
        FILE_SET arrayd
        TYPE HEADERS
        BASE_DIRS .
        FILES arrayd.hpp
)

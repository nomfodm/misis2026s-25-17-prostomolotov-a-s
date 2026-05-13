add_library(bitsetd
        bitsetd.hpp)
target_sources(bitsetd
    PRIVATE
        bitsetd.cpp

    PUBLIC
        FILE_SET bitsetd
        TYPE HEADERS
        BASE_DIRS .
        FILES bitsetd.hpp

)

# add_library(<name> <cpp> <hpp>)
#
# CMake module for Ripe cryptography wrapper
#
# Creates ${RIPE_INCLUDE_DIR} and ${RIPE_LIBRARY}
#
# If ${RIPE_USE_STATIC_LIBS} is ON then static libs are preferred over shared
#
# Copyright 2017-present @abumq (Majid Q.)
#
# https://github.com/abumq/ripe
#

set(RIPE_PATHS ${RIPE_ROOT} $ENV{RIPE_ROOT})

find_path(RIPE_INCLUDE_DIR
    Ripe.h
    PATH_SUFFIXES include
    PATHS ${RIPE_PATHS}
)

if (RIPE_USE_STATIC_LIBS)
    message ("-- Ripe: Static linking")
    find_library(RIPE_LIBRARY
        NAMES libripe.a
        HINTS "${CMAKE_PREFIX_PATH}/lib"
    )
else()
    message ("-- Ripe: Dynamic linking")
    find_library(RIPE_LIBRARY
        NAMES libripe.dylib libripe.so
        HINTS "${CMAKE_PREFIX_PATH}/lib"
    )
endif()

message ("-- Ripe: Include: " ${RIPE_INCLUDE_DIR} ", Binary: " ${RIPE_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Ripe REQUIRED_VARS RIPE_INCLUDE_DIR RIPE_LIBRARY)

cmake_minimum_required (VERSION 2.8.12)

project (licensepp CXX)

option (test "Build all tests" OFF)
option (BUILD_SHARED_LIBS "build shared libraries" ON)
option (travis "Travis CI" OFF)

set (LICENSEPP_MAJOR "1")
set (LICENSEPP_MINOR "0")
set (LICENSEPP_PATCH "6")

set (LICENSEPP_SOVERSION "${LICENSEPP_MAJOR}.${LICENSEPP_MINOR}.${LICENSEPP_PATCH}")
set (LICENSEPP_NAME "licensepp")

add_definitions (-DLICENSEPP_SOVERSION="${LICENSEPP_SOVERSION}")
add_definitions (-DRIPE_VERSION="4.0.1-custom-static")
if (travis)
    add_definitions (-DLICENSEPP_ON_CI)
endif()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include (FindPackageHandleStandardArgs)

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")
endif()

# Check for cryptopp (static)
set(CryptoPP_USE_STATIC_LIBS ON)
find_package(CryptoPP REQUIRED)
message ("-- Crypto++ binary: " ${CRYPTOPP_LIBRARY})
include_directories (${CRYPTOPP_INCLUDE_DIRS})


set(LICENSEPP_SOURCE_FILES
    license++/license-exception.h
    license++/license.h
    license++/issuing-authority.h
    license++/base-license-manager.h
    src/external/Ripe.cc
    src/utils.cc
    src/json-object.cc
    src/crypto/aes.cc
    src/crypto/base64.cc
    src/crypto/base16.cc
    src/crypto/rsa.cc
    src/issuing-authority.cc
    src/license.cc
)

add_library (licensepp-lib ${LICENSEPP_SOURCE_FILES})

set_target_properties (licensepp-lib PROPERTIES
    VERSION ${LICENSEPP_SOVERSION}
)
target_include_directories (licensepp-lib PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries (licensepp-lib
    ${CRYPTOPP_LIBRARIES}
)

set_target_properties (licensepp-lib PROPERTIES OUTPUT_NAME "licensepp")
install (TARGETS licensepp-lib EXPORT licensepp-config DESTINATION lib)
install (FILES license++/base-license-manager.h DESTINATION "include/license++")
install (FILES license++/issuing-authority.h DESTINATION "include/license++")
install (FILES license++/license.h DESTINATION "include/license++")
install (FILES license++/license-exception.h DESTINATION "include/license++")
install (EXPORT licensepp-config DESTINATION share/licensepp/cmake)
export (TARGETS licensepp-lib FILE licensepp-config.cmake)


############## Cmake Package #################

# Packaging config.
set (CPACK_PACKAGE_NAME "licensepp")
set (CPACK_PACKAGE_VERSION_MAJOR ${LICENSEPP_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${LICENSEPP_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${LICENSEPP_PATCH})
set (CPACK_SOURCE_GENERATOR TGZ)
set (CPACK_SOURCE_IGNORE_FILES
    "/build/"
)
include (CPack)

# Compile-time configuration.
configure_file (
    ${CMAKE_SOURCE_DIR}/cmake/config.h.cmake
    ${CMAKE_BINARY_DIR}/config.h
)

include_directories (${CMAKE_BINARY_DIR})
include_directories (${CMAKE_SOURCE_DIR})

if (test)

    find_package (gtest REQUIRED)

    include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    enable_testing()

    add_executable (licensepp-unit-tests
        test/license-manager-for-test.h
        test/license-manager-test.h
        test/main.cc
        test/test.h
    )

    # Standard linking to gtest stuff.
    target_link_libraries (licensepp-unit-tests gtest gtest_main)

    # Extra linking for the project.
    target_link_libraries (licensepp-unit-tests licensepp-lib)

    target_link_libraries (licensepp-unit-tests ${CRYPTOPP_LIBRARIES})

    add_test (NAME licenseppUnitTests COMMAND licensepp-unit-tests)
endif() ## test

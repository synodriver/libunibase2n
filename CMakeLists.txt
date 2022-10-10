cmake_minimum_required(VERSION 3.20)
project(libunibase2n C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

include(TestBigEndian)
test_big_endian(isBigEndian)
if (${isBigEndian})
    MESSAGE("isBigEndian")
    add_definitions(-DWORDS_BIGENDIAN)
else()
    MESSAGE("issmallEndian")
endif()



add_library(libunibase2n SHARED src/unibase2n.c src/uint128.c)
set_target_properties(libunibase2n PROPERTIES  OUTPUT_NAME unibase2n)

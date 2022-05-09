# Copyright (c) 2022 - ∞ blurryroots innovation qanat OÜ
CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

# set up versioning.
set(BUILD_MAJOR "0")
set(BUILD_MINOR "6")
set(BUILD_PATCH "1")
set(BUILD_MICRO "8")
set(BUILD_VERSION "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH}.${BUILD_MICRO}")

project (Carter VERSION ${BUILD_VERSION})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE PATH "" FORCE)
endif()

string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    IS_STANDALON_PROJECT
)

#
option(BUILD_SHARED_LIBS "Enable build of shared libraries" NO)
option(BCRYPT "Switches RNG to Bcrypt" NO)

#
include(CheckIncludeFile)
include(CheckSymbolExists)
check_include_file("bsd/stdlib.h" HAVE_BSD_STDLIB_H)
if (HAVE_BSD_STDLIB_H)
message(WARNING "Looking for arc4random in bsd/stdlib.h ...")
check_symbol_exists(arc4random "bsd/stdlib.h" HAVE_ARC4RANDOM)
endif()
if (NOT HAVE_ARC4RANDOM)
	message(WARNING "Looking for arc4random in stdlib.h ...")
	check_include_file("stdlib.h" HAVE_STDLIB_H)
	check_symbol_exists(arc4random "stdlib.h" HAVE_ARC4RANDOM)
endif()
if (HAVE_ARC4RANDOM)
	add_definitions(-DWITH_ARC4RANDOM=1)
	message(WARNING "System has arc4.")
else()
	add_definitions(-DWITH_ARC4RANDOM=0)
	add_definitions(-DRANDOLPH_DEFINE_ARC4=1)
	message(WARNING "No arc4 found, will define it later.")
endif()

#
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
	add_definitions(-Dstrdup=_strdup)
	if(BCRYPT)
		add_definitions(-DUSE_BCRYPT=1)
	else()
		add_definitions(-DUSE_BCRYPT=0)
	endif()
endif()

#
file (GLOB Carter_SRC
	"${PROJECT_SOURCE_DIR}/src/carter/*.c")
if(BUILD_SHARED_LIBS)
	add_library(Carter SHARED ${Carter_SRC})
else()
	add_library(Carter STATIC ${Carter_SRC})
endif()
target_include_directories(Carter PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>"
)
if(WIN32)
	find_library(ADVAPI32 advapi32.lib)
	if(BCRYPT)
		find_library(BCRYPT bcrypt.lib)
		set(Carter_Libraries ADVAPI32 BCRYPT)
	else()
		find_library(CRYPT32 crypt32.li)
		set(Carter_Libraries ADVAPI32 CRYPT32)
	endif()
	target_link_libraries(Carter ${Carter_Libraries})
endif()
set_property(TARGET Carter PROPERTY C_STANDARD 11)

#
if (Carter_WithTest)
	file(GLOB Carter_Test_SRC	
		"${PROJECT_SOURCE_DIR}/test/*.c"
		"${PROJECT_SOURCE_DIR}/test/**/*.c")
	add_executable (CarterTest ${Carter_Test_SRC})
	target_include_directories(CarterTest PUBLIC
	  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>"
	  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/test/src>"
	)
	#target_link_libraries(CarterTest Carter Testly)
	target_link_libraries(CarterTest Carter)
	set_property(TARGET CarterTest PROPERTY C_STANDARD 11)
endif()
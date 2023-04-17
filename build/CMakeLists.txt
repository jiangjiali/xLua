cmake_minimum_required(VERSION 2.8)

if ( WIN32 AND NOT CYGWIN AND NOT ( CMAKE_SYSTEM_NAME STREQUAL "WindowsStore" ) AND NOT ANDROID)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "")
endif ()

project(XLua)

option ( UINT_ESPECIALLY "using custom ulong" OFF )
option ( GC64 "using gc64" OFF )
option ( LUAC_COMPATIBLE_FORMAT "compatible format" OFF )

find_path(XLUA_PROJECT_DIR NAMES SConstruct
    PATHS 
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH
)

MARK_AS_ADVANCED(XLUA_PROJECT_DIR)

if (NOT LUA_VERSION)
    set(LUA_VERSION "5.4.1")
endif()

#begin lua-rapidjson
set (RAPIDJSON_SRC 
    lua-rapidjson/source/Document.cpp
	lua-rapidjson/source/rapidjson.cpp
	lua-rapidjson/source/Schema.cpp
    lua-rapidjson/source/values.cpp
	)
set_property(
	SOURCE ${RAPIDJSON_SRC}
	APPEND
	PROPERTY COMPILE_DEFINITIONS
	LUA_LIB
)
list(APPEND THIRDPART_INC  lua-rapidjson/include)
set (THIRDPART_SRC ${THIRDPART_SRC} ${RAPIDJSON_SRC})
#end lua-rapidjson

#begin lua-profobuf
set (LPB_SRC 
	lua-protobuf/pb.c
)
set_property(
	SOURCE ${LPB_SRC}
	APPEND
	PROPERTY COMPILE_DEFINITIONS
	LUA_LIB
)
list(APPEND THIRDPART_INC lua-protobuf)
set (THIRDPART_SRC ${THIRDPART_SRC} ${LPB_SRC})
#end lua-protobuf

#begin lua-mongo
set (RAPIDJSON_SRC 
    lua-mongo/main.c
	)
set_property(
	SOURCE ${RAPIDJSON_SRC}
	APPEND
	PROPERTY COMPILE_DEFINITIONS
	LUA_LIB
)
list(APPEND THIRDPART_INC  lua-mongo)
set (THIRDPART_SRC ${THIRDPART_SRC} ${RAPIDJSON_SRC})
#end lua-mongo

set(LUA_SRC_PATH lua-${LUA_VERSION})

set ( LUA_IDSIZE 120 CACHE STRING "gives the maximum size for the description of the source." )

configure_file ( ${LUA_SRC_PATH}/luaconf.h.in ${CMAKE_CURRENT_BINARY_DIR}/luaconf.h )

include_directories(
    ${CMAKE_SOURCE_DIR}
	${LUA_SRC_PATH}
	${CMAKE_CURRENT_BINARY_DIR}
	${THIRDPART_INC}
)

aux_source_directory(${LUA_SRC_PATH} LUA_CORE)
list(REMOVE_ITEM LUA_CORE ${LUA_SRC_PATH}/lua.c ${LUA_SRC_PATH}/luac.c)
set ( LUA_LIB )

set ( XLUA_CORE
    i64lib.c
    xlua.c
)

list (APPEND XLUA_CORE memory_leak_checker.c)

macro(source_group_by_dir proj_dir source_files)
    if(MSVC)
        get_filename_component(sgbd_cur_dir ${proj_dir} ABSOLUTE)
        foreach(sgbd_file ${${source_files}})
			get_filename_component(sgbd_abs_file ${sgbd_file} ABSOLUTE)
            file(RELATIVE_PATH sgbd_fpath ${sgbd_cur_dir} ${sgbd_abs_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_CORE)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} LUA_LIB)
source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} XLUA_CORE)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Switch")
    add_library(xlua STATIC
        ${LUA_CORE}
        ${LUA_LIB}
        ${XLUA_CORE}
        ${THIRDPART_SRC}
    )
    target_compile_options(xlua PRIVATE -m64 -mcpu=cortex-a57+fp+simd+crypto+crc -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -fms-extensions)
else ( )
    add_library(xlua SHARED
        ${LUA_CORE}
        ${LUA_LIB}
        ${XLUA_CORE}
        ${THIRDPART_SRC}
    )
endif ( )

if ( WIN32 AND NOT CYGWIN )
    target_compile_definitions (xlua PRIVATE LUA_BUILD_AS_DLL)
endif ( )

if (LUAC_COMPATIBLE_FORMAT)
    target_compile_definitions (xlua PRIVATE LUAC_COMPATIBLE_FORMAT)
endif ()

if(UINT_ESPECIALLY)
    ADD_DEFINITIONS(-DUINT_ESPECIALLY)
endif()

if ( WIN32 AND NOT CYGWIN )
    target_link_libraries(xlua
        ws2_32
        ${THIRDPART_LIB} 
    )
else ( )
    target_link_libraries(xlua
        m
        ${THIRDPART_LIB} 
    )
endif ( )

cmake_minimum_required(VERSION 2.8)

PROJECT("problem_solver")

# GENERAL SETTINGS

include(CheckCXXCompilerFlag)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")


# FUNCTIONS AND MACROS

# wrapper for add_library function that allows adding custom steps to all targets
macro(add_library name)
   _add_library (${name} ${ARGN})
endmacro(add_library)

# wrapper for add_executable function that allows adding custom steps to all targets
macro(add_executable name)
   _add_executable (${name} ${ARGN})
endmacro(add_executable)

function(add_compiler_flag_if_supported flag name)
  check_cxx_compiler_flag (${flag} ${name})
  if (${name})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
  endif ()
endfunction()

function(add_option name default_value description)
    #  message (${name} ${default_value} ${description})
    set (${name} ${default_value} CACHE STRING "${description}")
    add_definitions (-D${name}=${${name}})
endfunction()

function(add_bool_option name default_value description)
    set (${name} ${default_value} CACHE BOOL "${description}")
    if (${name})
        add_definitions (-D${name})
    endif()
endfunction()

function (copy_files dest_dir src_prefix)
  foreach(file ${ARGN})
    set(dest_name "${CMAKE_BINARY_DIR}/${dest_dir}/${file}")
    set(source_name "${src_prefix}/${file}")
    add_custom_command (OUTPUT ${dest_name}
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_name}" "${dest_name}"
      DEPENDS
      "${source_name}"
      )
    list(APPEND destfiles "${dest_name}")
  endforeach ()
  string(REPLACE "/" "_" rulename "${dest_name}")
  add_custom_target(copy_files_${rulename} ALL DEPENDS ${destfiles})
endfunction ()

#use this to copy and rename a single file
function (copy_and_rename dest_dir src_prefix src_filename dest_filename)
    set(dest_name "${CMAKE_BINARY_DIR}/${dest_dir}/${dest_filename}")
    set(source_name "${src_prefix}/${src_filename}")
    add_custom_command (OUTPUT ${dest_name}
      COMMAND ${CMAKE_COMMAND} -E copy "${source_name}" "${dest_name}"
      DEPENDS
      "${source_name}"
      )

    string(REPLACE "/" "_" rulename "${dest_name}")
    add_custom_target(copy_and_rename_${rulename} ALL DEPENDS ${dest_name})
endfunction ()


#COMPILER FLAGS

add_compiler_flag_if_supported(-Wall wall)
add_compiler_flag_if_supported(-Werror werror)

add_compiler_flag_if_supported(-Werror=write-strings wwrite_strings)
add_compiler_flag_if_supported(-Werror=type-limits type_limits)
add_compiler_flag_if_supported(-Werror=empty-body empty_body)
add_compiler_flag_if_supported(-Wno-error=deprecated-declarations wdeprecated)


# USED PACKAGES

find_package(Threads)
find_package(Boost COMPONENTS program_options thread regex system filesystem serialization iostreams)

set (MONGODB_LIBRARIES "-NOTFOUND")
find_library(MONGODB_LIBRARIES mongoclient
            PATHS /usr/lib /usr/lib64 /usr/lib/mongo)

find_path(MONGODB_INCLUDE_DIR
            NAMES mongo/client/dbclient.h
            PATHS /usr/include/mongo/ /usr/local/include/mongo/)


# CUSTOM SETTINGS

add_bool_option(TEST_BOOL_OPTION OFF "Just a test option")

# INCLUDE DIRECTORIES

include_directories(
    # project directories
    utils/include
    objects/include
    datalayer/include
    datalayer/include/mongodb/
    system/include
    server/include

    # external directories
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${MONGODB_INCLUDE_DIR}
)

# LIBS AND EXECUTABLES


# contains generic reusable code
add_library(utils STATIC
    utils/src/utils.cpp
)
target_link_libraries(utils
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)


# contains different datalayer implementations
add_library(datalayer STATIC
    datalayer/src/mongodb/mongodbdatalayer.cpp
)
target_link_libraries(datalayer
    utils
    ${MONGODB_LIBRARIES}
)


# contains system related code
add_library(system STATIC
    system/src/solvingmachine.cpp
    system/src/systemmanager.cpp
)
target_link_libraries(system
    datalayer
)


# JSON server interface for the system
add_executable(solvingserver
    server/src/solvingserver.cpp
    server/src/remotejsonmanager.cpp
    server/src/jsonserialization.cpp
    )
target_link_libraries (solvingserver
    system
    )
set_target_properties (solvingserver
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/solvingserver)



# FILE COPYING

#copy_files(datacacheserver/conf ${CMAKE_SOURCE_DIR}/datacacheserver/conf datacacheserver_deploy.xml datacacheserver_deploy_backup.xml)

#    copy_and_rename(. ${CMAKE_SOURCE_DIR}/ logger.conf.trace logger.conf)
#    copy_and_rename(datacacheserver/bin ${CMAKE_SOURCE_DIR}/ logger.conf.trace logger.conf)
#ELSE()
#    copy_and_rename(. ${CMAKE_SOURCE_DIR}/ logger.conf.error logger.conf)
#    copy_and_rename(datacacheserver/bin ${CMAKE_SOURCE_DIR}/ logger.conf.trace logger.conf)
#ENDIF()

#copy_files(datacacheserver/bin ${CMAKE_SOURCE_DIR}/datacacheserver/conf/ datacacheserver.ini.sample datacacheserver.ini.preprod datacacheserver.ice.conf)
#copy_files(server/bin ${CMAKE_SOURCE_DIR}/datacacheclient/conf/ datacacheclient.ice.conf)

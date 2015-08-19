include(FindPkgConfig)

pkg_check_modules(HNSERVER_PKG hnode-server-1.0)

if (HNSERVER_PKG_FOUND)
    find_path(HNSERVER_INCLUDE_DIR  NAMES hnode-srvobj.h PATH_SUFFIXES hnode-1.0
       PATHS
       ${HNSERVER_PKG_INCLUDE_DIRS}
       /usr/include/hnode-1.0
       /usr/include
       /usr/local/include/hnode-1.0
       /usr/local/include
    )

    find_library(HNSERVER_LIBRARIES NAMES hnode_server
       PATHS
       ${HNSERVER_PKG_LIBRARY_DIRS}
       /usr/lib
       /usr/local/lib
    )

else (HNSERVER_PKG_FOUND)
    # Find Glib even if pkg-config is not working (eg. cross compiling to Windows)
    find_library(HNSERVER_LIBRARIES NAMES hnode_server)

    string (REGEX REPLACE "/[^/]*$" "" HNSERVER_LIBRARIES_DIR ${HNSERVER_LIBRARIES})

    find_path(HNSERVER_INCLUDE_DIR NAMES hnode-srvobj.h PATH_SUFFIXES hnode-1.0)

endif (HNSERVER_PKG_FOUND)

if (HNSERVER_INCLUDE_DIR AND HNSERVER_LIBRARIES)
    set(HNSERVER_INCLUDE_DIRS ${HNSERVER_INCLUDE_DIR})
endif (HNSERVER_INCLUDE_DIR AND HNSERVER_LIBRARIES)

if(HNSERVER_INCLUDE_DIRS AND HNSERVER_LIBRARIES)
   set(HNSERVER_FOUND TRUE CACHE INTERNAL "hnode-server-1.0 found")
   message(STATUS "Found hnode-server-1.0: ${HNSERVER_INCLUDE_DIR}, ${HNSERVER_LIBRARIES}")
else(HNSERVER_INCLUDE_DIRS AND HNSERVER_LIBRARIES)
   set(HNSERVER_FOUND FALSE CACHE INTERNAL "hnode_server found")
   message(STATUS "hnode-server-1.0 not found.")
endif(HNSERVER_INCLUDE_DIRS AND HNSERVER_LIBRARIES)

mark_as_advanced(HNSERVER_INCLUDE_DIR HNSERVER_INCLUDE_DIRS HNSERVER_LIBRARIES)



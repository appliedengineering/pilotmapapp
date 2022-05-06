message("<Findmsgpack.cmake>")

#get_install_dir(MSGPACK_INSTALL_DIR "Install directory of MSGPACK")

set(RUNTIME_CMAKE_PATH "O:/MSGPACK/lib/cmake/msgpackc-cxx/msgpack-config.cmake")

if(EXISTS "${RUNTIME_CMAKE_PATH}")
  message("RUNTIME_CMAKE_PATH = ${RUNTIME_CMAKE_PATH}")
  include("${RUNTIME_CMAKE_PATH}")
else()
  message("MSGPACK Install Dir - $ENV{MSGPACK_INSTALL_DIR}")
  message(FATAL_ERROR "module ${RUNTIME_CMAKE_PATH} does not exist.")
endif()

message("</Findmsgpack.cmake>")
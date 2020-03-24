# Install script for directory: /mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SEAL-3.4/seal" TYPE FILE FILES
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/batchencoder.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/biguint.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/ciphertext.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/ckks.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/modulus.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/context.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/decryptor.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/intencoder.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/encryptionparams.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/encryptor.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/evaluator.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/galoiskeys.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/intarray.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/keygenerator.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/kswitchkeys.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/memorymanager.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/modulus.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/plaintext.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/publickey.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/randomgen.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/randomtostd.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/relinkeys.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/seal.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/secretkey.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/serialization.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/smallmodulus.h"
    "/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/valcheck.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/Users/snwag/Desktop/bootcamp/native/src/seal/util/cmake_install.cmake")

endif()


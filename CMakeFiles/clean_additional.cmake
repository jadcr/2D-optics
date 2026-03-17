# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "2D_Optics_autogen"
  "CMakeFiles\\2D_Optics_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\2D_Optics_autogen.dir\\ParseCache.txt"
  )
endif()

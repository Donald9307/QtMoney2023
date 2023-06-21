# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Money_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Money_autogen.dir/ParseCache.txt"
  "Money_autogen"
  )
endif()

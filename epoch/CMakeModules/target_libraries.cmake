if(__TARGET_LIBRARIES_INCLUDED)
  return()
endif()
set(__TARGET_LIBRARIES_INCLUDED TRUE)

# Copies the given dynamic libraries output to the target output.
#
# TARGET_DYNAMIC_LIBRARIES(_target <target> 
#                          ARGN [item1 [item2 [...]]]
#                          )
# 
# _target: the target to copy to.
# ARGN: a list of targets that _target depends on.
#
macro(TARGET_DYNAMIC_LIBRARIES _target)
  foreach(_dependency ${ARGN})
    add_custom_command(
      TARGET ${_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:${_dependency}>"
      "$<TARGET_FILE_DIR:${_target}>"
    )
  endforeach()
endmacro()

# Copies the boost dynamic libraries output to the target output.
#
# TARGET_BOOST_DYNAMIC_LIBRARIES(_target <target> 
#                               )
# 
# _target: the target that depends on boost.
#
macro(TARGET_BOOST_DYNAMIC_LIBRARIES _target)
  file(GLOB _boost_dlls "${Boost_LIBRARY_DIRS}/*.dll")
  foreach(_dll ${_boost_dlls})
    add_custom_command(
      TARGET ${_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${_dll}"
      "$<TARGET_FILE_DIR:${_target}>"
    )
  endforeach()
endmacro()

# Copies the Qt *icu dlls to the target output.
#
# TARGET_QT_DYNAMIC_LIBRARIES(_target <target>)
# 
# _target: the target that depends on Qt.
#
macro(TARGET_QT_DYNAMIC_LIBRARIES _target)
file(GLOB icu_dlls "$ENV{QTDIR}/bin/icu*.dll")
foreach(icu_dll ${icu_dlls})
  add_custom_command(
    TARGET ${_target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${icu_dll}"
    "$<TARGET_FILE_DIR:${_target}>"
  )
endforeach()
endmacro()
include(CMakeFindDependencyMacro)

if(FALSE)
    find_dependency(Jasper)
endif()

if(TRUE)
    find_dependency(JPEG)
endif()
if(TRUE)
    find_dependency(ZLIB)
endif()

if(true)
    if(TRUE)
        find_dependency(LCMS2)
    elseif(false)
        find_dependency(LCMS)
    endif()
endif()

if(OFF)
    find_dependency(LibXml2)
    find_dependency(Threads)
endif()

if(ON)
    find_dependency(OpenMP)
endif()



####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was librawConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include("${CMAKE_CURRENT_LIST_DIR}/librawTargets.cmake")
check_required_components("libraw")

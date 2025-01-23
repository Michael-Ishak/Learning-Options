include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(Learning_Options_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(Learning_Options_setup_options)
  option(Learning_Options_ENABLE_HARDENING "Enable hardening" ON)
  option(Learning_Options_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    Learning_Options_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    Learning_Options_ENABLE_HARDENING
    OFF)

  Learning_Options_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR Learning_Options_PACKAGING_MAINTAINER_MODE)
    option(Learning_Options_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(Learning_Options_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(Learning_Options_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Learning_Options_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Learning_Options_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Learning_Options_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(Learning_Options_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(Learning_Options_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Learning_Options_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(Learning_Options_ENABLE_IPO "Enable IPO/LTO" ON)
    option(Learning_Options_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(Learning_Options_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Learning_Options_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(Learning_Options_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(Learning_Options_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Learning_Options_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Learning_Options_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Learning_Options_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(Learning_Options_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(Learning_Options_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Learning_Options_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      Learning_Options_ENABLE_IPO
      Learning_Options_WARNINGS_AS_ERRORS
      Learning_Options_ENABLE_USER_LINKER
      Learning_Options_ENABLE_SANITIZER_ADDRESS
      Learning_Options_ENABLE_SANITIZER_LEAK
      Learning_Options_ENABLE_SANITIZER_UNDEFINED
      Learning_Options_ENABLE_SANITIZER_THREAD
      Learning_Options_ENABLE_SANITIZER_MEMORY
      Learning_Options_ENABLE_UNITY_BUILD
      Learning_Options_ENABLE_CLANG_TIDY
      Learning_Options_ENABLE_CPPCHECK
      Learning_Options_ENABLE_COVERAGE
      Learning_Options_ENABLE_PCH
      Learning_Options_ENABLE_CACHE)
  endif()

  Learning_Options_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (Learning_Options_ENABLE_SANITIZER_ADDRESS OR Learning_Options_ENABLE_SANITIZER_THREAD OR Learning_Options_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(Learning_Options_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(Learning_Options_global_options)
  if(Learning_Options_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    Learning_Options_enable_ipo()
  endif()

  Learning_Options_supports_sanitizers()

  if(Learning_Options_ENABLE_HARDENING AND Learning_Options_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Learning_Options_ENABLE_SANITIZER_UNDEFINED
       OR Learning_Options_ENABLE_SANITIZER_ADDRESS
       OR Learning_Options_ENABLE_SANITIZER_THREAD
       OR Learning_Options_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${Learning_Options_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${Learning_Options_ENABLE_SANITIZER_UNDEFINED}")
    Learning_Options_enable_hardening(Learning_Options_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(Learning_Options_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(Learning_Options_warnings INTERFACE)
  add_library(Learning_Options_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  Learning_Options_set_project_warnings(
    Learning_Options_warnings
    ${Learning_Options_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(Learning_Options_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    Learning_Options_configure_linker(Learning_Options_options)
  endif()

  include(cmake/Sanitizers.cmake)
  Learning_Options_enable_sanitizers(
    Learning_Options_options
    ${Learning_Options_ENABLE_SANITIZER_ADDRESS}
    ${Learning_Options_ENABLE_SANITIZER_LEAK}
    ${Learning_Options_ENABLE_SANITIZER_UNDEFINED}
    ${Learning_Options_ENABLE_SANITIZER_THREAD}
    ${Learning_Options_ENABLE_SANITIZER_MEMORY})

  set_target_properties(Learning_Options_options PROPERTIES UNITY_BUILD ${Learning_Options_ENABLE_UNITY_BUILD})

  if(Learning_Options_ENABLE_PCH)
    target_precompile_headers(
      Learning_Options_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(Learning_Options_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    Learning_Options_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(Learning_Options_ENABLE_CLANG_TIDY)
    Learning_Options_enable_clang_tidy(Learning_Options_options ${Learning_Options_WARNINGS_AS_ERRORS})
  endif()

  if(Learning_Options_ENABLE_CPPCHECK)
    Learning_Options_enable_cppcheck(${Learning_Options_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(Learning_Options_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    Learning_Options_enable_coverage(Learning_Options_options)
  endif()

  if(Learning_Options_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(Learning_Options_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(Learning_Options_ENABLE_HARDENING AND NOT Learning_Options_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Learning_Options_ENABLE_SANITIZER_UNDEFINED
       OR Learning_Options_ENABLE_SANITIZER_ADDRESS
       OR Learning_Options_ENABLE_SANITIZER_THREAD
       OR Learning_Options_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    Learning_Options_enable_hardening(Learning_Options_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()

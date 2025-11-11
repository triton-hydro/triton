set(TRITON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TRITON_BUILD_DIR ${CMAKE_BINARY_DIR})

if(NOT DEFINED MACHINE)
  if(DEFINED ENV{TRITON_MACHINE})
    set(MACHINE $ENV{TRITON_MACHINE})
  else()
    set(MACHINE ${CMAKE_HOST_SYSTEM_NAME})
  endif()
endif()

if(DEFINED COMPILER)
  set(COMPILER_NICKNAME ${COMPILER})
  unset(COMPILER CACHE)
else()
  set(COMPILER_NICKNAME "default")
endif()

if(NOT DEFINED BACKEND)
  set(BACKEND "default")
endif()

macro (set_environment)

set(machinefile_path "")

if(EXISTS "${MACHINE}")
  set(machinefile_path "${MACHINE}")

else()

  if("${MACHINE}" STREQUAL "${CMAKE_HOST_SYSTEM_NAME}")
    set(MACHINES_DIR "${TRITON_SOURCE_DIR}/cmake/machines")
    
    # Get all sub-directories
    file(GLOB SUBDIRS "${MACHINES_DIR}/*")
    
    set(MATCHED_MACHINE "")
    set(HOST_FQDN "")
    
    # Use Python to get FQDN
    execute_process(
      COMMAND python3 -c "import socket; print(socket.getfqdn())"
      OUTPUT_VARIABLE HOST_FQDN
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Detected FQDN: ${HOST_FQDN}")
    
	# Iterate through subdirectories
	set(MATCHED_MACHINES "")  # List of all matched machine names

	foreach(subdir ${SUBDIRS})
	  if(IS_DIRECTORY "${subdir}")
        get_filename_component(LAST_DIR_NAME "${subdir}" NAME)
		set(machine_cmake "${subdir}/__machine__.cmake")

		if(EXISTS "${machine_cmake}")
		  unset(FQDN_REGEX)
		  unset(ENV_VALUES)

		  # Include the CMake file so that FQDN_REGEX and MACHINE_NAME are set
		  include("${machine_cmake}")

		  if(DEFINED FQDN_REGEX)
			string(REGEX MATCH "${FQDN_REGEX}" _match "${HOST_FQDN}")

			if(_match)
		      if(DEFINED ENV_VALUES)
				set(ALL_ENV_MATCH TRUE)  # assume all match until proven otherwise

				# Split multiple key:value pairs by comma
				string(REPLACE "," ";" ENV_PAIRS "${ENV_VALUES}")

				foreach(pair ${ENV_PAIRS})
					# Split into key and expected value
					string(REPLACE ":" ";" ENV_PARTS "${pair}")
					list(GET ENV_PARTS 0 ENV_KEY)
					list(GET ENV_PARTS 1 EXPECTED_VALUE)

					# Get actual environment variable value
					set(ACTUAL_VALUE $ENV{${ENV_KEY}})
					DEBUG_MESSAGE("ENV_KEY = ${ENV_KEY}, EXPECTED_VALUE =
						${EXPECTED_VALUE}, ACTUAL_VALUE = ${ACTUAL_VALUE}")

					if(DEFINED ENV{${ENV_KEY}})
						if(NOT ACTUAL_VALUE STREQUAL EXPECTED_VALUE)
							set(ALL_ENV_MATCH FALSE)
						endif()
					else()
						set(ALL_ENV_MATCH FALSE)
					endif()
				endforeach()

				# If all key:value pairs matched, record this machine
				if(ALL_ENV_MATCH)
                    list(APPEND MATCHED_MACHINES "${LAST_DIR_NAME}")
                    message(STATUS "Matched ENV: ${ENV_VALUES}")
				endif()
              else()
			    list(APPEND MATCHED_MACHINES "${LAST_DIR_NAME}")
			    message(STATUS "Matched regex: ${FQDN_REGEX}")
              endif()
			endif()
		  else()
			message(WARNING " - FQDN_REGEX not defined in ${machine_cmake}")
		  endif()
		endif()
	  endif()
	endforeach()

	# Handle number of matches
	list(LENGTH MATCHED_MACHINES MATCH_COUNT)

	if(MATCH_COUNT EQUAL 1)
	  list(GET MATCHED_MACHINES 0 MACHINE)
	  message(STATUS "Matched machine: ${MACHINE}")
	elseif(MATCH_COUNT GREATER 1)
	  message(FATAL_ERROR
	    "Multiple matches found (${MATCH_COUNT}): ${MATCHED_MACHINES}.
	     Use -DMACHINE=<machine name or path> cmake command-line argument"
	  )
	endif()
  endif()

  file(GLOB_RECURSE ALL_FILES "${TRITON_SOURCE_DIR}/cmake/machines/${MACHINE}/*")
  set(machinefile_name "${COMPILER_NICKNAME}_${BACKEND}")
  
  DEBUG_MESSAGE("machinefile_name = ${machinefile_name}")

  foreach(f ${ALL_FILES})
    # Get the filename without the directory
    get_filename_component(fname "${f}" NAME_WLE)
  
    # Search for a machine file that matches fname
    if("${fname}" STREQUAL "${machinefile_name}")
      set(machinefile_path "${f}")
      #message(STATUS "fname : ${fname}")
      break()
    endif()
  endforeach()
endif()

if(NOT EXISTS "${machinefile_path}")
  message(FATAL_ERROR "No matching machine file: ${machinefile_path}")
else()
  get_filename_component(FILE_EXT "${machinefile_path}" EXT)
  set(ENVFILE "triton_env${FILE_EXT}")
  configure_file("${machinefile_path}" "${TRITON_BUILD_DIR}/${ENVFILE}" COPYONLY)
  message(STATUS "machine file: ${machinefile_path}")
endif()

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	run_win_command("${machinefile_path} && set" ENV_OUTPUT)
else()
	run_bash_command("source ${machinefile_path} && env" ENV_OUTPUT)
endif()

string(REPLACE "\n" ";" lines ${ENV_OUTPUT})

set(_REQUIRED_ENVS
  COMPILER
  BACKEND
  RUN_COMMAND
)

set(_TRITON_BACKENDS
  CUDA
  HIP
  SYCL
  OPENMP
  OPENMPTARGET
  THREADS
  SERIAL
  default
)

# set env. variables
foreach(line ${lines})
  string(REGEX MATCH "([A-Za-z_][A-Za-z0-9_]*)=(.*)" ENV_LINE ${line})
  if (ENV_LINE)
    set(ENV_VAR "${CMAKE_MATCH_1}")
    set(ENV_VAL "${CMAKE_MATCH_2}")
  
    string(REGEX MATCH "^TRITON_(.*)" _matched "${ENV_VAR}")
	if(_matched)
      set(SUFFIX "${CMAKE_MATCH_1}")
      if(NOT DEFINED "${SUFFIX}"
        OR ("${SUFFIX}" STREQUAL "BACKEND" AND "${BACKEND}" STREQUAL "default")
      )
        if(DEFINED ENV{TRITON_${SUFFIX}})
          set(${SUFFIX} $ENV{TRITON_${SUFFIX}})
        else()
          set(${SUFFIX} ${ENV_VAL})
        endif()
      endif()
    else()
      set(ENV{${ENV_VAR}} ${ENV_VAL})
    endif()
  endif()

endforeach()

set(_MISSING_ENVS "")

foreach(VAR IN LISTS _REQUIRED_ENVS)
  if(NOT DEFINED ${VAR})
    list(APPEND _MISSING_ENVS "${VAR}")
  endif()
endforeach()

if(_MISSING_ENVS)
  message(FATAL_ERROR "The following triton variables are not defined:\n  ${_MISSING_ENVS}")
endif()

find_program(CMAKE_CXX_COMPILER "${COMPILER}")
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${COMPILER_FLAGS_APPEND}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${LINKER_FLAGS_APPEND}")

if (DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRITON_DEBUG")
endif()

message(STATUS "TRITON_MACHINE=${MACHINE}")
message(STATUS "TRITON_COMPILER=${COMPILER}")

list(FIND _TRITON_BACKENDS "${BACKEND}" VAR_INDEX)
if(VAR_INDEX EQUAL -1)
  message(FATAL_ERROR "Not supported BACKEND: ${BACKEND}")
else()
  message(STATUS "TRITON_BACKEND=${BACKEND}")
endif()

set(_Kokkos_Enable_Var "Kokkos_ENABLE_${BACKEND}")
set(${_Kokkos_Enable_Var} ON CACHE BOOL "Enable Kokkos backend: ${BACKEND}")

if("${BACKEND}" STREQUAL "CUDA")
  set(Kokkos_ENABLE_CUDA_CONSTEXPR ON CACHE BOOL "Enable CUDA CONSTEXPR")
endif()

if (ARCH)
  set(_Kokkos_Arch_Var "Kokkos_ARCH_${ARCH}")
  set(${_Kokkos_Arch_Var} ON CACHE BOOL "Set Kokkos archtecture: ${ARCH}")
  message(STATUS "TRITON_ARCH=${ARCH}")
endif()

message(STATUS "TRITON_RUN_COMMAND=${RUN_COMMAND}")

endmacro()

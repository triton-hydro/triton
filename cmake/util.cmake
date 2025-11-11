macro(run_bash_command command outvar)
  execute_process(
    COMMAND bash -c "${command}"
    OUTPUT_VARIABLE ${outvar}     
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endmacro()

macro(run_win_command command outvar)
  execute_process(
    COMMAND cmd.exe /c "${command}"
    OUTPUT_VARIABLE ${outvar}     
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endmacro()

function(DEBUG_MESSAGE msg)
  if(DEBUG)
    message(STATUS "${msg}")
  endif()
endfunction()

macro(add_build_and_run_scripts)

  cmake_host_system_information(RESULT N_PHYSICAL_CORES QUERY NUMBER_OF_PHYSICAL_CORES)

  # create a build script         
  set(_BuildScript ${CMAKE_BINARY_DIR}/triton_build.sh)
  file(WRITE ${_BuildScript}  "#!/usr/bin/env bash\n\n")
  file(APPEND ${_BuildScript} "source ./${ENVFILE}\n\n")
  # On Windows, you can omit -j for Visual Studio, or use -j<num_cores> for Ninja/Mak
  file(APPEND ${_BuildScript} "cmake --build . -j ${N_PHYSICAL_CORES}\n\n")
  execute_process(COMMAND chmod +x ${_BuildScript})

  if (ENSEMBLE_BUILD)
    # create an ensemble run script           
    set(_RunScript ${CMAKE_BINARY_DIR}/triton_ensrun.sh)
    file(WRITE ${_RunScript}  "#!/usr/bin/env bash\n\n")
    file(APPEND ${_RunScript} "source ./${ENVFILE}\n\n")
    file(APPEND ${_RunScript} "ENSCFG_FILE=\${1:-./ensembles_ensify.yaml}\n")
    file(APPEND ${_RunScript} "MPI_CMD=\${2:-${RUN_COMMAND}}\n")
    file(APPEND ${_RunScript} "\${MPI_CMD} ./${TRITON_EXECUTABLE} \${ENSCFG_FILE}\n\n")
    execute_process(COMMAND chmod +x ${_RunScript})

    configure_file(${CMAKE_SOURCE_DIR}/test/ensembles_ensify.yaml
      ${CMAKE_BINARY_DIR}/ensembles_ensify.yaml COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/test/paraboloid_ens1.cfg
      ${CMAKE_BINARY_DIR}/paraboloid_ens1.cfg COPYONLY)
    configure_file(${CMAKE_SOURCE_DIR}/test/paraboloid_ens2.cfg
      ${CMAKE_BINARY_DIR}/paraboloid_ens2.cfg COPYONLY)

  else()

    # create a run script           
    set(_RunScript ${CMAKE_BINARY_DIR}/triton_run.sh)
    file(WRITE ${_RunScript}  "#!/usr/bin/env bash\n\n")
    file(APPEND ${_RunScript} "source ./${ENVFILE}\n\n")
    file(APPEND ${_RunScript} "CFG_FILE=\${1:-./input/paraboloid/paraboloid.cfg}\n")
    file(APPEND ${_RunScript} "MPI_CMD=\${2:-${RUN_COMMAND}}\n")
    file(APPEND ${_RunScript} "\${MPI_CMD} ./${TRITON_EXECUTABLE} \${CFG_FILE}\n\n")
    execute_process(COMMAND chmod +x ${_RunScript})
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_SOURCE_DIR}/input"
            "${CMAKE_BINARY_DIR}/input"
  )

endmacro()

macro(add_test_script)
  # create a ctest script         
  set(_CtestScript ${CMAKE_BINARY_DIR}/triton_ctest.sh)
  file(WRITE ${_CtestScript}  "#!/usr/bin/env bash\n\n")
  file(APPEND ${_CtestScript} "source ./${ENVFILE}\n\n")
  file(APPEND ${_CtestScript} "ctest $*\n\n")
  execute_process(COMMAND chmod +x ${_CtestScript})

  configure_file(${CMAKE_SOURCE_DIR}/test/reference/compare_runs_simple.py
      ${CMAKE_BINARY_DIR}/compare_runs_simple.py COPYONLY)

endmacro()

macro(add_clean_script)
  set(_CleanScript ${CMAKE_BINARY_DIR}/triton_clean.sh)
  file(WRITE ${_CleanScript}  "#!/usr/bin/env bash\n\n")
  file(APPEND ${_CleanScript} "rm -rf \\\n")
  file(APPEND ${_CleanScript} "    cid \\\n")
  file(APPEND ${_CleanScript} "    CMakeFiles \\\n")
  file(APPEND ${_CleanScript} "    cmake_packages \\\n")
  file(APPEND ${_CleanScript} "    git-state.txt \\\n")
  file(APPEND ${_CleanScript} "    Makefile \\\n")
  file(APPEND ${_CleanScript} "    CTestTestfile.cmake \\\n")
  file(APPEND ${_CleanScript} "    test \\\n")
  file(APPEND ${_CleanScript} "    tools \\\n")
  file(APPEND ${_CleanScript} "    Testing \\\n")
  file(APPEND ${_CleanScript} "    triton_* \\\n")
  file(APPEND ${_CleanScript} "    ${TRITON_EXECUTABLE} \\\n")
  file(APPEND ${_CleanScript} "    CMakeCache.txt \\\n")
  file(APPEND ${_CleanScript} "    cmake_install.cmake \\\n")
  file(APPEND ${_CleanScript} "    compare_runs_simple.py \\\n")
  file(APPEND ${_CleanScript} "    output_allatoona \\\n")
  file(APPEND ${_CleanScript} "    output_circular_dambreak \\\n")
  file(APPEND ${_CleanScript} "    output_paraboloid \\\n")
  file(APPEND ${_CleanScript} "    triton_ensrun.sh \\\n")
  file(APPEND ${_CleanScript} "    ensembles_ensify.yaml \\\n")
  file(APPEND ${_CleanScript} "    paraboloid_ens1.cfg \\\n")
  file(APPEND ${_CleanScript} "    paraboloid_ens2.cfg \\\n")
  file(APPEND ${_CleanScript} "    output_ens1 \\\n")
  file(APPEND ${_CleanScript} "    output_ens2 \\\n")
  file(APPEND ${_CleanScript} "    ens1.err \\\n")
  file(APPEND ${_CleanScript} "    ens1.out \\\n")
  file(APPEND ${_CleanScript} "    ens2.err \\\n")
  file(APPEND ${_CleanScript} "    ens2.out \\\n")
  file(APPEND ${_CleanScript} "    external \\\n")
  file(APPEND ${_CleanScript} "    input \\\n")
  file(APPEND ${_CleanScript} "    output")
  execute_process(COMMAND chmod +x ${_CleanScript})

endmacro()

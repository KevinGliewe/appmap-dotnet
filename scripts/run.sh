#!/bin/bash

find_file() {
    local name="$1"
    shift
    for d in "$@"; do
        [ -f $d/$name ] && echo $d && return
    done
    return 1
}

BINPATH=$(find_file libappmap-instrumentation.so $(dirname $0) $(dirname $0)/../out)

export CORECLR_ENABLE_PROFILING=${CORECLR_ENABLE_PROFILING:-1}
export CORECLR_PROFILER=${CORECLR_PROFILER:-{324F817A-7420-4E6D-B3C1-143FBED6D855}}
export CORECLR_PROFILER_PATH_64=${CORECLR_PROFILER_PATH_64:-$BINPATH/libInstrumentationEngine.so}
export CORECLR_PROFILER_PATH=${CORECLR_PROFILER_PATH:-$BINPATH/libInstrumentationEngine.so}
export MicrosoftInstrumentationEngine_LogLevel=${MicrosoftInstrumentationEngine_LogLevel:-Errors}
export MicrosoftInstrumentationEngine_DisableCodeSignatureValidation=${MicrosoftInstrumentationEngine_DisableCodeSignatureValidation:-1}
export MicrosoftInstrumentationEngine_ConfigPath64_TestMethod=${MicrosoftInstrumentationEngine_ConfigPath64_TestMethod:-$BINPATH/ProductionBreakpoints_x64.config}

exec "$@"
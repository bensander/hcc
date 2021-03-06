macro(ensure_llvm_is_present dest_dir name)

if(EXISTS "${dest_dir}/${name}")
    MESSAGE("LLVM already exists.")
else(EXISTS "${dest_dir}/${name}")
    MESSAGE("Downloading LLVM 3.5")
    Find_Package(Wget)
    if(NOT WGET_EXECUTABLE)
        message(FATAL_ERROR "upstream llvm is not present at ${dest_dir}/${name} and wget could not be found")
    else()
        execute_process( COMMAND wget http://llvm.org/releases/3.5.0/llvm-3.5.0.src.tar.xz )
        execute_process( COMMAND tar xf llvm-3.5.0.src.tar.xz )
        execute_process( COMMAND mv llvm-3.5.0.src ${dest_dir}/${name} )
        execute_process( COMMAND rm llvm-3.5.0.src.tar.xz )
    endif()
endif()
endmacro()

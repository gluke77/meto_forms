time /t > build_debug_start.txt
mingw32-make clean
call remake
mingw32-make
time /t > build_debug_end.txt
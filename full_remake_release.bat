time /t > build_release_start.txt
mingw32-make clean
call remake
mingw32-make release
time /t > build_release_end.txt
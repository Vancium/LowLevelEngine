



cmake -S . -B bin -G "Unix Makefiles" -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE
cd bin

make

cd ..




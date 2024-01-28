

cmake -S . -B bin -D CMAKE_BUILD_TYPE=Release -G "Unix Makefiles" 
cd bin

make

cd ..

rm compile_commands.json

ln -s bin/compile_commands.json compile_commands.json

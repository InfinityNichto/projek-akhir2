#!/bin/bash
if [ -d "build" ]; then
  rm -rf ./build
fi

mkdir ./build
cd ./build
cmake ..

if [ ! -f "make-n-run.sh" ]; then
  touch ./make-n-run.sh
  printf "#!/bin/bash\nmake && ./projek-akhir2\n\ncounter=\$(<../build_counter)\n((counter++))\nprintf \"%%s\" \"\$counter\" > ../build_counter\necho -n \"build counter: \"\ncat ../build_counter\necho" > ./make-n-run.sh
  chmod +x ./make-n-run.sh
fi

if [ -f "../compile_commands.json" ]; then
  rm ../compile_commands.json
fi

ln -s ./compile_commands.json ../compile_commands.json


clear

EXECUTABLE_PATH=false;

case $1 in
    "") ;;
    Core)
        EXECUTABLE_PATH=./build/Core/Core
        ;;
    *)
        echo Target no encontrado
        exit
        ;;
esac

mkdir -p build
cd build
cmake ..
cmake --build .

BUILD_STATUS=$?

cd ..

if [ $BUILD_STATUS != 0 ]; then
    exit
fi

$EXECUTABLE_PATH

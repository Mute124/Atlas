@ECHO OFF

::set BASEDIR=%~dp0
::PUSHD %BASEDIR%

::RMDIR /Q /S build

::@ECHO ON

::conan install . --output-folder=build --build=missing
::cd build
::cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=build\conan_toolchain.cmake
::cmake --build . --config=Debug
::cd ../build-scripts
python3 configure.py --release --debug
echo "Generating project"
python3 generate.py

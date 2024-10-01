cmake -D CMAKE_CXX_COMPILER=/usr/bin/g++-11 -S . -B build

cmake --build build -v

cd build && ls && ./testing

if test -f ../sample_input.txt; then
  ls && ./executable < ../sample_input.txt
else
  ls && ./executable
fi

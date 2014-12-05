# Mandelbrot
This script create a picture by using the [Mantelbrot set](http://en.wikipedia.org/wiki/Mandelbrot_set). Source code in C.

# Mac Requirements
- X11: <http://xquartz.macosforge.org/>
- Homebrew
	- `brew install homebrew/versions/gcc49`
	- `brew install imlib2`

# Build
- Basic
	
		( rm -rf build && mkdir -p build && cd build && cmake .. && make VERBOSE=1 && make test ) #cmake
	
	(On **Mac**) use GCC not Clang because Clang doesn't support OpenMP:
	
		( rm -rf build && mkdir -p build && cd build && cmake -DCMAKE_C_COMPILER=/usr/local/bin/gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ .. && make VERBOSE=1 && make test ) #cmake

## Project Links
- [Travis CI Repository](https://travis-ci.org/TheFox/mandelbrot-c)

## License
Copyright (C) 2014 Christian Mayer <http://fox21.at>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

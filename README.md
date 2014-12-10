# Mandelbrot
This script create a picture by using the [Mantelbrot set](http://en.wikipedia.org/wiki/Mandelbrot_set). Source code in C.

## Mac Requirements
- X11: <http://xquartz.macosforge.org/>
- Homebrew
	- `brew install homebrew/versions/gcc49`
	- `brew install imlib2`

## Build
- Basic
	
		( rm -rf build_$(uname -s) && mkdir -p build_$(uname -s) && cd build_$(uname -s) && cmake .. && make VERBOSE=1 && make test ) #cmake
	
	(On **Mac**) use GCC not Clang because Clang doesn't support OpenMP:
	
		( rm -rf build_$(uname -s) && mkdir -p build_$(uname -s) && cd build_$(uname -s) && cmake -DCMAKE_C_COMPILER=/usr/local/bin/gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ .. && make VERBOSE=1 && make test ) #cmake

## Usage
After building you can generate with `./build/bin/mandelbrot` an image:

	./build/bin/mandelbrot I_WIDTH I_HEIGHT DEPTH_MIN DEPTH_MAX MB_WIDTH_MID MB_WIDTH_ZOOM MB_HEIGHT_MID MB_HEIGHT_MAX

Example:

	./build/bin/mandelbrot 1024 1024 1 100 -.5 2 0 2

This will generate an 1024x1024 pixel image with depth 1 to 100. The real part coordinate -.5 and the imaginary part coordinate 0 will be centered to the image.

### Arguments
- `I_WIDTH`: The width of the image.
- `I_HEIGHT`: The height of the image.
- `DEPTH_MIN`: The minimum depth. Default 1.
- `DEPTH_MAX`: The maximum depth.
- `MB_WIDTH_MID`: Real part (CR).
- `MB_WIDTH_ZOOM`: Zoom of CR.
- `MB_HEIGHT_MID`: Imaginary part (CI).
- `MB_HEIGHT_MAX`: Zoom of CI.

## Project Links
- [Travis CI Repository](https://travis-ci.org/TheFox/mandelbrot-c)

## License
Copyright (C) 2014 Christian Mayer <http://fox21.at>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

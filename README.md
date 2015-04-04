# Mandelbrot
This script create a picture by using the [Mantelbrot set](http://en.wikipedia.org/wiki/Mandelbrot_set). Source code in C.

## Mac Requirements
- X11: <http://xquartz.macosforge.org/>
- Homebrew
	- `brew install homebrew/versions/gcc49`
	- `brew install imlib2`

## Build
- Basic
	
		( rm -rf build && mkdir -p build && cd build && cmake .. && make VERBOSE=1 && make test ) #cmake
	
	(On **Mac**) use GCC not Clang because Clang doesn't support OpenMP:
	
		( rm -rf build && mkdir -p build && cd build && cmake -DCMAKE_C_COMPILER=/usr/local/bin/gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ .. && make VERBOSE=1 && make test ) #cmake

### Multithreading Support
- GCC
	- OpenMP: Yes.
	- OpenCL for Mac: No, because the default [GCC doesn't include blocks support](http://stackoverflow.com/questions/4644814/are-objective-c-blocks-supported-by-compilers-on-linux).
- Clang
	- OpenMP: No, because default Clang doesn't include OpenMP support.
	- OpenCL: Yes.

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

## Related Links
- <http://openmp.org/>
- <http://openmp.llvm.org/>
- <http://libcxx.llvm.org/>
- <http://libcxxabi.llvm.org/>
- <http://stackoverflow.com/questions/7001424/opencl-problem-with-double-type>
- <http://stackoverflow.com/questions/4644814/are-objective-c-blocks-supported-by-compilers-on-linux>
- <http://stackoverflow.com/questions/13805059/dirname-in-c-is-the-manual-wrong>
- <http://stackoverflow.com/questions/7031126/switching-between-gcc-and-clang-llvm-using-cmake>
- <http://stackoverflow.com/questions/26791343/using-openmp-with-llvm-clang>
- <http://stackoverflow.com/questions/2998864/how-to-add-a-or-condition-in-ifdef>
- <http://stackoverflow.com/questions/5557164/causes-for-cl-invalid-work-group-size>
- <http://stackoverflow.com/questions/7996537/cl-invalid-work-group-size-error>
- <http://www.cmake.org/Wiki/CMake_Useful_Variables>
- <https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html>
- <http://tersetalk.wordpress.com/2012/04/13/opencl-error-codes/>
- <https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/scalarDataTypes.html>
- <https://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/reservedDataTypes.html>
- <https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/cl_khr_fp64.html>
- <https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clEnqueueNDRangeKernel.html>

## License
Copyright (C) 2014 Christian Mayer <http://fox21.at>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

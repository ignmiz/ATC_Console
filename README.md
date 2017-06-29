# Research Radar ATC Simulator
## Purpose
Research air traffic control simulators are used to simulate high fidelity aircraft trajectories for R&D purposes. Most notable battery of such simulators is created and maintained by Eurocontrol Experimental Centre (EEC). This is an attempt to create my own, local version of such simulator. It is my first major software project ever, so if you're looking for some good software engineering practices, you most likely won't find them here.

## License & Warranty
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Project is released under MIT License. It can be freely used as long as the original creator is attributed. Software provided comes with no warranty whatsoever.

## Installation
Code can be downloaded and built straight away using qmake in Qt Toolkit. Alternatively, you can convert it to CMake and go from there.
The project uses functionalities from two external libraries:
 - [GeographicLib](https://geographiclib.sourceforge.io/) - computations on WGS-84 ellipsoid
 - [Intel Math Kernel Library](https://software.intel.com/en-us/mkl) - linear interpolators

GeographicLib is provided in `./lib/GeographicLib`, but you may want to build it yourself to avoid binary incompatibility. Intel MKL is not provided due large size. After the libraries have been obtained, paths in project files `./ATCConsole.pro`, `./test/ATCConsoleTest.pro` should be updated, eg.:
```
CONFIG(debug): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic_d
CONFIG(release): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic

LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/mkl/lib/intel64_win" -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core
LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/compiler/lib/intel64_win" -llibiomp5md
```


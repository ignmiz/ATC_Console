# Research Radar ATC Simulator
## Purpose
Research air traffic control simulators are used to simulate high fidelity aircraft trajectories for R&D purposes. Most notable battery of such simulators is created and maintained by Eurocontrol Experimental Centre (EEC). This is an attempt to develop my own, local version of such simulator. It is my first major software project ever, so if you're looking for some good software engineering practices, you most likely won't find them here.

## License & Warranty
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Project is released under MIT License. It can be freely used as long as the original creator is attributed. Software provided comes with no warranty.

## Contents
Repository contains the source code for two projects:
 - main project - `./ATCConsole.pro` - containing C++ source code of simulator using Qt 5.6 libraries
 - test project - `./test/ATCConsoleTest.pro` - containing C++ unit tests using own solution based on QtTest
 
Sometimes `main.cpp` files from both projects may be mixed up during compilation. If that happens, I would recommend either total separation of both projects or decorating the `main.cpp` name (e.g. `main_.cpp`) in the other project.

## Installation
Code can be downloaded and built straight away using qmake in Qt Toolkit with Qt Libraries version of at least 5.6. Alternatively, you can convert it to CMake and go from there (you will need to provide Qt Libraries manually).
The project uses functionalities from two other external libraries:
 - [GeographicLib](https://geographiclib.sourceforge.io/) - computations on WGS-84 ellipsoid
 - [Intel Math Kernel Library](https://software.intel.com/en-us/mkl) - linear interpolators

GeographicLib is provided in `./lib/GeographicLib`, but you may want to build it yourself to avoid binary incompatibility. Intel MKL is not provided due to large size. After the libraries have been obtained, paths in project files `./ATCConsole.pro`, `./test/ATCConsoleTest.pro` should be updated, e.g.:
```
CONFIG(debug): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic_d
CONFIG(release): LIBS += -L"$$_PRO_FILE_PWD_/lib/GeographicLib/" -lGeographic

LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/mkl/lib/intel64_win" -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core
LIBS += -L"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2017.2.187/windows/compiler/lib/intel64_win" -llibiomp5md
```

## Data sources
Static navigation data uses [VATSIM](https://www.vatsim.net/) sectorfiles: [*.sct](http://www1.metacraft.com/VRC/docs/doc.php?page=appendix_g) and [*.ese](http://www.euroscope.hu/mediawiki/index.php?title=ESE_Files_Description), which can be obtained from any local VACC. They need to be stripped down of some unused parts and placed in `./config`. Samples of what should be left are provided in the mentioned directory.

Aircraft types are imported from Eurocontrol [Base of Aircraft Data](http://www.eurocontrol.int/services/bada) Family 3. It provides close to full coverage of aircraft types tracked in the ECAC area. Licence to the files must be obtained directly at the maintainer's website. 

Path to the sectorfiles and BADA directory should be changed in `./config/paths.txt`.

## Bugs & Issues
As for every first big, overly ambitious project in young dev's life, several mistakes were made. I'll try to list them here, based of what I learned since then. If you are insane, suffering from insomnia and thus having enough time to solve any of them, feel free to open a pull request. The problems I know of:
 - local code replication due mistakes in architecture
 - poorly designed multithreading resulting in data races
 - few rare, hard to track out-of-bounds errors
 - old code style not using RAII
 - some Qt components are badly handled (e.g. main display)
 - lack of design patterns where they should obviously be used
 
## Functionalities
This section provides sample videos and screens of different simulator functionalities.

Videos:
 - simulation with 3D technology:
[![3D](http://img.youtube.com/vi/GWc-Kyylh48/maxresdefault.jpg)](https://youtu.be/GWc-Kyylh48)
 - simulation with elements of 4D technology:
[![4D](http://img.youtube.com/vi/bYN1PGMVWBc/maxresdefault.jpg)](https://youtu.be/bYN1PGMVWBc)
 
Screenshots:
 - Scenario Creator with file handling:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/scenario_editor.png">
</p>

 - Flight Creator integrated with Eurocontrol Base of Aircraft Data:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/flight_editor.png">
</p>

 - Settings Editor with file handling:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/settings.png">
</p>

 - Leader Lines and Trailing Dots Editor:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/leader_trailing.png">
</p>

 - Flight List allowing change of runways, SIDs and STARs during simulation:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/flight_list.png">
</p>

 - Extended Arrival Manager with interactive labels, interface for RTA modification and statistics:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/aman.png">
</p>

 - two-state interactive Data Tags:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/data_tags.png">
</p>

 - Ruler with magnetic headings and distances along rhumb and geodesic:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/ruler.png">
</p>

 - 4D Route Prediction with levels and ETAs estimation:
<p align="center">
  <img src="https://github.com/ignmiz/Images/blob/master/ATC_Console/route_all_2.png">
</p>

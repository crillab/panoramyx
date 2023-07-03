# Panoramyx - Programming pArallel coNstraint sOlveRs mAde aMazingly easY

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](http://www.gnu.org/licenses/lgpl-3.0)[![Build Status](https://github.com/crillab/autis/actions/workflows/ci.yml/badge.svg)](https://github.com/crillab/autis/actions/workflows/ci.yml)


## Description

Panoramyx is a `C++` framework for solving constraint programming problems in parallel. 

## Building from sources


### Dependencies

For a list of dependencies, please refer to [vcpkg.json](vcpkg.json).

### Build

This library is designed for simplified use in projects using `CMake` and [`vcpkg`](https://vcpkg.io/en/).

```sh
git clone https://github.com/crillab/panoramyx
cd panoramyx 
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="<vcpkg_root>/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

With `vcpkg_root` is the path to your vcpkg install folder. 


### Using as dependencies with vcpkg

We assume that you have configured the registry of `crillab`, see [here](https://crillab.github.io/tootatis/utility) for more information. 

Add `crillab-panoramyx` in your `vcpkg.json` file :

```json
"dependencies": [
      "crillab-panoramyx"
      .....
  ]
```

Find package `crillab-panoramyx` in your `CMakeLists.txt`:

```cmake
find_package(crillab-panoramyx REQUIRED)
```

Finally, link the library to your target:

```cmake
target_link_libraries(<your_target> crillab-panoramyx::crillab-panoramyx)
```

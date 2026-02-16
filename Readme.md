# Project
Distributed Key Value Store

## Build
make sure you have conan in your system
```
mkdir build
```

```
conan install . --output-folder build --build=missing
```

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

```
cmake --build build
```

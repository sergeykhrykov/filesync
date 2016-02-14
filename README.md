# filesync
An utility to sync files between two dirs. Cross-platform, uses hardlinks.

## How to install it using Conan & CMake, (Windows, MSVC 14, x64)
```
$ git clone git@github.com:sergeykhrykov/filesync.git
$ mkdir build.debug
$ cd build.debug
$ conan install .. -s build_type=Debug --build=missing
$ cmake .. -G "Visual Studio 14 Win64"
$ cmake --build . --config Debug
```

or 
```
$ mkdir build.release
$ cd build.release
$ conan install .. -s build_type=Release --build=missing
$ cmake .. -G "Visual Studio 14 Win64"
$ cmake --build . --config Release
```

## Usage

`$ filesync <dir1> <dir2> <file extension>`

## Limitations
- No support for pattern matching in file or folder names yet.
- On Windows, the two folders must be on the same drive (otherwise hardlinks can't be created between them)

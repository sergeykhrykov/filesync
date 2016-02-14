# filesync
An utility to sync files between two dirs. Cross-platform, uses hardlinks (hence, same-drive and NTFS-only for Windows).
Files are filtered by user-provided extension.

## Installation 
Requires Conan & CMake. 

(Windows, MSVC 14, x64)
for Debug:
```
$ git clone git@github.com:sergeykhrykov/filesync.git
$ mkdir build.debug
$ cd build.debug
$ conan install .. -s build_type=Debug --build=missing
$ cmake .. -G "Visual Studio 14 Win64"
$ cmake --build . --config Debug
```

or, for Release: 
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
- On Windows, the two folders must be on the same NTFS drive (otherwise hardlinks can't be created between them)

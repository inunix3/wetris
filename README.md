# wetris

A tetris clone written in C11 and using SDL3.

<p align="center">
  <img src="screenshots/screenshot.png" alt="a screenshot of the tetris">
</p>

## Building on Linux

```
mkdir build; cd build
cmake -G 'your favorite generator' -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

Currently SDL3 is not widely installable from repositories, so you'll probably need to pass
`-DVENDORED_LIBS=ON` to build SDL3 and its extensions.

After a successful building, binary `src/wetris` will be lying in the build directory.

If you like my tetris, you can also install it from the build directory:

```
sudo cmake --install .
```

## Building on Windows

You can obtain prebuilt binaries from the [Releases](https://github.com/inunix3/wetris/releases) page.

### Compiling from sources

Create a build directory:

```
mkdir build; cd build
```

If you're going to use multi-config generator like Visual Studio, these flags must be passed, and
the build type must be specified in build-time:

```
cmake -G 'your favorite generator' -DVENDORED_LIBS=ON -DSDLIMAGE_VENDORED=ON -DSDLMIXER_VENDORED=ON -DSDLTTF_VENDORED=ON ..
cmake --build . --config Release
```

If you wish to use single-config generator like MinGW makefiles:

```
cmake -G 'your favorite generator' -DCMAKE_BUILD_TYPE=Release -DVENDORED_LIBS=ON -DSDLIMAGE_VENDORED=ON -DSDLMIXER_VENDORED=ON -DSDLTTF_VENDORED=ON ..
cmake --build .
```

Although if you're using MinGW, you can actually try to copy SDL3 and its extensions into
compiler directories from packages marked as devel.
The SDL libraries can be obtained from [their repositories](https://github.com/libsdl-org).

After a successful building, binary `src/wetris.exe` will be lying in the build directory.

## Controls

| Key           | Action                  |
|---------------|-------------------------|
| `escape`      | Quit                    |
| `a` / `left`  | Move left               |
| `d` / `right` | Move right              |
| `q`           | Rotate counterclockwise |
| `e`           | Rotate clockwise        |
| `s` / `down`  | Speed up falling        |
| `space`       | Drop                    |
| `p`           | Pause                   |

## Contribution

If you have found a problem or have a suggestion, feel free to open an issue or send a pull request.
I'd appreciate it.

## Notice

Used font is **The PC Paint Font Collection** made by [codeman38](https://www.zone38.net/font/).

Graphics were made in Aseprite and Gimp, sound effects in MilkyTracker and Audacity.

## License

wetris is licensed under the [MIT license](LICENSE.md).

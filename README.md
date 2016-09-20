# [Holo](http://www.wastevensv.com/projects/holo/)
<img alt="Projection" src="http://www.wastevensv.com/projects/holo/holo_projection.jpg" width=400>

I've seen a lot of tutorials online about how to make a phone sized reflective prism for projecting 'holograms' (not technically a hologram, but creates the illusion of one). However, they were too small to be very interesting, and I couldn't find any software to render a object properly. As a result, I created a 4-sided reflective prism from 8"x10" lexan sheets, and wrote software to render an object in the proper perspectives.

## Features
 * Uses multiple glViewports to render the object onto the same screen from different perspectives.
 * Normalizes the size of an object to properly fit on the screen.
 * Adjustable distance and rotation.

## Building with Windows (using git bash)
### Dependencies
* [Visual Studio](https://www.visualstudio.com/) (Only tested with 2015 x64)
* [Git Bash](https://git-scm.com/downloads)
* [cmake](https://cmake.org/)
```bash
git clone https://github.com/wastevensv/Holo
git submodule update --init
mkdir build
cd build
cmake .. # May need to specify generator with -G option. Currently only "Visual Studio 14 2015 Win64" is known working.
# Then open generated Holo.sln file and build in Visual Studio.
cd Debug
./holo ../objs/bb8.obj
```

![Screenshot](http://www.wastevensv.com/projects/holo/holo_screenshot.png)

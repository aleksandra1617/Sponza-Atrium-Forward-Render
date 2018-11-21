# Reprise My Sponza

A revision and improvement lab exercise for my graphics programming students.

## Getting Started

### Obtaining

The project can be obtained from the git repository located at
https://bitbucket.org/Tees3GP/reprisemysponza.git, for example:

```
git clone https://bitbucket.org/Tees3GP/reprisemysponza.git
```

### What's Included

```
RepriseMySponza/
|-- RepriseMySponza/
|   |-- assets/
|   |-- doc/
|   |-- shaders/
|   |-- source
|-- sponza/
|-- tgl/
|-- tygra/
```

The root directory contains the Visual Studio solution tying the sub-projects
together.

The `RepriseMySponza` directory contains the necessary project configuration
files.

The `assets` sub-directory contains runtime data files required by the app
(e.g. texture image files).

The `doc` sub- directory contains any documentation or configuration files for
redistribution to users.

The `shaders` sub-directory contains the runtime shaders files for
redistribution with the app to users.

The `source` sub-directory contains the source code (including any private
headers) for the project.

The `sponza` directory contains a sub-project to expose the Sponza environment
to the programmer.

The `tgl` directory contains a sub-project to handle exposing OpenGL function
prototypes based upon desired OpenGL version.

The `tygra` directory contains a sub-project to provide OpenGL window
management and utility file operations.

### Using

The project is designed to be used within Visual Studio producing an
executable.

The project will build into a directory structure under a `build` sub-directory
in the solution directory.

For ease, all projects in the solution are using the tdk configuration files.
If you add additional sub-projects, easily embed them by copying `tdk.props`
and `tdk.targets` into the new project directories and include the following
at the appropriate location in their `vcxproj` files:

```
<Import Project="tdk.props" />
<Import Project="tdk.targets" />
```

RepriseMySponza is dependent upon the following libraries:

* TGL: available from https://bitbucket.org/tpdavison/tgl

* TyGrA: available from https://bitbucket.org/tpdavison/tygra

* GLFW3: available from http://www.glfw.org/

* GLM: available from https://github.com/g-truc/glm

* TCF: available from https://scma-intranet.tees.ac.uk/users/u0018196/tdk/tcf.zip

* TDL: available from https://scma-intranet.tees.ac.uk/users/u0018196/tdk/tdl.zip

* TSL: available from https://scma-intranet.tees.ac.uk/users/u0018196/tdk/tsl.zip

* Sponza: available from https://scma-intranet.tees.ac.uk/users/u0018196/tdk/sponza.zip

* NvToolsExt (optional): available as part of NSIGHT (https://developer.nvidia.com/nvidia-nsight-visual-studio-edition)

TGL, TyGrA and Sponza are expected to be a side-along project in the parent
solution.  The other library files should be added into the appropriate
location in the following directory structure under the parent solution
directory.

```
external\
|-- bin\
|   |-- Win32\
|   |-- x64\
|-- doc\
|-- include\
|   |-- GLFW\
|   |-- glm\
|   |-- tcf\
|   |-- tdl\
|   |-- tsl\
|-- lib\
|   |-- Win32\
|   |-- x64\
|-- res\
```

Dynamic library files (DLLs) should be placed in the appropriate sub-directory
of the `bin` directory.  Runtime resource files should be placed in the `res`
directory.

## Known Issues

There are currently no known bugs -- please report potential bugs via email
to the author(s).

Feature requests are welcome -- please email the author(s).

## Authors and Contributors

Tyrone Davison (<t.p.davison@tees.ac.uk>)

## License

This project is licensed under the MIT License.

Copyright (c) 2016 Tyrone Davison (<t.p.davison@tees.ac.uk>)

```
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```

This software includes the following third-party contributions:

Crytek Sponza  
Copyright (c) 2011 Morgan McGuire (<http://g3d.cs.williams.edu/g3d/data10/index.html>)  
Copyright (c) 2010 Frank Meinl, Crytek (<http://www.crytek.com/cryengine/cryengine3/downloads>)  
Copyright (c) 2002 Marko Dabrovic

Stanford Bunny  
Copyright (c) 2011 Morgan McGuire (<http://g3d.cs.williams.edu/g3d/data10/index.html>)  
Copyright (c) 1996 Stanford Computer Graphics Laboratory (<http://www.graphics.stanford.edu/data/3Dscanrep/>)

Happy Buddha  
Copyright (c) 2011 Morgan McGuire (<http://g3d.cs.williams.edu/g3d/data10/index.html>)  
Copyright (c) 1996 Stanford Computer Graphics Laboratory (<http://www.graphics.stanford.edu/data/3Dscanrep/>)

Dragon  
Copyright (c) 2011 Morgan McGuire (<http://g3d.cs.williams.edu/g3d/data10/index.html>)  
Copyright (c) 1996 Stanford Computer Graphics Laboratory (<http://www.graphics.stanford.edu/data/3Dscanrep/>)

GLFW  
Copyright (c) 2002-2006 Marcus Geelnard  
Copyright (c) 2006-2010 Camilla Berglund (<elmindreda@elmindreda.org>)

OpenGL Mathematics (GLM)  
Copyright (c) 2005 - 2016 G-Truc Creation (http://www.g-truc.net)

libpng  
libpng versions 1.0.7, July 1, 2000 through 1.6.23, June 9, 2016 are  
Copyright (c) 2000-2002, 2004, 2006-2016 Glenn Randers-Pehrson, are  
derived from libpng-1.0.6, and are distributed according to the same  
disclaimer and license as libpng-1.0.6

zlib  
(C) 1995-2013 Jean-loup Gailly and Mark Adler

uriparser - RFC 3986 URI parsing library  
Copyright (C) 2007, Weijia Song (<songweijia@gmail.com>)  
Copyright (C) 2007, Sebastian Pipping (<sebastian@pipping.org>)  
All rights reserved.

NVTX  
Copyright 2009-2012  NVIDIA Corporation.  All rights reserved.

# TGL

A small C API exposing OpenGL utility functions, primarily loading GL
functions (upto 4.5) without involving `windows.h` whilst hiding prototypes
for GL versions other than that which is desired.

## Getting Started

### Obtaining

The project can be obtained from the git repository located at https://bitbucket.org/tpdavison/tgl.git, for example:

```
git clone https://bitbucket.org/tpdavison/tgl.git
```

However, the project is usually embedded in other projects as a git submodule.

### What's Included

```
tgl/
|-- doc/
|-- include/
|-- src/
```

The root directory contains the necessary configuration files.

The `doc` directory contains any documentation or configuration files for
redistribution to client programmers.

The `include` directory contains the public header files for redistribution to
client programmers.

The `src` directory contains the source code (including any private headers)
for the project.

### Using

The project is designed to be used as a dependent project within a parent
Visual Studio solution, producing a lib file to be linked against.

The project will build its lib files into a directory structure under a `build`
sub-directory in the solution directory.

For easy embedding within a parent solution, ensure the other projects in the
solution are using the `tdk.props`, `tdk.targets` and `tgl.lib.props`
configuration files by copying them into the various project directories and
including the following at the appropriate location in their `vcxproj` files:

```
<Import Project="tdk.props" />
<Import Project="tdk.targets" />
```

TGL is designed to let you select which version of OpenGL you would like to
expose in the client code.  To do so, ensure an appropriate `#define` is made
before including `tgl.h` in client source code, for example:

```
#define TGL_TARGET_GL_4_1
#include <tgl/tgl.h>
```

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

This also project includes portions of software under MIT license with the
following copyrights:

Copyright (c) 2013-2016 The Khronos Group Inc.

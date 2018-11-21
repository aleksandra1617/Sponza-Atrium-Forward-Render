# TyGrA

A small library of C++ classes and utility functions to aid with managing
OpenGL windows and reading data files.

## Getting Started

### Obtaining

The project can be obtained from the git repository located at
https://bitbucket.org/tpdavison/tygra.git, for example:

```
git clone https://bitbucket.org/tpdavison/tygra.git
```

However, the project is usually embedded in other projects as a git submodule.

### What's Included

```
tygra/
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
solution are using the `tdk.props`, `tdk.targets`, `tygra.vars.props`,
`tygra.lib.targets`, `tygra.xml` and `tygra.lib.props` configuration files by
copying them into the various project directories and including the following
at the appropriate location in their `vcxproj` files:

```
<Import Project="tdk.props" />
<Import Project="tdk.targets" />
```

TyGrA is dependent upon the following libraries:

* TGL: available from https://bitbucket.org/tpdavison/tgl

* TDL: available from https://scma-intranet.tees.ac.uk/users/u0018196/tdk/tdl.zip

* GLFW3: available from http://www.glfw.org/

TGL is expected to be a side-along project in the parent solution.  The other
library files should be added into the appropriate location in the following
directory structure under the parent solution directory.

```
external\
|-- bin\
|   |-- Win32\
|   |-- x64\
|-- doc\
|-- include\
|   |-- GLFW\
|   |-- tdl\
|-- lib\
    |-- Win32\
	|-- x64\
```

Dynamic library files (DLLs) should be placed in the appropriate sub-directory
of the `bin` directory.

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

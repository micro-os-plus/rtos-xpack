
[![GitHub package.json version](https://img.shields.io/github/package-json/v/micro-os-plus/rtos-xpack)](https://github.com/micro-os-plus/rtos-xpack/blob/xpack/package.json)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/micro-os-plus/rtos-xpack)](https://github.com/micro-os-plus/rtos-xpack/tags/)
[![npm (scoped)](https://img.shields.io/npm/v/@micro-os-plus/rtos.svg?color=blue)](https://www.npmjs.com/package/@micro-os-plus/rtos/)
[![license](https://img.shields.io/github/license/micro-os-plus/rtos-xpack)](https://github.com/micro-os-plus/rtos-xpack/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/rtos-xpack/actions/workflows/CI.yml/badge.svg)](https://github.com/micro-os-plus/rtos-xpack/actions/workflows/CI.yml)

# A source library xPack with the µOS++ RTOS files

A source library xPack with the **µOS++ RTOS**, a POSIX inspired open
source framework, written in C++.

The project is hosted on GitHub as
[micro-os-plus/rtos-xpack](https://github.com/micro-os-plus/rtos-xpack).

## Maintainer info

This page is addressed to developers who plan to include this source
library into their own projects.

For maintainer info, please see the
[README-MAINTAINER](README-MAINTAINER.md) file.

## Install

As a source library xPack, the easiest way to add it to a project is via
**xpm**, but it can also be used as any Git project, for example as a submodule.

### Prerequisites

A recent [xpm](https://xpack.github.io/xpm/),
which is a portable [Node.js](https://nodejs.org/) command line application.

For details please follow the instructions in the
[xPack install](https://xpack.github.io/install/) page.

### xpm

Note: the package will be available from npmjs.com at a later date.

For now, it can be installed from GitHub:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install github:micro-os-plus/rtos-xpack
```

When ready, this package will be available as
[`@micro-os-plus/rtos`](https://www.npmjs.com/package/@micro-os-plus/rtos)
from the `npmjs.com` registry:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install @micro-os-plus/rtos@latest

ls -l xpacks/micro-os-plus-rtos
```

### Git submodule

If, for any reason, **xpm** is not available, the next recommended
solution is to link it as a Git submodule below an `xpacks` folder.

```sh
cd my-project
git init # Unless already a Git project
mkdir -p xpacks

git submodule add https://github.com/micro-os-plus/rtos-xpack.git \
  xpacks/micro-os-plus-rtos
```

## Branches

Apart from the unused `master` branch, there are two active branches:

- `xpack`, with the latest stable version (default)
- `xpack-develop`, with the current development version

All development is done in the `xpack-develop` branch, and contributions via
Pull Requests should be directed to this branch.

When new releases are published, the `xpack-develop` branch is merged
into `xpack`.

## Developer info

The **µOS++** _(micro oh ɛs plus plus)_ project is
the third iteration of µOS++, a POSIX-like, portable, open source,
royalty-free, multi-tasking real-time framework written in C++,
intended for 32/64-bits embedded applications.

The APIs are documented in the
[µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

### Status

The µOS++ project is fully functional for Cortex-M devices and also runs
on synthetic POSIX platforms (like macOS and GNU/Linux).

### Related projects

The current µOS++ code tries to be as portable as possible.

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is packed as
[µOS++ Cortex-M architecture xPack](https://github.com/micro-os-plus/architecture-cortexm-xpack)
- the synthetic POSIX port of the scheduler is packed as
[µOS++ POSIX architecture xPack](https://github.com/micro-os-plus/architecture-posix-xpack)

### Build & integration info

To include this package in a project, consider the following details.

#### Include folders

The following folders should be passed to the compiler during the build:

- `include`

TODO: list the available headers

#### Source files

The source files to be added to the build are:

TODO

#### Preprocessor definitions

See [µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

#### Compiler options

- `-std=c++20` or higher for C++ sources
- `-std=c11` for C sources

#### C++ Namespaces

TBD

#### C++ Classes

TBD

### Examples

TBD

### Known problems

- none

### Tests

TBD

### Documentation

- [µOS++](http://micro-os-plus.github.io)
- [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)

### Demo projects

TODO

## Change log - incompatible changes

According to [semver](https://semver.org) rules:

> Major version X (X.y.z | X > 0) MUST be incremented if any
backwards incompatible changes are introduced to the public API.

The incompatible changes, in reverse chronological order,
are:

- TBD

## License

The original content is released under the
[MIT License](https://opensource.org/licenses/MIT/),
with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul/).

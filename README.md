
# µOS++ RTOS

A source xPack with the **µOS++ RTOS**, a POSIX inspired open
source framework, written in C++. The project is hosted on
GitHub as
[micro-os-plus/rtos-xpack](https://github.com/micro-os-plus/rtos-xpack).

## Purpose

The **µOS++** _(micro oh ɛs plus plus)_ project is
the third iteration of µOS++, a POSIX-like, portable, open source,
royalty-free, multi-tasking real-time framework written in C++,
intended for 32/64-bits embedded applications.

The APIs are documented in the
[µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

## Status

The µOS++ project is fully functional for Cortex-M devices and also runs
on synthetic POSIX platforms (like macOS and GNU/Linux).

## Related projects

The current µOS++ code tries to be as portable as possible.

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is packed as
[µOS++ Cortex-M architecture xPack](https://github.com/micro-os-plus/architecture-cortexm-xpack)
- the synthetic POSIX port of the scheduler is packed as
[µOS++ POSIX architecture xPack](https://github.com/micro-os-plus/architecture-posix-xpack)

## Build Configuration

To include µOS++ in a project, in addition to one of the port
specific project, consider the following details:

### Include folders

- `include`

### Source folders

- `src`

### Symbols

See [µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

## Documentation

- [µOS++](http://micro-os-plus.github.io)
- [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)

## Demo projects

TODO

## Tests

TODO

## License

The original content is released under the
[MIT License](https://opensource.org/licenses/MIT), with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul).

# Glance - An OpenGL boilerplate project

[![CMake](https://github.com/dy5function/glance/actions/workflows/cmake.yml/badge.svg)](https://github.com/dy5function/glance/actions/workflows/cmake.yml/badge.svg)

## Summary

*Glance* is a boilerplate OpenGL project structure that I am developing for personal projects and want anybody to freely use for their purposes. This project is heavily inspired by [Glitter](http://polytonic.github.io/Glitter/), so make sure to check out this project as well!

## Dependencies

This section is intended to list all external dependencies needed for building and running Glance that are not included as submodules with this project. As the project progresses, additional operating systems might get added here. In general, the goal is to have as little dependencies as possible so this project is easy to setup.

### Ubuntu 22

```bash
sudo apt install cmake xorg-dev
```

## Building Glance

*Glance* currently supports building on *Linux* via [cmake](https://cmake.org/). From the project root run the following:

```bash
# Create build directory and changedir into it
mkdir build
cd build

# Generate makefile
cmake ..

# Build Glance and dependencies
make
```

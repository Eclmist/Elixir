<h1 align="center">
  <br>
  <a href="https://elixir.moe/"><img src="https://raw.githubusercontent.com/eclmist/elixir/master/docs/elixir.png" alt="Elixir-Logo" width="200"></a>
  <br>
</h1>
<h4 align="center">A (WIP) physically based raytracing renderer based on <a href="http://www.pbr-book.org/" target="_blank">PBRT</a> and <a href="http://sort-renderer.com/" target="_blank">SORT</a>.</h4>

<p align="center">
  <a href="https://travis-ci.com/Eclmist/Elixir">
    <img src="https://travis-ci.com/Eclmist/Elixir.svg?branch=master" alt="Travis Build Status">
  </a>
  <a href="#------">
    <img src="https://img.shields.io/badge/stability-experimental-orange.svg">
  </a>
  <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">
    <img src="https://img.shields.io/badge/license-GPL3--or--later-blue.svg">
  </a>
</p>

<div align="center">
  <h7>
    <a href="https://elixir.moe">
      Website
    </a>
    <span> | </span>
    <a href="https://docs.elixir.moe">
      Documentation
    </a>
  </h7>
</div>

## Introduction

Elixir is a cross-platform raytracing renderer written in C++, meant be used as a standalone program or used in 3D packages such as [Blender](https://www.blender.org/) and [Cauldron](https://github.com/DarkDestry/Cauldron) as a renderer plugin. 

Inspired by previous works such as [PBRT](http://www.pbr-book.org/) and [SORT](http://sort-renderer.com/), Elixir is physically based and implements similar features such as path tracing, special acceleration structures, and various BXDFs. The project is still in its early stages and currently under development.

Elixir is open-sourced, licensed under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html).

## Getting Started

> Note: Elixir is currently a WIP and is considered unstable. Proceed with caution at your own risk.

Elixir uses CMake to build it's binaries. Run the following CMake commands to generate projects and build using CMake.



```shell
$ cmake -B build -S . -DCMAKE_CONFIGURATION_TYPES:STRING="Debug;Release;"
$ cmake --build .
```

This will generate OS specific project in the `/build/` folder, and will build the Elixir binaries into `/bin/`.

## Screenshots

<p align="center">
  <img src="https://raw.githubusercontent.com/Eclmist/Elixir/master/docs/output1.png" width=700>
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/Eclmist/Elixir/master/docs/output2.png" width=700>
</p>

# Just Another Web Crawler

## Table of Contents

- [About](#about)
- [Getting Started](#getting_started)

## About <a name = "about"></a>

Just a simple web crawler made in C. I just wanted to code something and this seemed like a nice project.

Finally learned CMake tho... worth it.

## Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

You will need CMake, a C compiler (preferably clang), and whatever dependencies cURL and Lexbor have.

```bash
Arch Linux:
sudo pacman -S clang cmake

Other OS:
Idk, you can figure it out, i believe in you.
```

### Installing

Just run these commands, CMake will download an build the dependencies,

```bash
git clone https://github.com/TarcisMoreda/justAnotherWebCrawler.git
cd justAnotherWebCrawler/build
cmake -DCMAKE_BUILD_TYPE=Release .. OR cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
src/jawc_app
```

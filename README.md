## Reflection

**Summarize the project and what problem it was solving.**

The Corner Grocer wanted to know which items people buy the most so they could rearrange the produce section. They had a text file where every purchase was just an item name on its own line, thousands of them, and no easy way to read it. My program reads that file, counts how many times each item shows up, saves the counts to `frequency.dat` as a backup, and then gives a menu where you can look up one item, list every item with its count, or print a histogram made of asterisks. Simple problem. But it was the first time in this course I built something that felt like a real tool instead of an exercise.

**What did you do particularly well?**

I think the structure is the part I'm proudest of. Instead of throwing everything into `main.cpp`, I split the code into folders for app logic, data, file I/O, and the UI, each with its own headers under `include/grocery/`. That meant when the menu was acting up I only had to look in one place. I also set up Doxygen and a GitHub Actions workflow that publishes the docs to GitHub Pages, plus another workflow that builds binaries for Linux, macOS, and Windows. None of that was required. I did it because I wanted to see if I could, and honestly it was the most fun part.

**Where could you enhance your code?**

Input handling could be tighter. Right now the program trusts that the input file is clean, one item per line, and it would be better to trim whitespace and normalize capitalization so "apples" and "Apples " don't get counted as two different things. I'd also like to add unit tests. I tested by running the menu over and over and checking the numbers by hand, which works for a file this size but wouldn't scale. On the security side, the program reads a file path from the working directory and overwrites `frequency.dat` without asking, so checking that the files exist and are what you expect before touching them would make it safer to run somewhere other than my own laptop.

**Which pieces of the code did you find most challenging to write, and how did you overcome this?**

Weirdly, the hardest parts weren't the C++. They were the build setup. When I added the Visual Studio project, the linker kept failing because I had four files all named `controller.cpp` in different folders, and Visual Studio was dumping their object files into the same place so they overwrote each other. It took me a while to figure out that the code was fine and the build config was the problem. The fix was telling Visual Studio to keep each source folder's path in its object file names. GitHub Pages was the other headache. My docs workflow kept failing with an error about a missing `docs` folder, and it turned out Pages was set to deploy from a branch and was running Jekyll instead of my workflow. Switching the source to GitHub Actions fixed it. For resources, I leaned on cppreference, the GitHub Actions docs, and a lot of reading through other people's issue threads. I'm keeping those in my toolkit, along with clang-tidy, which caught a few things I'd never have noticed.

**What skills from this project will be particularly transferable?**

Breaking a program into small classes with one job each is the big one. I'll use that in pretty much every project from now on. Reading and writing files, using a map to count things, and building a menu loop that doesn't crash on bad input are all basic, but they show up everywhere. The CI and documentation setup will carry over too, since almost every real codebase has some kind of automated build.

**How did you make this program maintainable, readable, and adaptable?**

Every class and function has a Doxygen comment explaining what it does, and I used clang-format so the style stays consistent no matter which editor I'm in. I also made class and flowchart diagrams with Graphviz and put them in the `diagrams` folder so someone new can see how the pieces fit before reading any code. Because the file reading, the data storage, and the display are separate, swapping one out is easy. If the grocer wanted the report as a CSV, or wanted to read from a database instead of a text file, I'd only have to change one part and leave the rest alone.

---

# Grocery Frequency Report

CS-210 Project Three — Grocery Frequency Report

This C++17 console application reads grocery item records, counts item
occurrences, writes a frequency report, and provides an interactive menu for
searching and displaying the collected data.

## Features

- Reads grocery records from `CS210_Project_Three_Input_File.txt`
- Counts occurrences of each grocery item
- Writes serialized results to `frequency.dat`
- Searches for an individual item's quantity
- Lists all items and their quantities
- Displays an asterisk-based frequency histogram
- Generates API documentation with Doxygen
- Publishes generated documentation to GitHub Pages through GitHub Actions
- Builds and publishes release binaries for Linux, macOS, and Windows

## Requirements

- A C++17-compatible compiler
- GNU Make
- Doxygen for local documentation generation
- GitHub Actions and GitHub Pages for automatic online documentation publishing

The project has been tested with the compiler and tools available on macOS.

## Binary releases

Create and push a version tag to build and publish compressed binaries:

```bash
git tag v1.0.0
git push origin v1.0.0
```

The `Build and publish binaries` workflow also supports manual runs from the
repository's **Actions** tab. Every push to `main` creates downloadable
workflow artifacts. To publish those artifacts as a GitHub Release without
creating a tag first, manually run the workflow with **Publish the build
artifacts as a GitHub Release** enabled. You may provide a release tag, or
leave it blank to use `build-<run number>`. Tagged runs publish one asset for each supported
operating-system and architecture combination:

```text
grocery-linux-x64.tar.gz
grocery-linux-x86.tar.gz
grocery-linux-arm64.tar.gz
grocery-macos-x64.tar.gz
grocery-macos-arm64.tar.gz
grocery-windows-x64.tar.gz
grocery-windows-x86.tar.gz
grocery-windows-arm64.tar.gz
```

The archives contain the executable named for the matching platform and
architecture. Linux x86 and Windows x86 are 32-bit builds. Current macOS SDKs
no longer support linking 32-bit Intel (`i386`) applications, so macOS
releases target Intel 64-bit (`x64`) and Apple Silicon (`arm64`) only. The
macOS x64 build is cross-compiled on the newer macOS runner to avoid the
limited availability of older Intel runners.

## Building

From the project root, run:

```bash
make
```

The release executable is created at:

```text
build/release/grocery
```

To build the debug executable without launching a debugger:

```bash
make build/debug/grocery
```

To build and launch the debug executable in LLDB or GDB:

```bash
make debug
```

The debug executable is created at:

```text
build/debug/grocery
```

You can also build the release target explicitly:

```bash
make release
```

To remove generated build files:

```bash
make clean
```

## Visual Studio Code tasks

The Makefile targets are available from **Terminal → Run Task** in Visual
Studio Code. The default build task is `Make: build`; the other available
tasks are `Make: release`, `Make: build debug`, `Make: run`, `Make: debug`,
`Make: docs`, `Make: diagrams`, and `Make: clean`.

## Visual Studio

Visual Studio 2026 project files are included for Windows development:

```text
Grocery.sln
```

Open `Grocery.sln` in Visual Studio 2026, select `Debug` or `Release` and either
the `x64` or `ARM64` platform, and build the `Grocery` project. The
executable is written to:

```text
build\vs\<Platform>\<Configuration>\grocery.exe
```

The project uses the repository root as its working directory so
`CS210_Project_Three_Input_File.txt` is found when launched from Visual
Studio. The Visual Studio project is an additional build option; the
Makefile remains the command-line build system for macOS and Unix-like
environments. Visual Studio preserves each source directory in its
intermediate object paths so files with the same name, such as the four
`controller.cpp` files, receive unique intermediate object names and do not
collide during linking. After changing the project configuration, use
**Build → Clean Solution**, then **Build → Rebuild Solution**.

## Running

Run the application with:

```bash
make run
```

The program expects the input file to be available in the current working
directory. It creates or replaces `frequency.dat` in that same directory.

The interactive menu provides these options:

1. Find an item
2. List items and quantities
3. Print a histogram
4. Exit

You can also enter `q` at the main menu to quit. While searching for an item,
enter `q` to return to the main menu.

## Input and output

### Input file

`CS210_Project_Three_Input_File.txt` contains grocery item records. Each item
is represented as a separate line, for example:

```text
Apples
Broccoli
Apples
```

### Output file

`frequency.dat` is generated when the application starts. It contains the
serialized frequency report produced from the in-memory grocery store.

## Project structure

```text
.
├── include/                  Public header files
│   └── grocery/
│       ├── app/              Application orchestration
│       ├── data/             Storage, serialization, and presentation
│       ├── io/               File reading and writing
│       └── ui/               Menu and console dialogs
├── src/                      C++ implementations
├── main.cpp                  Program entry point
├── Makefile                  Build, run, clean, and documentation targets
├── CS210_Project_Three_Input_File.txt
├── frequency.dat
├── Doxyfile                  Doxygen configuration
├── diagrams/                 Flowchart and class design diagrams
└── .github/workflows/
    └── docs.yml              GitHub Pages documentation workflow
```

## Documentation

Generate the HTML documentation locally with:

```bash
make docs
```

The generated site is located at:

```text
docs/index.html
```

The published Doxygen documentation is available online at:

```text
https://dr3mro.github.io/CS-210-12997-Project_Three_Grocery_Frequency_Report/
```

[Open the published Doxygen documentation](https://dr3mro.github.io/CS-210-12997-Project_Three_Grocery_Frequency_Report/)

The `Publish Doxygen documentation` GitHub Actions workflow runs whenever
changes are pushed to the `main` branch. It also supports manual execution
from the repository's **Actions** tab. The workflow runs `make docs`, packages
the generated HTML, and deploys it to GitHub Pages.

To enable the online deployment, configure the repository settings as:

```text
Settings → Pages → Build and deployment → Source: GitHub Actions
```

Do not select **Deploy from a branch**. That mode makes GitHub run Jekyll
against the repository's `docs` directory instead of using the Doxygen HTML
artifact produced by the workflow.

The workflow must be selected from the repository's **Actions** tab and run
after changing the Pages source. If an Actions log contains:

```text
Source: /github/workspace/./docs
Error: No such file or directory ... /github/workspace/docs
```

GitHub is using the branch-based Jekyll publisher instead of
`.github/workflows/docs.yml`. Set **Settings → Pages → Build and deployment →
Source** to **GitHub Actions**, save, and rerun the `Publish Doxygen
documentation` workflow. If the workflow reports `Get Pages site failed`, a
repository administrator must enable Pages first.

## Development commands

| Command                    | Description                                          |
| -------------------------- | ---------------------------------------------------- |
| `make`                     | Build the release executable                         |
| `make release`             | Build the release executable explicitly              |
| `make run`                 | Build and run the application                        |
| `make build/debug/grocery` | Build the debug executable                           |
| `make debug`               | Build and launch the debug executable in LLDB or GDB |
| `make diagrams`            | Regenerate the flowchart and class diagram PNG files |
| `make docs`                | Generate Doxygen HTML documentation                  |
| `make clean`               | Remove build and generated documentation files       |

CMake, Ninja, vcpkg and the `./cx` helper are also supported. See the
[template wiki](https://github.com/dr3mro/cpp-template/wiki) for those
workflows.

## License

This project was created for the SNHU CS-210 course.

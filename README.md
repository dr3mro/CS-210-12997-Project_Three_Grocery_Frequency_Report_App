# Grocery Frequency Report

CS-210 Project Three — Grocery Frequency Report

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

This C++20 console application reads grocery item records, counts item
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

- A C++20-compatible compiler (GCC 10+, Clang 14+, or MSVC 19.29+ / Visual Studio 2019 16.11+)
- CMake 3.25 or newer, used by the `./cx` helper
- Doxygen for local documentation generation
- GitHub Actions and GitHub Pages for automatic online documentation publishing

The project has been tested with the compiler and tools available on macOS.

## Binary releases

Create and push a version tag, `v` followed by the version in the `VERSION` file, to build and publish compressed binaries:

```bash
git tag v1.0.0
git push origin v1.0.0
```

The `Release` workflow (`.github/workflows/release.yml`) also supports manual
runs from the repository's **Actions** tab. To publish a GitHub Release without
creating a tag first, run the workflow manually with **Publish the build
artifacts as a GitHub Release** (`publish_release`) enabled. Optionally, enter a
release tag; if you leave it blank, the tag is `build-<run number>`. Tagged runs
and manual runs with publishing enabled produce these archives for each
supported operating-system and architecture combination:

```text
grocery-linux-x64.tar.gz
grocery-linux-x86.tar.gz
grocery-linux-arm64.tar.gz
grocery-macos-x64.tar.gz
grocery-macos-arm64.tar.gz
grocery-windows-x64.zip
grocery-windows-x86.zip
grocery-windows-arm64.zip
```

Linux and macOS archives are `.tar.gz` files. Windows archives are `.zip` files.
Each platform can also produce two companion archives, with the same extension
as its main archive:

```text
grocery-<os>-<arch>-debug   Debug symbols (.debug on Linux, .dSYM on macOS, .pdb on Windows)
grocery-<os>-<arch>-sbom    Software bill of materials
```

The release also includes a `SHA256SUMS` file with the checksums of every archive.

The archives contain the executable named for the matching platform and
architecture. Linux x86 and Windows x86 are 32-bit builds. Current macOS SDKs
no longer support linking 32-bit Intel (`i386`) applications, so macOS
releases target Intel 64-bit (`x64`) and Apple Silicon (`arm64`) only. The
macOS x64 build is cross-compiled on the newer macOS runner to avoid the
limited availability of older Intel runners.

## Visual Studio Code tasks

The Makefile targets are available from **Terminal → Run Task** in Visual
Studio Code. The default build task is `Make: build`; the other available
tasks are `Make: release`, `Make: build debug`, `Make: run`, `Make: debug`,
`Make: docs`, `Make: diagrams`, and `Make: clean`.

## Visual Studio

Visual Studio project files are included for Windows development:

```text
grocery.sln
grocery.vcxproj
```

Open `grocery.sln` in Visual Studio, select `Debug` or `Release` and the
platform you want, and build the `grocery` project. The solution also includes a
`grocery_tests` project. Build output is written under:

```text
build\Windows\<arch>\vs\
```

The project uses the repository root as its working directory so
`CS210_Project_Three_Input_File.txt` is found when launched from Visual
Studio. The Visual Studio project is an additional build option. The
command-line build uses CMake and the `./cx` helper described below.

## Building with CMake

The project uses CMake. The `./cx` helper wraps the CMake presets. From the
project root, run:

```bash
./cx build
```

On Windows, use `cx.cmd` or `cx.ps1` instead. The default preset is `debug`,
which enables AddressSanitizer and UndefinedBehaviorSanitizer. `cx` needs CMake
3.25 or newer on your PATH.

Binaries are written to a host-specific folder:

```text
build/<Host>/<preset>/grocery
```

`<Host>` is `Darwin`, `Linux`, or `Windows`. For example, a release build on
macOS produces `build/Darwin/release/grocery`.

To remove generated build files:

```bash
./cx clean
```

Use `./cx clean --all` to remove everything, including all presets.

## Presets

Available CMake presets include:

| Preset                     | Purpose                                   |
| -------------------------- | ----------------------------------------- |
| `release`                  | Optimized build                           |
| `debug`                    | Debug build with ASan and UBSan (default) |
| `debug-nosan`              | Debug build without sanitizers            |
| `tsan`                     | ThreadSanitizer build                     |
| `ci`, `ci-gcc`, `ci-clang` | Configurations used by CI                 |
| `coverage`                 | Coverage build                            |
| `fuzz`                     | Fuzzing build                             |
| `docs`                     | Documentation build                       |
| `vs2022`, `vs2026`         | CMake-generated Visual Studio projects    |

List the presets with:

```bash
./cx presets
```

## Visual Studio presets

The CMake-generated Visual Studio presets write their projects to:

```text
build\Windows\<arch>\vs2026
build\Windows\<arch>\vs2022
```

## Editor tasks

Build, run, and other commands are also available as editor tasks in:

- Visual Studio Code: `.vscode/tasks.json`
- Zed: `.zed/tasks.json`

## Running

Run the application with:

```bash
./cx run
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
├── tests/                    Unit tests (doctest, fetched with CMake)
├── fuzz/                     Fuzz targets
├── cmake/                    CMake build helpers, including the cx commands
├── main.cpp                  Program entry point
├── CMakeLists.txt            Top-level CMake build definition
├── CMakePresets.json         Configure, build, test, and docs presets
├── cx, cx.ps1, cx.cmd        Helper scripts that wrap CMake
├── grocery.sln, grocery.vcxproj  Hand-written Visual Studio project
├── VERSION                   Release version (must match the release tag)
├── CS210_Project_Three_Input_File.txt
├── frequency.dat
├── Doxyfile                  Doxygen configuration
└── .github/workflows/        CI, CodeQL, docs, release, and Scorecard workflows
```

## Documentation

Generate the HTML documentation locally with:

```bash
./cx docs
```

This runs `cmake --workflow --preset docs`. The generated site is located at:

```text
docs/index.html
```

If Graphviz is installed, the docs build also renders the architecture and class
diagrams. Hand-written `.dot` files are optional. The `diagrams/` folder is not
present in this repository.

The published Doxygen documentation is available online at:

```text
https://dr3mro.github.io/CS-210-12997-Project_Three_Grocery_Frequency_Report/
```

[Open the published Doxygen documentation](https://dr3mro.github.io/CS-210-12997-Project_Three_Grocery_Frequency_Report/)

The `Publish Doxygen documentation` GitHub Actions workflow runs whenever
changes are pushed to the `main` branch. It also supports manual execution
from the repository's **Actions** tab. The workflow runs
`cmake --workflow --preset docs`, packages the generated HTML, and deploys it
to GitHub Pages.

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

| Command         | Description                                     |
| --------------- | ----------------------------------------------- |
| `./cx build`    | Build the default (debug) preset                |
| `./cx run`      | Build and run the application                   |
| `./cx test`     | Run the test suite                              |
| `./cx dev`      | Development workflow                            |
| `./cx fmt`      | Format sources                                  |
| `./cx lint`     | Run static analysis                             |
| `./cx docs`     | Generate Doxygen HTML documentation             |
| `./cx coverage` | Build and report code coverage                  |
| `./cx fuzz`     | Run fuzz targets                                |
| `./cx presets`  | List CMake presets                              |
| `./cx clean`    | Remove build files (`--all` removes everything) |
| `./cx help`     | Show all commands                               |

Run `./cx help` for the full list, which also includes `doctor`, `install`,
`package`, `targets`, and `version`.

## License

This project was created for the SNHU CS-210 course.

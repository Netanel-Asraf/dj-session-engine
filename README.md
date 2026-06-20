# DJ Session Engine

## Overview
The DJ Session Engine is a C++ based audio processing and session management system. It provides a complete backend infrastructure for managing audio tracks, coordinating playlists, handling caching mechanisms, and executing audio mixing operations. Built with a strong emphasis on system performance and resource management, the engine leverages explicit memory management techniques and object-oriented design principles.

## Core Architecture
The system is designed with a layered architecture, enforcing a strict separation of concerns across audio handling, caching, and session coordination.

* **Audio Track Management:** Polymorphic base classes handle varying audio formats (`MP3Track`, `WAVTrack`), ensuring extensible format support.
* **Caching Layer:** Implements a custom Least Recently Used (LRU) Cache policy to optimize memory footprint when loading and mixing heavy audio files.
* **Service Layer:**
    * `DJControllerService`: Handles operations for DJ hardware control interfaces.
    * `DJLibraryService`: Manages the underlying music repository and metadata.
    * `MixingEngineService`: Processes and combines audio tracks seamlessly.
* **Configuration & Parsing:** Dynamically initializes session states through `SessionFileParser` and `ConfigurationManager`.

## Build and Execution

### Prerequisites
* C++ Compiler (g++ recommended)
* Make build automation tool
* Linux/Unix environment (or WSL)

### Compilation
Navigate to the project root and execute the provided Makefile to build the project:

```bash
# Build the standard executable
make

# Build with debug symbols
make debug

# Build an optimized release version
make release
```
## Execution and Command-Line Flags
The engine requires two command-line flags to run properly:
* **`-I`**: Enables interactive manual control.
* **`-A`**: Automatically loads and processes all playlists.

```bash
./bin/dj_manager -I -A

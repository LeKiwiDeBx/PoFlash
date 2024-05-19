# PoFlash

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/travis/username/PoFlash.svg)](https://travis-ci.org/username/PoFlash)
[![Coverage Status](https://coveralls.io/repos/github/username/PoFlash/badge.svg?branch=main)](https://coveralls.io/github/username/PoFlash?branch=main)

## Table of Contents

- [PoFlash](#poflash)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Features](#features)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Configuration](#configuration)
  - [Contributing](#contributing)
  - [License](#license)
  - [Acknowledgements](#acknowledgements)

## Overview

PoFlash is a utility designed to streamline the creation of localization files for translators. Utilizing the gettext system, PoFlash allows for quick generation of POT, PO, and MO files. The program features a graphical user interface built with GTK4 (development version 4.6.9) and is written in C. PoFlash's command-line capabilities can be modified, copied, and pasted into a terminal for execution.

## Features

- Fast generation of POT, PO, and MO files.
- User-friendly graphical interface built with GTK4.
- Command-line commands can be modified and executed directly from the terminal.

## Installation

### Prerequisites

Before you begin, ensure you have met the following requirements:

- **GTK4 (version 4.6.9)**: Install GTK4 from [GTK4 Downloads](https://www.gtk.org/download/).
- **gettext**: Ensure gettext is installed on your system.
- **C Compiler**: Ensure you have a C compiler installed (e.g., GCC).

### Steps

1. Clone the repository:
   ```sh
   git clone https://github.com/username/PoFlash.git
   ```
2. Navigate to the project directory:
   ```sh
   cd PoFlash
   ```
3. Install dependencies:

   For Linux (Debian-based):
   ```sh
   sudo apt-get install libgtk-4-dev gettext
   ```

   For Windows:
   Follow the instructions on [GTK4 for Windows](https://www.gtk.org/download/windows.php) and install gettext using MSYS2:
   ```sh
   pacman -S mingw-w64-x86_64-gettext
   ```

4. Build the project:
   ```sh
   gcc -o poflash src/main.c `pkg-config --cflags --libs gtk4`
   ```

## Usage

To start the PoFlash application, run:

```sh
./poflash
```

### Command-Line Integration

PoFlash allows you to generate and modify command-line commands for creating localization files. These commands can be copied and executed directly in your terminal.

For example, to generate a POT file:
```sh
xgettext -o messages.pot source_file.c
```

For detailed usage instructions, refer to the [documentation](docs/usage.md).

## Configuration

None, is alpha version :)

## Contributing

We welcome contributions! Please see the [contributing guidelines](CONTRIBUTING.md) for more details.

To contribute:

1. Fork the repository.
2. Create a new branch:
   ```sh
   git checkout -b feature/YourFeatureName
   ```
3. Make your changes and commit them:
   ```sh
   git commit -m 'Add some feature'
   ```
4. Push to the branch:
   ```sh
   git push origin feature/YourFeatureName
   ```
5. Open a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgements

- [gettext](https://www.gnu.org/software/gettext/) for providing the internationalization and localization system.
- [GTK4](https://www.gtk.org/) for the graphical toolkit.

Feel free to open an issue or submit a pull request for any improvements or bug fixes. Thank you for your interest and contribution!

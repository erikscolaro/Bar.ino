# Automated Bartending System

Brief description of the project.

## Project Description

Bar.ino is an automated bartending system that uses a touchscreen interface to manage drink recipes and ingredient dispensing. The system allows users to select and prepare cocktails through an interactive GUI, manage ingredients inventory, and execute drink recipes.

## Features

- Interactive touchscreen GUI for drink selection and control
- Recipe management system with customizable drinks
- Ingredient inventory tracking
- Automated drink preparation
- SD card support for storing recipes and ingredient data

## Prerequisites

- PlatformIO IDE (with Visual Studio Code recommended)
- Arduino-compatible microcontroller
- ILI9341 compatible touchscreen display
- SD card reader
- Drink dispensing hardware (pumps, valves, etc.)

## Required Libraries

- Adafruit_BusIO
- Adafruit_GFX
- Adafruit_ILI9341
- Adafruit_TouchScreen
- MCUFRIEND_kbv
- SdFat

## Installation

Clone this repository:

```sh
git clone <repository-url>
```

Open the project in PlatformIO:

1. Launch Visual Studio Code
2. Install the PlatformIO extension if not already installed
3. Open the project folder
4. PlatformIO will automatically detect the project configuration from platformio.ini

Connect your hardware setup to your computer

Build and upload the project:

```sh
platformio run --target upload
```

## Usage

Prepare your SD card with the necessary files:

1. Copy the contents of the `res/` folder to the root of your SD card
2. Ensure `ingredients.csv` and recipes files are properly formatted
3. Insert the SD card into the device's card reader

Power on the system

Use the touchscreen interface to:

- Browse available recipes
- Check ingredient levels
- Prepare drinks
- Manage inventory

## Folder Structure

- `src/` - Source code files
  - `main.cpp` - Main application entry point
  - `Executor/` - Code for controlling physical dispensing mechanisms
  - `Gui/` - User interface implementation
  - `Recipe/` - Recipe management system
  - `Warehouse/` - Ingredient inventory management
- `include/` - Header files
  - `general.settings.h` - General configuration settings
  - `glyphs.h` - Custom GUI graphics
  - `gui.settings.h` - User interface configuration
  - `ingredient.settings.h` - Ingredient-related settings
  - `recipe.settings.h` - Recipe-related settings
  - `warehouse.settings.h` - Inventory management settings
- `lib/` - External libraries
- `res/` - Resources
  - `images/` - GUI images and icons
  - `ingredients.csv` - Ingredient database
  - `recipes/` - Drink recipe files
- `test/` - Test files for development

## Contributing

Contributions to this project are welcome. Please follow these steps:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin feature-name`
5. Submit a pull request

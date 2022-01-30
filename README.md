# Ticker Timescale Swap

This repo is responsible for changing the time scale of ticker data.

This method is one way only, meaning you can only go from a specific time frame (1min) to one that is less specific than that (5min / 10min / 1h / ...).

## Table of contents

 - [Cloning](#cloning)
 - [Build Dependencies](#build-dependencies)
 - [Compiling the application](#compiling-the-application)
 - [Using the Application](#using-the-application)
 - [Verify Data Integrity](#verify-data-integrity)

## Cloning

```bash
git clone https://github.com/Research-Project-Crypto/TickerTimescaleSwap.git --recursive
```

If you forgot to clone recursively you can use the following command:
```bash
git submodule update --init --recursive
```

## Build Dependencies

The following instructions are made for Arch based linux systems but they will give you an idea on how to port it to any other systems.

```bash
pacman -S --noconfirm --needed gcc make
pacman -S --noconfirm --needed premake
```

## Compiling the application

```bash
premake5 gmake2
make config=release
```

## Using the application

### With arguments

|Position|Argument|
|--|--|
|1|Data input folder|
|2|Data output folder|

Example:
```bash
DataPreprocessor data/input data/output
```

## Verify Data Integrity

Included with this project is a python script with which you can verify the binary output data.

```bash
python3 scripts/binary_reader.py
```
`requires numpy`

It will loop over all the cells slowly, this mostly to shortly verify calculation mistakes in the program.

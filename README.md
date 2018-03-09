# A4Tech Bloody Backlight Level Utility

Set backlight level for the A4Tech Bloody mice.

Initial idea, reverse-engineering and all credit goes to the original project: https://github.com/maxmati/a4tech-bloody-linux-driver.

This utility is written in C, built with a simple Makefile, accepts a command line option and includes a systemd unit file.

## Installation

Compile and install on distributions with systemd:

```sh
make
sudo cp bloody /usr/local/bin
sudo cp mousebacklight.service /etc/systemd/system
sudo systemctl enable --now mousebacklight
```

## Usage

```sh
bloody [0|1|2|3]
```

0 turns off backlight completely, while 3 sets it to the maximum brightness.

## Supported Mice

Look into the source code.

## Limitations

This utility changes the backlight level for all supported mice simultaneously.

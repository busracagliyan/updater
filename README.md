## Updater

gtk implementation of apt update command

## Usage

```sh
gcc -Wall `pkg-config --cflags --libs gtk+-3.0` -o updateGtkApp updateGtkApp.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

```

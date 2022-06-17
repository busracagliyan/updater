## Updater

gtk implementation of apt update command in linux.

## Usage

run this command in terminal

```sh
git clone https://github.com/busracagliyan/updater
gcc -Wall `pkg-config --cflags --libs gtk+-3.0` -o updateGtkApp updateGtkApp.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
./updateGtkApp
```

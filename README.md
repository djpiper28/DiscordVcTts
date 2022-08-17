# DiscordVcTts
**Discord Vc Text To Speech**

Do you want some high quality\* and, excellent\* text to speech in 
your discord voice channels? Well look no further\*.

## Features
Click to add text

## Compiling
### Step -1 (git clone?)
```bash
git clone https://github.com/djpiper28/DiscordVcTts --recursive
cd DiscordVcTts
```

### Step 0
Install orca
```bash
# Idk man, stores your id in the exec, dont @ me bruv
export TOKEN=#your token
cd orca-sm # dont ask
cp -f ../config.json ./config.json
make -j
sudo make install
cd .. #?
```

### Step 1
Install pthread, ffmpeg, curl and, jansson. You may also want some 
compilers, maybe some cmake, maybe even a build essential.

```bash
mkdir build
cd build
cmake ..
cmake --build . -j
```


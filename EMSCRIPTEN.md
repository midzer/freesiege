# Emscripten

## Build

```
mkdir build && cd build
emcmake cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Link

```
em++ -flto -O3 *.o */*.o -o index.html -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL_MIXER=2 -sSDL2_MIXER_FORMATS='["ogg"]' -sUSE_SDL_TTF=2 -sASYNCIFY -sENVIRONMENT=web --preload-file anims/ --preload-file sounds/ --preload-file sprites/ --preload-file anims.cfg --preload-file combi.cfg --preload-file musics.cfg --preload-file sprites.cfg --preload-file Swift.ttf -Wl,-u,fileno --closure 1
```

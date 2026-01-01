
# SPACE INVADERS

Kopie og arkadovky v sdl3 knihovne a jazyce C jako projekt do UPR

## Jak program funguje

hrac ovlada lod ve spodni casti obrazovky a brani zemi
nepratele se pohybuji v roji a kratkem intervalu ze strany na stranu (pri dosahnuti okraje se posunuji niz)
bunkry poskytuji hraci ochranu, ale jdou znicit - kazda cast ma vice fazi poskozeni
ufo proleti nad rojem mimozemstanu, proleti nahodne a pri zniceni prinasi vetsi bodovy obnos
hra dokaze ulozit do textoveho souboru az 10 nejlepsich skore (bez upravy jmen :-())


## Ovladani
sipka vlevo - posun doleva
sipka vpravo - posun doprava
mezernik - strelba hrace
enter - start nove hry, restart

## Kompilace

slozka texture s grafikou a fontem musi byt ve stejnem adresari jako program 
### Makefile
```bash
# Makefile v kelvinovi
make

# Spuštění
./main
```

### Pomoci gcc
```bash
# Kompilace
gcc main.c bunker.c enemy.c player.c projectile.c text.c ui.c  -lSDL3 -lSDL3_ttf -g -fsanitize=address -o main

# Spuštění
./main
```

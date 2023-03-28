
# MSYS mingw32
# pacman -Syyu
# pacman -Su
# pacman -S mingw32/mingw-w64-i686-toolchain
# pacman -S mingw32/mingw-w64-i686-gtk3

# cd /z/

#sudo apt-get install gtk+-3.0-dev

# -mwindows not necessary if using gtk+
# gui program = -mwindows WinMain as entry point
# else -mconsole

# disable all warning messages -w
# -fpermissive ( write bad c code )

# GTK_DEBUG=interactive ./out/mod_manager.exe

GCC=i686-w64-mingw32-g++

BINARY= "mod_manager.exe"

OBJS= \
obj/main.o \
obj/page_home.o \
obj/page_analysis.o

#-Wl,-Bstatic
#-fpermissive -w -static-libgcc -static-libstdc++
CFLAGS= `i686-w64-mingw32-pkg-config --cflags gtk+-3.0` -I hdr -fpermissive -w -g
LDFLAGS= `i686-w64-mingw32-pkg-config --libs gtk+-3.0`
#-Wl,--allow-multiple-definition 

$(BINARY): $(OBJS)
	$(GCC) -o out/$(BINARY) $^ $(LDFLAGS)


obj/main.o : src/main.cpp
	$(GCC) -c $^ $(CFLAGS) -o $@

obj/page_home.o : src/page_home.cpp
	$(GCC) -c $^ $(CFLAGS) -o $@

obj/page_analysis.o : src/page_analysis.cpp
	$(GCC) -c $^ $(CFLAGS) -o $@


.phony: clean

clean:
	rm obj/*
	rm out/*
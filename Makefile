CC = gcc
CFLAGS = -Wall -O2
LIBS = -lgmp

OBJS = main.o hybrid.o aes.o rsa.o
KEYGEN_OBJS = keygen.o rsa.o

PKG_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
PKG_LIBS = $(shell pkg-config --libs gtk+-3.0)

all: hybrid keygen gui

hybrid: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

keygen: $(KEYGEN_OBJS)
	$(CC) -o $@ $(KEYGEN_OBJS) $(LIBS)

gui: file_encrypt_gui.o aes.o rsa.o hybrid.o
	$(CC) -o file_encrypt_gui file_encrypt_gui.o aes.o rsa.o hybrid.o $(LIBS) $(PKG_LIBS)

file_encrypt_gui.o: file_encrypt_gui.c
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -c file_encrypt_gui.c -o file_encrypt_gui.o

clean:
	rm -f *.o hybrid keygen file_encrypt_gui *.key encrypted.bin decrypted.txt

# --- Run targets ---

run-keygen:
	./keygen 2048

run-encrypt:
	./hybrid encrypt input.txt encrypted.bin

run-decrypt:
	./hybrid decrypt encrypted.bin decrypted.txt

run-gui:
	./file_encrypt_gui

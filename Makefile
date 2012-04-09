all:: git-credential-gnomekeyring

CC = gcc

GLIB :=			glib-2.0
GNOME_KEYRING :=	gnome-keyring-1

CFLAGS +=	-g -O0 -Wall

CFLAGS +=	$(shell pkg-config --cflags $(GLIB))
CFLAGS +=	$(shell pkg-config --cflags $(GNOME_KEYRING))
LIBS +=		$(shell pkg-config --libs   $(GLIB))
LIBS +=		$(shell pkg-config --libs   $(GNOME_KEYRING))

git-credential-gnomekeyring: git-credential-gnomekeyring.o
	$(CC) -o $@ $< $(LIBS)

git-credential-gnomekeyring.o: git-credential-gnomekeyring.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) git-credential-gnomekeyring git-credential-gnomekeyring.o

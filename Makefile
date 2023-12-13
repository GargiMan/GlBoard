# Makefile IMP Project
# Marek Gergel (xgerge01)

LOGIN := xgerge01

.PHONY: all pdf clean zip tar

all: pdf

pdf:
	pandoc -V geometry:margin=1in README.md -o README.pdf

clean:
	rm -rf $(LOGIN).zip $(LOGIN).tar README.pdf

zip: pdf
	zip -r $(LOGIN).zip LICENSE README* Makefile src config.h GlBoard.ino

tar: pdf
	tar -cf $(LOGIN).tar LICENSE README* Makefile src config.h GlBoard.ino
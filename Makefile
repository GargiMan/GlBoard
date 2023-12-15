# Makefile IMP Project
# Marek Gergel (xgerge01)

LOGIN := xgerge01

.PHONY: all pdf clean zip tar

all: pdf

pdf:
	pandoc -V geometry:margin=1in README.md -o documentation.pdf

clean:
	rm -rf $(LOGIN).zip $(LOGIN).tar documentation.pdf

zip: pdf
	zip -r $(LOGIN).zip LICENSE README* documentation.pdf Makefile src config.h GlBoard.ino GlControl.ino

tar: pdf
	tar -cf $(LOGIN).tar LICENSE README* documentation.pdf Makefile src config.h GlBoard.ino GlControl.ino
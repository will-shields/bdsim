#MOPT=--no-split
MOPT=

all: htmlmanual manual.pdf
                                                                               
htmlmanual: manual.texi mokka.texi
	makeinfo $(MOPT) --html manual.texi

manual.pdf:  manual.texi mokka.texi
	texi2dvi manual.texi
	dvipdf manual.dvi
	rm -f *.aux *.cp *.fn *.toc *.pg *.ky *.log *.vr *.tp

clean:
	rm -f *.aux *.cp *.fn *.toc *.pg *.ky *.log *.vr *.tp \
	manual.pdf bdsim.html

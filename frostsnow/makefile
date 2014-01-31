compiler = gcc
flags = -Wall -Werror --pedantic-errors -rdynamic --std=c99
libraries = -lm
executables = e1_2-2 e1_2-3 p1-1 e2_1-2 e2_1-3 e2_2-2
sources = ${executables:=.c}

solutions = e1_2-2 e1_2-3 p1-1 e2_1-2 e2_1-3 e2_1-4 e2_2-1 e2_2-2 e2_2-3
texs = ${solutions:=.tex}
pdfs = ${solutions:=.pdf}
dvis = ${solutions:=.dvi}
pss = ${solutions:=.ps}
auxs = ${solutions:=.aux}
logs = ${solutions:=.log}

# Compile both programs and PDFs, then clean intermediate files.
all: ${executables} ${pdfs}

# Compile the programs.
#${executables}: ${sources}
${executables}: %: %.c
	${compiler} $< -o $@ ${libraries}

# Compile the PDFs.
${pdfs}: %.pdf: %.ps
	ps2pdf $<

# Compile the PostScripts
${pss}: %.ps: %.dvi
	dvips -R -Poutline $< -o $@

# Compile the DVIs.
${dvis}: %.dvi: %.tex
	latex $<
	latex $<

# Remove all the output files.
clean: tidy
	rm -f ${executables}
	rm -f ${pdfs}

# Remove all intermediate output files.
tidy:
	rm -f ${dvis}
	rm -f ${pss}
	rm -f ${auxs}
	rm -f ${logs}

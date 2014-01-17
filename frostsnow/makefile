compiler = gcc
flags = -Wall -Werror --pedantic-errors -rdynamic --std=c99
libraries = -lm
executables = e1_2-2
sources = ${executables:=.c}

solutions = e1_2-2
texs = ${solutions:=.tex}
pdfs = ${solutions:=.pdf}
dvis = ${solutions:=.dvi}
pss = ${solutions:=.ps}
auxs = ${solutions:=.aux}
logs = ${solutions:=.log}

# Compile both programs and PDFs, then clean intermediate files.
all: ${executables} ${pdfs}

# Compile the programs.
${executables}: ${sources}
	${compiler} $@.c -o $@ ${libraries}

# Compile the PDFs.
${pdfs}: ${pss}
	ps2pdf $<

# Compile the PostScripts
${pss}: ${dvis}
	dvips -R -Poutline $< -o $@

# Compile the DVIs.
${dvis}: ${texs}
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

compiler = gcc
flags = -Wall -Werror --pedantic-errors -rdynamic --std=c99
libraries = -lm

# Code samples.
executables = e1_2-2 e1_2-3 p1-1
executables += e2_1-2 e2_1-3 e2_2-2 e2_3-1 p2-3 p2-4
executables += e4_1-3 e4_2-2 e4_3-6
executables += e6_2-2
sources = ${executables:=.c}

# Typeset answers.
solutions = e1_2-2 e1_2-3 p1-1
solutions += e2_1-1 e2_1-2 e2_1-3 e2_1-4 e2_2-1 e2_2-2 e2_2-3 e2_3-1 e2_3-2 e2_3-4 e2_3-5 e2_3-6 p2-1 p2-2 p2-3 p2-4
solutions += e3_1-1 e3_1-3 e3_1-4 e3_1-6 e3_2-1 e3_2-2 e3_2-6 e3_2-7 p3-1 p3-2 p3-3 p3-4 p3-5 p3-6
solutions += e4_1-1 e4_1-2 e4_1-3 e4_1-4 e4_1-5 e4_2-1 e4_2-2 e4_2-3 e4_2-4 e4_2-5 e4_2-6 e4_2-7 e4_3-1 e4_3-2 e4_3-3 e4_3-4 e4_3-5 e4_3-6 e4_3-7 e4_3-8 e4_3-9 e4_4-1 e4_4-2 e4_4-3 e4_4-4 e4_4-5 e4_4-6
solutions += e6_1-1 e6_1-2 e6_1-3 e6_1-4 e6_1-5 e6_1-6 e6_1-7 e6_2-1 e6_2-2 e6_2-3 e6_2-4 e6_2-5 e6_2-6 e6_3-1 e6_3-2 e6_3-3 e6_4-1 e6_4-2 e6_4-3
solutions += ea_1-1 ea_1-2 ea_1-3 ea_1-7
appendices = appendixa
chapters = chapter01 chapter02 chapter03 chapter04 chapter06
texs = all.tex
pdfs = all.pdf
dvis = all.dvi
pss = all.ps
auxs = all.aux
logs = all.log

# Graphs.
dots = e6_2-1 e6_3-1 e6_4-1
graphs = ${dots:=.png}

# Compile both programs and PDFs, then clean intermediate files.
all: ${executables} ${graphs} ${pdfs}

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
${dvis}: %.dvi: %.tex ${solutions:=.tex} ${chapters:=.tex} ${appendices:=.tex}
	latex $<
	latex $<

# Create the graphs.
${graphs}: %.png: %.dot
	dot -Tpng $< > $@

# Remove all the output files.
clean: tidy
	rm -f ${executables}
	rm -f ${pdfs}
	rm -f ${graphs}

# Remove all intermediate output files.
tidy:
	rm -f ${dvis}
	rm -f ${pss}
	rm -f ${auxs}
	rm -f ${logs}

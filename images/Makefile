dot_files = $(wildcard *.dot)
png_files = $(patsubst %.dot, %.png, $(dot_files))

.PHONY: all

all: $(png_files)

$(png_files): %.png: %.dot
	dot -Tpng -o $@ $<

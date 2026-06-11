
#TARGET=sim65
TARGET=run6502
# this will use VICE
#TARGET=c64

#.SILENT:

all: opt
#all: std

.PHONY: attribs run6502

attribs:
	@chmod 755 maketests
	@chmod 755 makereport.sh
	@chmod 755 maketable.sh

run6502:
# rebuild the run6502 lib to avoid version mismatch
ifeq ($(TARGET),run6502)
	@make -C ../run6502/cc65 clean all
endif

std: attribs
	@./maketests --target $(TARGET) --standard cc65 --show-config --reference

	@./maketests --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --target $(TARGET) --standard cc65 --targetopts -Osir --show-config --targetprogs --run --report
	
#	@./maketests --target $(TARGET) --standard c89 --show-config --reference
	@./maketests --target $(TARGET) --standard c89 --show-config --targetprogs --run --report
	@./maketests --target $(TARGET) --standard c89 --targetopts -Osir --show-config --targetprogs --run --report

#	@./maketests --target $(TARGET) --standard c99 --show-config --reference
	@./maketests --target $(TARGET) --standard c99 --show-config --targetprogs --run --report
	@./maketests --target $(TARGET) --standard c99 --targetopts -Osir --show-config --targetprogs --run --report

#	@./maketests --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
#	@./maketests --target $(TARGET) --standard cc65 --targetopts -Osir --show-config --targettprogs --run --report

	@./maketests --target $(TARGET) --show-config --overview

opt: attribs run6502
	@./maketests --target $(TARGET) --standard cc65 --show-config --reference

	@./maketests --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -O --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Os --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Oi --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Or --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Osi --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Osr --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Oir --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report
	@./maketests --targetopts -Osir --target $(TARGET) --standard cc65 --show-config --targetprogs --run --report

	@./maketests --target $(TARGET) --show-config --overview

clean:
	@rm -f *~
	@rm -f report-*.html
	@rm -f ./arch/c64/*~
	@rm -f ./arch/cc65/*~
	@rm -f ./arch/gcc/*~
	@rm -f ./arch/lcc65/*~
	@rm -f ./arch/run6502/*~
	@rm -f ./arch/sim65/*~
	@rm -f ./arch/sim65816/*~
	@rm -f ./bin/*
	@rm -f ./docs/*~
	@rm -f ./html/*
	@rm -f ./tbin/*
	@rm -f ./temp/*
	@rm -f ./tests/*~
	@rm -f ./util/*~
	@./maketests --clean

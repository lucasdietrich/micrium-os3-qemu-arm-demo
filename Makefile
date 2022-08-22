# Choose the target to build
# BOARD="netduinoplus2"
# BOARD="lm3s6965evb"
BOARD="mps2_an385"

# Tell if disassembly files should be generated (could take additional time)
DO_DISASSEMBLY=0

all: cmake make

cmake:
	cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake \
		-DBOARD=$(BOARD) -S . -B build \
		-DCMAKE_BUILD_TYPE=Debug \
		-DDO_DISASSEMBLY=$(DO_DISASSEMBLY) \
		-DCONFIG_NETWORKING=1 \
		-DCONFIG_FS=1

cmake_lm3s6965evb:
	cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake \
		-DBOARD=lm3s6965evb -S . -B build \
		-DCMAKE_BUILD_TYPE=Debug \
		-DDO_DISASSEMBLY=$(DO_DISASSEMBLY) \
		-DCONFIG_NETWORKING=0 \
		-DCONFIG_FS=0

make:
	make -C build --no-print-directory

qemu: make
	make -C build qemu --no-print-directory

run: make
	make -C build run --no-print-directory

clean:
	rm -rf build
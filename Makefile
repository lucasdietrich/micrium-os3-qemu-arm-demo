all: cmake make

cmake:
	cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build

make:
	make -C build --no-print-directory

qemu: 
	make -C build qemu --no-print-directory

qemu_test:
	qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -semihosting -s -S -nographic -device loader,file=build/ucos3-qemu-app.elf

# -S for stop on startup
# -s flag for debug on port 1234

clean:
	rm -rf build
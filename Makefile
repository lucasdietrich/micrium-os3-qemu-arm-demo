all: cmake make

cmake:
	cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build

make:
	make -C build --no-print-directory

qemu: 
	make -C build qemu --no-print-directory

run: 
	make -C build run --no-print-directory

clean:
	rm -rf build
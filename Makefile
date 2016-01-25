all: telemetry

CMAKE_FLAGS := -DCMAKE_INSTALL_PREFIX=/tmp/usr/local
DEBUG_FLAGS := -DCMAKE_BUILD_TYPE=Debug

install:
	cd build && make install

uninstall:
	cd build && make uninstall

folder:
	@mkdir -p build

telemetry: folder
	cd build && cmake $(CMAKE_FLAGS) ..
	cd build && make

gdb: folder
	cd build && cmake $(DEBUG_FLAGS) ..
	cd build && make
	gdb -tui ./build/example/kft_example -ex "focus next" -ex "set output-radix 16" -ex "set print pretty on"

clean:
	rm -rf build

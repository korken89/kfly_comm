all: telemetry

CMAKE_FLAGS := -DCMAKE_INSTALL_PREFIX=/tmp/usr/local

install:
	cd build && make install

uninstall:
	cd build && make uninstall

telemetry:
	@mkdir -p build
	cd build && cmake $(CMAKE_FLAGS) ..
	cd build && make

clean:
	rm -rf build

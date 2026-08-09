BUILD_DIR ?= build
RELEASE_BUILD_DIR ?= build-release

.PHONY: all configure build test benchmark clean

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

build: configure
	cmake --build $(BUILD_DIR) --parallel

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

benchmark:
	cmake -S . -B $(RELEASE_BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_TESTING=OFF
	cmake --build $(RELEASE_BUILD_DIR) \
		--target orderbook_benchmark \
		--parallel
	./$(RELEASE_BUILD_DIR)/orderbook_benchmark

clean:
	rm -rf $(BUILD_DIR) $(RELEASE_BUILD_DIR)
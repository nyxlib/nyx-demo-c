all:
	nyx-gen nyx-demo.json

	rm -fr ./NYX_DEMO/build/
	mkdir ./NYX_DEMO/build/

	cd ./NYX_DEMO/build/ && cmake .. && make

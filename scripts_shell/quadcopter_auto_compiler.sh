#!/bin/sh

echo "[i] Program update request received"

if [$1 = "-r"]; then
	echo "[i] Relaunching program..."
	pkill quadcopter
	quadcopter
fi

if [$1 = "-rc"]; then
	echo "[i] Killing program..."
	pkill quadcopter
	echo "[i] Downloading latest source changes from github..."
	cd ~/Quadcopter_code
	git pull
	rm -f  Builds/quadcopter
	cd Quadcopter
	echo "[i] Compiling program from sources..."
	gcc * -o ../Builds/quadcopter
	cd ../Builds
	chmod +x quadcopter
	echo "[i] Relaunching Quadcopter program..."
	./quadcopter
fi
		


#!/bin/sh


if [ $1 = "-r" ]; then
	echo "[i] Program update request received"
	echo "[i] Relaunching program..."
	pkill quadcopter
	quadcopter
fi

if [ $1 = "-rc" ]; then
	echo "[i] Program update request received"
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
		

if [ $1 = "-c" ]; then
	echo "[i] Program update request received"
	cd ~/Quadcopter_code
	git pull
	rm -f  Builds/quadcopter
	cd Quadcopter
	echo "[i] Compiling program from sources..."
	gcc * -o ../Builds/quadcopter
	cd ../Builds
	chmod +x quadcopter
	
else
	echo "[e]What do you want to do ?"
fi
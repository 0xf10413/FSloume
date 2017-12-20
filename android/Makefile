ALL_SOURCES=$(wildcard ../game/*.cpp) ../build/rc/*.c ../main.cpp

all: cpplist
	-gradle assembleArmDebug
	-echo "Output in $$PWD/build/outputs/apk/arm/debug/"

cpplist: $(ALL_SOURCES)
	-echo "LOCAL_SRC_FILES := " $(ALL_SOURCES) > CppList.mk

clean:
	-rm .externalNativeBuild -rf
	-rm .gradle -rf

mrproper: clean
	-rm build -rf

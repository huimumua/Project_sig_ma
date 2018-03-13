#!/bin/sh
PATH_BACKUP=$PATH 
zip_base=$PWD

toolchain=$zip_base/../android-toolchain-arm	
PATH=$toolchain/bin:$toolchain/sysroot:$PATH 
echo "Make zipctl:"
make TARGET_PLATFORM=ANDROID_ZIP2 INCLUDE_PATH="-I../../openssl/include/openssl -I../../openssl/include"

#If using DEBUG=1: 
#make TARGET_PLATFORM=ANDROID_ZIP2 DEBUG=1 INCLUDE_PATH="-I../../openssl/include/openssl -I../../openssl/include"

if test $? -eq 0
then
    echo "Make zipctl done."
	if  [ -d demos ] 
	then		
		cd demos
		echo "make demos:"
		make TARGET_PLATFORM=ANDROID_ZIP2		
		if test $? -eq 0
		then
			echo "Make demos done."
		else
			echo "Make demos failed!"
		fi			
	else
		echo "demos directory doesn't exist!"
	fi	
else
    echo "Make zipctl failed!"
fi

PATH=$PATH_BACKUP 

#Go back to initial directory
cd $zip_base
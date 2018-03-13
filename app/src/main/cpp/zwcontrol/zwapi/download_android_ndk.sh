#!/bin/sh

zip_base=$PWD
external_src_dir="$zip_base/external"

cd $external_src_dir

if  [ -e android-ndk-r11c-linux-x86_64.zip ] 
then
    echo "Android ndk is already downloaded."
else
	echo "Wget ndk to directory: $external_src_dir/"
	wget -c http://dl.google.com/android/repository/android-ndk-r11c-linux-x86_64.zip

	if test $? -eq 0
	then
		echo "Wget ndk done."
	else
		echo "Wget ndk failed!"
	fi	
fi

#Go back to initial directory
cd $zip_base
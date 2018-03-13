#!/bin/sh

zip_base=$PWD
external_src_dir="$zip_base/external"
ndk_src=android-ndk-r11c-linux-x86_64.zip
ndk_untar_dir=android-ndk-r11c
ndk_dir=ndk

echo "Z/IP base directory: " $zip_base

echo "Unzip ndk lib: $external_src_dir/$ndk_src"
cd ..
unzip -q "$external_src_dir/$ndk_src"

if test $? -eq 0
then
    echo "Unzip ndk done."

    if [ -L $ndk_dir ] ; then
    echo "Soft link $ndk_dir exist."
    rm $ndk_dir
    fi

    ln -s $ndk_untar_dir $ndk_dir

    echo "Done."
	
	# Currently only arm toolchain is supported.
	$ndk_dir/build/tools/make-standalone-toolchain.sh --platform=android-19 --toolchain=arm-linux-androideabi-4.9 --install-dir=`pwd`/android-toolchain-arm	
else
    echo "Unzip ndk failed!"
fi

#Go back to initial directory
cd $zip_base


















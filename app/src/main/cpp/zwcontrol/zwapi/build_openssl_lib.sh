#!/bin/sh
zip_base=$PWD
start_time=$(date)
external_src_dir="$zip_base/external"
openssl_lib_dir=openssl
openssl_lib_cfg=""
PATH_BACKUP=$PATH

cd ..

openssl_lib_base="$PWD/$openssl_lib_dir"
openssl_cfg_common="--prefix=$openssl_lib_base/install --openssldir=$openssl_lib_base/install/openssl threads"

function display_usage
{
  echo "usage:source build_openssl_lib.sh {linux | beaglebone | raspberry | smp86xx | smp87xx | android | osx} {debug | release} [shared | no-shared]"
  echo "NOTE: arguments in {} are compulsory; those in [] are optional."
  echo "NOTE: default optional setting: no-shared"
  echo "NOTE: shared library build is only supported in linux, beaglebone or raspberry platform"
}

function update_shared_lib_ver
{
	#get openssl major version
	MAJOR="$(grep ^MAJOR Makefile | cut -d = -f 2)"
	#get openssl minor version
	MINOR="$(grep ^MINOR Makefile | cut -d = -f 2)"

	#safety mechanism to reflect major and minor version number to the final shared object file.
	#This avoids generating openssl with default "libssl1.0.0.so" filename, which can be a great source of confusion.
	
	sed -i -e "s/SHLIB_VERSION_NUMBER=1.0.0/SHLIB_VERSION_NUMBER=${MAJOR}.${MINOR}/" Makefile #reflect major version in final shlib name (disambiguation)
	sed -i -e "s/SHLIB_MINOR=0.0/SHLIB_MINOR=${MINOR}/" Makefile #reflect  minor version in final shlib name (disambiguation)
}

opt_shared="no-shared" #optional shared/no-shared argument, default value is no-shared

if [ $# -lt 2 ]
then
    display_usage
elif [ $1 != linux -a $1 != beaglebone -a $1 != raspberry -a $1 != smp86xx -a $1 != smp87xx -a $1 != android -a $1 != osx ]; then
    display_usage
elif [ $2 != debug -a $2 != release ]; then
    display_usage
elif [[ $# -eq 3 ]]  && [[ $3 != shared ]] && [[ $3 != no-shared ]]
then
    display_usage
else
	#shared library option is only supported in linux, beaglebone or raspberry platform
    if [[ $# -eq 3 ]] && [ $1 = "linux" -o $1 = "beaglebone" -o $1 = "raspberry" ]
    then
		opt_shared="$3"
    fi

openssl_cfg_common="$openssl_cfg_common $opt_shared" #fill up the default openssl config

cfg_linux_common="linux-generic32 no-asm"
cfg_linux_le="$cfg_linux_common -DL_ENDIAN $openssl_cfg_common" #embedded LE targets
cfg_linux_be="$cfg_linux_common -DB_ENDIAN $openssl_cfg_common" #embedded BE targets
	   
echo "Configuring for platform $1, $2 version, $opt_shared objects"

#Extra settings for shared library build
extra_args=""
if [ "$opt_shared" = "shared" ]; then
	extra_args="-Wl,--default-symver -Wl,--default-imported-symver"
fi

cd $openssl_lib_base

case $1 in
    linux ) openssl_lib_cfg="$openssl_cfg_common"
            echo "config $openssl_lib_cfg $extra_args"
            ./config "$openssl_lib_cfg $extra_args"
            update_shared_lib_ver
            ;;
    beaglebone ) openssl_lib_cfg=$cfg_linux_le
            echo "Configure $openssl_lib_cfg $extra_args"
            CC=arm-linux-gnueabihf-gcc LD=arm-linux-gnueabihf-ld AR=arm-linux-gnueabihf-ar RANLIB=arm-linux-gnueabihf-ranlib NM=arm-linux-gnueabihf-nm  ./Configure $openssl_lib_cfg $extra_args
            update_shared_lib_ver
            ;;
    raspberry ) openssl_lib_cfg=$cfg_linux_le
            echo "Configure $openssl_lib_cfg $extra_args"
            CC=arm-linux-gnueabihf-gcc LD=arm-linux-gnueabihf-ld AR=arm-linux-gnueabihf-ar RANLIB=arm-linux-gnueabihf-ranlib NM=arm-linux-gnueabihf-nm  ./Configure $openssl_lib_cfg $extra_args
            update_shared_lib_ver
            ;;
    smp86xx ) openssl_lib_cfg=$cfg_linux_le
            CC=mipsel-linux-gcc LD=mipsel-linux-ld AR=mipsel-linux-ar RANLIB=mipsel-linux-ranlib NM=mipsel-linux-nm  ./Configure $openssl_lib_cfg
	    sed -i "s/^CFLAG=\(.*\)/CFLAG=\1 -fPIC/" Makefile
            ;;
    smp87xx ) openssl_lib_cfg=$cfg_linux_le
            CC=arm-none-linux-gnueabi-gcc LD=arm-none-linux-gnueabi-ld AR=arm-none-linux-gnueabi-ar RANLIB=arm-none-linux-gnueabi-ranlib NM=arm-none-linux-gnueabi-nm  ./Configure $openssl_lib_cfg
	    sed -i "s/^CFLAG=\(.*\)/CFLAG=\1 -fPIC/" Makefile
            ;;
    android ) openssl_lib_cfg=$cfg_linux_le
            CC=arm-linux-androideabi-gcc LD=arm-linux-androideabi-ld AR=arm-linux-androideabi-ar RANLIB=arm-linux-androideabi-ranlib NM=arm-linux-androideabi-nm  ./Configure $openssl_lib_cfg
			toolchain=$zip_base/../android-toolchain-arm
			PATH=$toolchain/bin:$toolchain/sysroot:$PATH
            ;;
    osx ) openssl_lib_cfg="darwin-i386-cc --prefix=$openssl_lib_base/install --openssldir=$openssl_lib_base/install/openssl threads no-shared"
            ./Configure $openssl_lib_cfg
	    sed -e "s/^CFLAG=\(.*\)/CFLAG=\1 -fPIE -arch i386/" -i .bak Makefile
            ;;
          * )   echo "Unknown platform: $1"
esac


if test $? -eq 0
then
	echo "configure done."
	# Add PURIFY flag to avoid valgrind to complain about use of uninitialized data
	sed -i "s/^CFLAG=\(.*\)/CFLAG=\1 -DPURIFY/" Makefile
	if [ $2 = debug ]; then
		echo "Modify makefile for debug version"
		if [ $1 = osx ]; then
			sed -e 's/-O3/-g -O0/' -i .bak Makefile
		else
			sed -i -e 's/-O3/-g -O0/' Makefile
		fi
	fi
	echo "make clean"
	rm -rf $openssl_lib_base/install
	make clean
	echo "make"
	make
	if test $? -eq 0
	then
	    echo "make install_sw"
	    make install_sw
	fi
	echo Build start time is $start_time
	echo Build stop time is `date`	
else
	echo "configure failed!"
fi

fi
PATH=$PATH_BACKUP
#Go back to initial directory
cd $zip_base



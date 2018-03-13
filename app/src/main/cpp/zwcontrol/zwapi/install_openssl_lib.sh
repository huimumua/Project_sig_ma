#!/bin/sh
zip_base=$PWD
external_src_dir="$zip_base/external"
openssl_lib_src=openssl-1.0.2l.tar.gz
openssl_lib_untar_dir=openssl-1.0.2l
openssl_lib_dir=openssl
openssl_lib_patch=openssl.patch
echo "Z-ware lib base directory: " $zip_base

cd ..

check_ok=yes
# Check whether the untar directory exists
if [ -d $openssl_lib_untar_dir ]; then
	echo "$openssl_lib_untar_dir exists, remove it? (y/n):"
	read confirm_remove
	if [ "$confirm_remove" = "y" ]; then
		rm -rf $openssl_lib_untar_dir
	else
		echo "Please manually remove $openssl_lib_untar_dir and run this script again."
		echo "Openssl installation failed!"
		check_ok=no
	fi
fi

if [ "$check_ok" = "yes" ]; then
	#Unzip openssl library
	echo "Unzip openssl lib: $external_src_dir/$openssl_lib_src"
	tar zxf "$external_src_dir/$openssl_lib_src"

	if test $? -eq 0
	then
    echo "Unzip openssl lib done."
	#Remove existing openssl soft link
	if [ -L $openssl_lib_dir ] ; then
		echo "Soft link $openssl_lib_dir exists."
		rm $openssl_lib_dir
	fi
	
    ln -s $openssl_lib_untar_dir $openssl_lib_dir
	
	#Apply patch
    echo "Patching openssl lib ..."
#    sed  -i -r "s%unsigned char iv\[EVP_MAX_IV_LENGTH\].*%unsigned char iv\[EVP_MAX_IV_LENGTH\] = {0}; /* to prevent Valgrind uninitialized error*/%" \
#    $openssl_lib_dir/ssl/d1_srvr.c
#    sed  -i -r "s%unsigned char key_name\[16\].*%unsigned char key_name\[16\] = {0}; /* to prevent Valgrind uninitialized error*/%" \
#    $openssl_lib_dir/ssl/d1_srvr.c
    cd $openssl_lib_dir
    patch -p1 < "$external_src_dir/$openssl_lib_patch"

    echo "Done."

	else
		echo "Unzip openssl lib failed!"
	fi
fi

#Go back to initial directory
cd $zip_base


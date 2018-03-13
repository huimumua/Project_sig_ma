#!/bin/bash
dox_base=$PWD
config_file_name=""

cd ..
cd ..
zip_base=$PWD
cd $dox_base

function display_usage
{
echo "usage: ./generate_doc.sh [ce | portal] [min | moderate | max]"
echo "ce and portal are target product documentation"
echo "min, moderate and max are the level of depth of documentation"
}


if [ $# -ne 2 ]; then
display_usage
elif [ $1 != ce -a $1 != portal ]; then
display_usage
elif [ $2 != min -a $2 != moderate -a $2 != max ]; then
display_usage
else
# ---------------------------
# Command parameters are o.k.
# ---------------------------
# Check for doxygen installation
which doxygen > /dev/null
if [ "$?" != "0" ]; then
	echo -n "The doxygen executable is needed.  Do you want to install it? (y/n):"
	read confirm_install_arm
	if [ "$confirm_install_arm" != "y" ]; then
		exit 1
	fi

    sudo apt-get install doxygen
    if [ "$?" != "0" ]; then
            echo "Failed to install doxygen!"
            exit 1
    fi
fi

if [ "$2" = "max" ]; then
    # Check for graphviz installation
    which dot > /dev/null
    if [ "$?" != "0" ]; then
    	echo -n "The graphviz package is needed.  Do you want to install it? (y/n):"
    	read confirm_install_arm
    	if [ "$confirm_install_arm" != "y" ]; then
    		exit 1
    	fi

        sudo apt-get install graphviz
        if [ "$?" != "0" ]; then
                echo "Failed to install graphviz!"
                exit 1
        fi
    fi

fi


output_dir=$dox_base/output_$1

# Get version from project header file "zip_api.h"
zip_major_ver=`cat $zip_base/include/zip_api.h | grep APPL_VERSION |  awk -F" " '{print $3}' `
zip_minor_ver=`cat $zip_base/include/zip_api.h | grep APPL_SUBVERSION |  awk -F" " '{print $3}' `
if [ "$zip_minor_ver" -lt "10" ]; then
    zip_minor_ver="0"$zip_minor_ver
fi

case $2 in
    min ) config_file_name="Doxyfile_headers_"$1.cfg
            sed  -i -r "s%^OUTPUT_DIRECTORY .*%OUTPUT_DIRECTORY = $output_dir%" $config_file_name
            sed  -i -r "s%^INPUT .*%INPUT = $zip_base/include%" $config_file_name
            sed  -i -r "s%^PROJECT_NUMBER .*%PROJECT_NUMBER = $zip_major_ver.$zip_minor_ver%" $config_file_name
            ;;
    moderate ) config_file_name="Doxyfile_no_graph_"$1.cfg
            sed  -i -r "s%^OUTPUT_DIRECTORY .*%OUTPUT_DIRECTORY = $output_dir%" $config_file_name
            sed  -i -r "s%^INPUT .*%INPUT = $zip_base/include \\\%" $config_file_name
            sed  -i -r "s%^PROJECT_NUMBER .*%PROJECT_NUMBER = $zip_major_ver.$zip_minor_ver%" $config_file_name
            ;;
    max ) config_file_name="Doxyfile_call_graph_"$1.cfg
            sed  -i -r "s%^OUTPUT_DIRECTORY .*%OUTPUT_DIRECTORY = $output_dir%" $config_file_name
            sed  -i -r "s%^INPUT .*%INPUT = $zip_base/include \\\%" $config_file_name
            sed  -i -r "s%^PROJECT_NUMBER .*%PROJECT_NUMBER = $zip_major_ver.$zip_minor_ver%" $config_file_name
            sed  -i -r "s%^DOT_PATH .*%DOT_PATH =%" $config_file_name
            ;;
      * )   echo "Unknown param: $2"
            exit 1
            ;;
esac

# Remove output directory
if [ -d $output_dir ]; then
    rm -rf $output_dir
fi

# Start generate documentation
doxygen $config_file_name

if test $? -eq 0
then
	echo "Documentation generated."
else
	echo "Documentation generation failed!"
fi

fi
#Go back to initial directory
cd $dox_base



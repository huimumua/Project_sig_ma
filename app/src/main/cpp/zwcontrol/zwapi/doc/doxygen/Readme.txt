
Manual generation of documentation
----------------------------------
(1) The configuration files are meant to be used by Doxygen version 1.8.8 and higher
(2) Please edit the configuration files to change the directory path according to your installation path.
    The variables in configuration files that need to be edited are : OUTPUT_DIRECTORY, INPUT, PROJECT_NUMBER
    and DOT_PATH (if call graph is required, see Doxyfile_call_graph_*.cfg).
(3) The following is a brief description of each of the configuration files:

Doxyfile_headers_*.cfg -- The minimal configuration that generate documentation based on header files only.

Doxyfile_no_graph_*.cfg -- Moderate configuration that generate documentation based on header and source files.

Doxyfile_call_graph_*.cfg -- Configuration that include call graphs. It may take long time to run.

NOTE: The naming convention is those configuration files with name ending with _ce is meant for CE version;
      files with name ending with _portal is meant for Portal version.

Auto generation of documentation in Ubuntu Linux
------------------------------------------------
(1) Change the script file "generate_doc.sh" to executable by typing the following command in a terminal:
    chmod +x generate_doc.sh
    
(2) Run the script file:
    ./generate_doc.sh [ce | portal] [min | moderate | max]
    where: ce and portal are target product documentation
           min, moderate and max are the level of depth of documentation
           
               

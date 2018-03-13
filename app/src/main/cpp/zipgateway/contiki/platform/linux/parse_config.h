/*
 * parse_config.h
 *
 *  Created on: Jan 14, 2011
 *      Author: aes
 */

#ifndef PARSE_CONFIG_H_
#define PARSE_CONFIG_H_
#include <pkgconfig.h>
//#include "ZIP_Router.h"


#define DATA_DIR INSTALL_LOCALSTATEDIR "/lib/" PACKAGE_TARNAME "/"

/**
 * Platform dependent routine which fills the \ref router_config structure
 * */
void ConfigInit(const char* resPath); //djnakata


void config_update(const char* key, const char* value);
#endif /* PARSE_CONFIG_H_ */

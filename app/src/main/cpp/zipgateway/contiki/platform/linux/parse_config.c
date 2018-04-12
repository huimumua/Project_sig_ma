/*
 * parse_config.c
 *
 *  Created on: Jan 14, 2011
 *      Author: aes
 */
#include "lib/list.h"

/*For strdup! */
#define _SVID_SOURCE

/* for strncasecmp(3) */
#include <strings.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <provisioning_list_files.h>
#include "parse_config.h"

int getopt(int argc, char * const argv[], const char *optstring);

extern char *optarg;
extern int optind, opterr, optopt;

#include <getopt.h>

#include "net/uiplib.h"
#include "ZIP_Router.h"

extern int prog_argc;
extern char** prog_argv;

const char* linux_conf_eeprom_file;
const char* linux_conf_provisioning_cfg_file;
const char* linux_conf_provisioning_list_storage_file;
const char* linux_conf_tun_script;
const char* linux_conf_fin_script;
#ifdef NO_ZW_NVM
const char* linux_conf_nvm_file;
#endif

struct pair
{
  struct pair* next;
  char* key;
  char* val;
};

static struct pair* cfg_values = 0;

static char cfgfile[500] = {0};
//static char* cfgfile = INSTALL_SYSCONFDIR "/" PACKAGE_TARNAME ".cfg"; //djnakata

static char* trim(char* b)
{
  char* e = strrchr(b, '\0'); /* Find the final null */
  while (b < e && isspace(*b)) /* Scan forward */
    ++b;
  while (e > b && isspace(*(e - 1))) /* scan back from end */
    --e;
  if(e != NULL)
    *e = '\0'; /* terminate new string */
  return b;
}

void config_exit()
{
  struct pair* p;

  while (cfg_values)
  {
    p = cfg_values->next;
    free(cfg_values->key);
    free(cfg_values->val);
    free(cfg_values);
    cfg_values = p;
  }

}

//djnakata
static int config_open(char *cfgfile)
{
  FILE * f;
  char line[1024];
  char* k, *v;
  struct pair* p;

  LOG_PRINTF("Opening config file %s\n", cfgfile);
  f = fopen(cfgfile, "r");
  if (!f)
    return 0;
  config_exit();
  while (!feof(f))
  {
    if (!fgets(line, sizeof(line), f))
      break;
    k = strtok(line, "=");
    v = strtok(0, "=");
    if (k && v)
    {
      p = (struct pair*) malloc(sizeof(struct pair));
      p->key = strdup(trim(k));
      p->val = strdup(trim(v));
      p->next = cfg_values;
      cfg_values = p;
    }
  }
  fclose(f);
  return 1;
}

static const char* config_get_val(const char* key, const char* def)
{
  struct pair* p;
  for (p = cfg_values; p != 0; p = p->next)
  {
    if (strcmp(key, p->key) == 0)
      return p->val;
  }
  return def;
}

static int hex2int(char c) {
  if(c >= '0' && c <= '9') {
    return c-'0';
  } else if(c >= 'a' && c <= 'f') {
    return c-'a' + 0xa;
  } else if(c >= 'A' && c <= 'F') {
    return c-'A' + 0xa;
  } else {
    return -1;
  }
}
/*---------------------------------------------------------------------------*/
//djnakata
void ConfigInit(const char* resPath)
{
  char *t;
  int val ;
  const char *s;
  uint8_t *c;
  u8_t *len;
  memset(&cfg, 0, sizeof(cfg));

  sprintf(cfgfile, "%s/%s.cfg", resPath, PACKAGE_TARNAME);

  config_open(cfgfile);
  {
    cfg.serial_port = config_get_val("ZipSerialAPIPortName", "null");

    uiplib_ipaddrconv(config_get_val("ZipUnsolicitedDestinationIp6", "0::0"), &(cfg.unsolicited_dest));
    cfg.unsolicited_port = atoi(config_get_val("ZipUnsolicitedDestinationPort", "4123"));

    uiplib_ipaddrconv(config_get_val("ZipUnsolicitedDestination2Ip6", "0::0"), &(cfg.unsolicited_dest2));
    cfg.unsolicited_port2 = atoi(config_get_val("ZipUnsolicitedDestination2Port", "4123"));

    cfg.serial_log = config_get_val("SerialLog", 0);

    s = config_get_val("ZipPortal", 0);
    if (s)
    {
      strcpy(cfg.portal_url, s);
      ;
    } else
    {
      cfg.portal_url[0] = 0;
    }
    cfg.portal_portno = atoi(config_get_val("ZipPortalPort", "44123"));

    //djnakata
    //cfg.ca_cert = config_get_val("ZipCaCert", DATA_DIR "ca_x509.pem");
    //cfg.cert = config_get_val("ZipCert", DATA_DIR "x509.pem");
    //cfg.priv_key = config_get_val("ZipPrivKey", DATA_DIR "key.pem");
    //linux_conf_eeprom_file = config_get_val("Eepromfile", DATA_DIR "eeprom.dat");
    //linux_conf_provisioning_list_storage_file = config_get_val("PVSStorageFile", PROVISIONING_LIST_STORE_FILENAME_DEFAULT);
    char ca_cert[500] = {0}, cert[500] = {0}, priv_key[500] = {0}, eeprom_file[500] = {0},
            provisioning_list_storage_file[500] = {0};
    sprintf(ca_cert, "%s/Portal.ca_x509.pem", resPath);
    sprintf(cert, "%s/ZIPR.x509_1024.pem", resPath);
    sprintf(priv_key, "%s/ZIPR.key_1024.pem", resPath);
    sprintf(eeprom_file, "%s/eeprom.dat", resPath);
    sprintf(provisioning_list_storage_file, "%s/provisioning_list_store.dat", resPath);

    cfg.ca_cert = strdup(ca_cert);
    cfg.cert = strdup(cert);
    cfg.priv_key = strdup(priv_key);
    linux_conf_eeprom_file = strdup(eeprom_file);
	linux_conf_provisioning_list_storage_file = strdup(provisioning_list_storage_file);
#ifdef NO_ZW_NVM
    linux_conf_nvm_file = config_get_val("Nvmfile", DATA_DIR "nvm.dat");
#endif
    linux_conf_tun_script = config_get_val("TunScript", INSTALL_SYSCONFDIR "/" PACKAGE_TARNAME ".tun");
    linux_conf_fin_script = config_get_val("FinScript", INSTALL_SYSCONFDIR "/" PACKAGE_TARNAME ".fin");

    //djnakata
    char provisioning_cfg_file[500] = {0};
    sprintf(provisioning_cfg_file, "%s/zipgateway_provisioning_list.cfg", resPath);
    linux_conf_provisioning_cfg_file = strdup(provisioning_cfg_file);
    //linux_conf_provisioning_cfg_file = config_get_val("ProvisioningConfigFile", PROVISIONING_CONFIG_FILENAME_DEFAULT);

    cfg.log_level = atoi(config_get_val("LogLevel", "0"));

    uiplib_ipaddrconv(config_get_val("ZipPanIp6", "::"), &(cfg.cfg_pan_prefix));
    uiplib_ipaddrconv(config_get_val("ZipLanIp6", "::"), &(cfg.cfg_lan_addr));
    cfg.cfg_lan_prefix_length = atoi(config_get_val("ZipLanIp6PrefixLength", "64"));

    uiplib_ipaddrconv(config_get_val("ZipLanGw6", "::"), &(cfg.gw_addr));
    cfg.tun_prefix_length = atoi(config_get_val("ZipTunIp6PrefixLength", "128"));
    uiplib_ipaddrconv(config_get_val("ZipTunPrefix", "::"), &(cfg.tun_prefix));

    cfg.ipv4disable = atoi(config_get_val("DebugZipIp4Disable", "0"));
    cfg.client_key_size = atoi(config_get_val("ZipClientKeySize", "1024"));

    cfg.manufacturer_id = strtol(config_get_val("ZipManufacturerID", "0"), NULL, 0);
    cfg.product_id = strtol(config_get_val("ZipProductID", "1"), NULL, 0);
    cfg.product_type = strtol(config_get_val("ZipProductType", "1"), NULL, 0);
    cfg.hardware_version = strtol(config_get_val("ZipHardwareVersion", "1"), NULL, 0);

    cfg.mb_port = uip_htons(atoi(config_get_val("ZipMBPort", "41230")));
    uiplib_ipaddrconv(config_get_val("ZipMBDestinationIp6", "0::0"), &(cfg.mb_destination));

    cfg.mb_conf_mode = DISABLE_MAILBOX; //djnakata //atoi(config_get_val("ZipMBMode", "1"));

    s = config_get_val("ZipPSK", "123456789012345678901234567890AA");
    cfg.psk_len=0;
    while(*s && cfg.psk_len < sizeof(cfg.psk)) {
      val = hex2int(*s++);
      if(val < 0) break;
      cfg.psk[cfg.psk_len]  = ((val) &0xf) <<4;
      val = hex2int(*s++);
      if(val < 0) break;
      cfg.psk[cfg.psk_len] |= (val & 0xf);

      cfg.psk_len++;
    }

    /*Parse extra classes*/
    c = cfg.extra_classes;
    len = &cfg.extra_classes_len;
    *len = 0;
    t = strtok((char*) config_get_val("ExtraClasses", ""), " ");
    while (t)
    {
      if (!strncasecmp(t, "0xF100", 6)) // we found marker
      {
        c = cfg.sec_extra_classes;
        len = &cfg.sec_extra_classes_len;
        *len = 0;
        t = strtok(NULL, " ");
      } else {
        *c = strtoul(t, NULL, 0) & 0xFF;

        if (*c != 0)
           c++;

        (*len)++;
        t = strtok(NULL, " ");
        if (!t)
          break;
      }
    }
    
    *c = 0;

  }
}


void config_update(const char* key, const char* value) {
  FILE * f_in,*f_out;
  char template[256];
  char line[1024];
  char line_bak[1024];

  char* cfgfile_out;
  char* k;
  struct pair* p;
  int sz;
  int key_was_found = 0;
  cfgfile_out = mktemp(template);
  if (strlen(cfgfile) < (256 - (6+1)))
  {
    snprintf(template, strlen(cfgfile) + 7, "%s%s", cfgfile, "XXXXXX");
    cfgfile_out = mktemp(template);
  } else {
    goto fail_nthing;
  }


  f_in = fopen(cfgfile, "r");
  if (!f_in)
  {
    ERR_PRINTF("Could not open config file for updating: %s, Error: %s \n", cfgfile, strerror(errno));
    goto fail_nthing;

  }

  f_out = fopen(cfgfile_out,"w");
  if (!f_out)
  {
    ERR_PRINTF("Could not create temp config file for updating: %s, Error: %s \n", cfgfile_out, strerror(errno));
    goto fail_in;
  }


  while (!feof(f_in))
  {
    if (!fgets(line, sizeof(line), f_in))
      break;

    strncpy(line_bak,line,sizeof(line_bak));
    k = trim(strtok(line_bak, "="));
    //Replace value
    if(strcmp(key,k) ==0) {
      key_was_found=1;
      sz = snprintf(line,sizeof(line),"%s = %s\n", key,value);
    } else {
      sz = strlen(line);
    }
    if(fwrite(line,sz,1,f_out) != 1)
    {
      ERR_PRINTF("Could not write full data to config file, Error: %s \n",strerror(errno));
      goto fail;
    }
  }

  //Append at the end
  if(key_was_found==0) {
    sz = snprintf(line,sizeof(line),"%s = %s\n", key,value);
    if(fwrite(line,sz,1, f_out) != 1)
    {
      ERR_PRINTF("Could not write full data to config file: Error: %s\n", strerror(errno));
      goto fail;
    }
  }
  /*
  ZipUnsolicitedDestinationIp6
  ZipUnsolicitedDestinationIp6
*/
  fclose(f_out);
  fclose(f_in);

  if(rename(cfgfile_out,cfgfile) != 0)
  {
    ERR_PRINTF("Rename of %s to %s failed: Error:%s \n", cfgfile_out, cfgfile, strerror(errno));
    goto fail_nthing;
  }
  return; /* Returning */
fail:
  fclose(f_out);
fail_in:
  fclose(f_in);
fail_nthing:
  ERR_PRINTF("Persisting changes to config file didnt work");
  return;

}

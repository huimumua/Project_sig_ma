package com.askey.firefly.zwave.control.application;


public class ZwaveProvisionList {
	/** Meta data information type*/
	public static final int PL_INFO_TYPE_PROD_TYPE = 0;      ///< Product type
	public static final int PL_INFO_TYPE_PROD_ID  =  1 ;     ///< Product ID
	public static final int PL_INFO_TYPE_INC_INTV =  2 ;     ///< Smart Start inclusion request interval used by the node
	public static final int PL_INFO_TYPE_UUID16   =  3  ;    ///< UUID assigned to the node
	public static final int PL_INFO_TYPE_NAME     =  0x32;   ///< Device name
	public static final int PL_INFO_TYPE_LOC      =  0x33;   ///< Device location
	public static final int PL_INFO_TYPE_INCL_STS =  0x34;   ///< Inclusion status of the provisioning list entry
	public static final int PL_INFO_TYPE_S2_GRNT  =  0x35 ;  ///< S2 keys to be granted
	public static final int PL_INFO_TYPE_BOOT_MODE = 0x36 ;  ///< Bootstrapping mode
	public static final int PL_INFO_TYPE_NW_STS   =  0x37 ;  ///< Network status of the provisioning list entry

	public static final int PL_MAX_INFO_TYPE      =  10 ;    ///< Maximum number of defined meta data information types

	public static final int PL_MAX_NAME          =   62 ;    ///< Maximum name length in bytes
	public static final int PL_MAX_LOC          =    62  ;   ///< Maximum location length in bytes

	public static final int PL_UUID_LEN          =   16  ;   ///< UUID length

	/** Inclusion status of the provisioning list entry */
	public static final int PL_INCL_STS_PENDING  =   0 ;     ///< The node will be included in the network when it issues SmartStart inclusion requests.
	public static final int PL_INCL_STS_PASSIVE  =   2 ;     /**< The node is in the Provisioning List but it has been decided by the supporting or
	                                            controlling node that the node is unlikely to issue SmartStart inclusion requests in
	                                            the near future.
	                                            SmartStart Inclusion requests will be ignored by the Z/IP Gateway.
	                                            All entries with this status MUST be updated to the “Pending” status (PL_INCL_STS_PENDING) when a
	                                            Provisioning List Iteration Get Command is received.*/
	public static final int PL_INCL_STS_IGNORED  =  3;      /**< SmartStart inclusion requests sent by the node in the Provisioning List entry will be
	                                                        ignored until the status is changed again by a Z/IP Client or controlling node.*/

	/** Network status of the provisioning list entry */
	public static final int PL_NW_STS_NOT_INCL   =   0;      ///< The node in the Provisioning List is not currently included (added) in the network.
	public static final int PL_NW_STS_ADDED      =   1 ;     ///< The node in the Provisioning List is included in the network and is functional.
	public static final int PL_NW_STS_FAILED     =   2 ;     /**< The node in the Provisioning List has been included in the Z-Wave network
	                                                              but is now marked as failing. */

	/** Bootstrapping mode */
	public static final int PL_BOOT_MODE_S2      =   0  ;    ///< The node MUST manually be set to Learn Mode and follow the S2 bootstrapping instructions (if any).
	public static final int PL_BOOT_MODE_SMART_STRT = 1;



    /*******************************************************************/

    public int                 type;                //pl info type  
    public ProvisionListInfo   stProvisionList;     //pl detail info  
      
    public class ProvisionListInfo {
        public String           name;     // device name
        public String           loc;      // device location
        /**< For type PL_INFO_TYPE_INC_INTV; Smart Start inclusion request interval in unit of 128 seconds.
             This field must have value ranging from 5 to 99. */
        public int              interval;

        /*
            Inclusion status of the provisioning list entry
            #define PL_INCL_STS_PENDING     0     // init state
			#define PL_INCL_STS_PASSIVE     2      
			#define PL_INCL_STS_IGNORED     3
		*/
		public int inclusionState;

		/*  Bootstrapping mode 
			#define PL_BOOT_MODE_S2         0
			#define PL_BOOT_MODE_SMART_STRT 1
		*/
        public int             boot_mode;

		public class ProductTypeInfo{
        	public int     generic_cls;    /**< Generic device class*/
	        public int     specific_cls;	/**< Specific device class*/
            public int     icon_type;      /**< Installer icon type*/

            public ProductTypeInfo()
            {
            	this.generic_cls = 0;
            	this.specific_cls = 0;
            	this.icon_type = 0;
            }
        }
        public ProductTypeInfo  pti; // product type info
        
        public class ProductIdInfo{
        	public int    manf_id;        /**< Manufacturer ID*/
        	public int    prod_type;	  /**< Product type*/
        	public int    prod_id;        /**< Product ID*/
        	public int    app_ver;        /**< Application version*/
        	public int    app_sub_ver;    /**< Application sub version*/

        	public ProductIdInfo(){
        		this.manf_id = 0;
        		this.prod_type = 0;
        		this.prod_id = 0;
        		this.app_ver = 0;
        		this.app_sub_ver = 0; 
        	}
        }
        public ProductIdInfo pii;         // product id info
        

        public ProvisionListInfo(){
        	this.name = null;
        	this.loc = null;
        	this.inclusionState = PL_INCL_STS_PENDING; // default status: pending will auto start incl
        	this.interval = 100;
        	this.boot_mode = PL_BOOT_MODE_SMART_STRT;  // default mode: smart start
        	this.pti = new ProductTypeInfo();
        	this.pii = new ProductIdInfo();
        }
    };

    public ZwaveProvisionList()
    {
    	this.type = 0;
    	this.stProvisionList = new ProvisionListInfo();
    }

    public void setType(int type)
    {
    	this.type = type;
    }

    public void setBootMode(int bootmode){
    	this.stProvisionList.boot_mode = bootmode;
    }

    public void setInclusionState(int state){
    	this.stProvisionList.inclusionState = state;
    }
};  
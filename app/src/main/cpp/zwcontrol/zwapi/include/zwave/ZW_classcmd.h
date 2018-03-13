// Generated on: 09.03.2016 16:58:43
/*******************************  ZW_classcmd.h  *******************************
 *           #######
 *           ##  ##
 *           #  ##    ####   #####    #####  ##  ##   #####
 *             ##    ##  ##  ##  ##  ##      ##  ##  ##
 *            ##  #  ######  ##  ##   ####   ##  ##   ####
 *           ##  ##  ##      ##  ##      ##   #####      ##
 *          #######   ####   ##  ##  #####       ##  #####
 *                                           #####
 *          Z-Wave, the wireless language.
 *
 *              Copyright (c) 2001
 *              Zensys A/S
 *              Denmark
 *
 *              All Rights Reserved
 *
 *    This source file is subject to the terms and conditions of the
 *    Zensys Software License Agreement which restricts the manner
 *    in which it may be used.
 *
 *---------------------------------------------------------------------------
 *
 * Description: Device and command class types and definitions.
 *
 *              Be aware that temporary types and definitions can be present
 *              in this header file. Only the types and definitions found in
 *              the latest version of the Z-Wave Device Class Specification
 *              (doc. nr. 9031002xx) is valid.
 *
 * Author: Jшrgen Franck
 *
 * Last Changed By:  $Author$
 * Revision:         $Revision$
 * Last Changed:     $Date$
 *
 ****************************************************************************/
#ifndef _ZW_CLASSCMD_H_
#define _ZW_CLASSCMD_H_

/****************************************************************************
 *    TYPES and DEFINITIONS
 ***************************************************************************/
/************ Basic Device Class identifiers **************/
#define BASIC_TYPE_CONTROLLER                                                            0x01 /*Node is a portable controller */
#define BASIC_TYPE_ROUTING_SLAVE                                                         0x04 /*Node is a slave with routing capabilities*/
#define BASIC_TYPE_SLAVE                                                                 0x03 /*Node is a slave*/
#define BASIC_TYPE_STATIC_CONTROLLER                                                     0x02 /*Node is a static controller*/


/***** Generic and Specific Device Class identifiers ******/
/* Device class Av Control Point */
#define GENERIC_TYPE_AV_CONTROL_POINT                                                    0x03 /*AV Control Point*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_DOORBELL                                                           0x12
#define SPECIFIC_TYPE_SATELLITE_RECEIVER                                                 0x04 /*Satellite Receiver*/
#define SPECIFIC_TYPE_SATELLITE_RECEIVER_V2                                              0x11 /*Satellite Receiver V2*/

/* Device class Display */
#define GENERIC_TYPE_DISPLAY                                                             0x04
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SIMPLE_DISPLAY                                                     0x01 /*Display (simple) Device Type*/

/* Device class Entry Control */
#define GENERIC_TYPE_ENTRY_CONTROL                                                       0x40 /*Entry Control*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_DOOR_LOCK                                                          0x01 /*Door Lock*/
#define SPECIFIC_TYPE_ADVANCED_DOOR_LOCK                                                 0x02 /*Advanced Door Lock*/
#define SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK                                            0x03 /*Door Lock (keypad –lever) Device Type*/
#define SPECIFIC_TYPE_SECURE_KEYPAD_DOOR_LOCK_DEADBOLT                                   0x04 /*Door Lock (keypad – deadbolt) Device Type*/
#define SPECIFIC_TYPE_SECURE_DOOR                                                        0x05 /*Barrier Operator Specific Device Class*/
#define SPECIFIC_TYPE_SECURE_GATE                                                        0x06 /*Barrier Operator Specific Device Class*/
#define SPECIFIC_TYPE_SECURE_BARRIER_ADDON                                               0x07 /*Barrier Operator Specific Device Class*/
#define SPECIFIC_TYPE_SECURE_BARRIER_OPEN_ONLY                                           0x08 /*Barrier Operator Specific Device Class*/
#define SPECIFIC_TYPE_SECURE_BARRIER_CLOSE_ONLY                                          0x09 /*Barrier Operator Specific Device Class*/
#define SPECIFIC_TYPE_SECURE_LOCKBOX                                                     0x0A /*SDS12724*/
#define SPECIFIC_TYPE_SECURE_KEYPAD                                                      0x0B

/* Device class Generic Controller */
#define GENERIC_TYPE_GENERIC_CONTROLLER                                                  0x01 /*Remote Controller*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_PORTABLE_REMOTE_CONTROLLER                                         0x01 /*Remote Control (Multi Purpose) Device Type*/
#define SPECIFIC_TYPE_PORTABLE_SCENE_CONTROLLER                                          0x02 /*Portable Scene Controller*/
#define SPECIFIC_TYPE_PORTABLE_INSTALLER_TOOL                                            0x03
#define SPECIFIC_TYPE_REMOTE_CONTROL_AV                                                  0x04 /*Remote Control (AV) Device Type*/
#define SPECIFIC_TYPE_REMOTE_CONTROL_SIMPLE                                              0x06 /*Remote Control (Simple) Device Type*/

/* Device class Meter */
#define GENERIC_TYPE_METER                                                               0x31 /*Meter*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SIMPLE_METER                                                       0x01 /*Sub Energy Meter Device Type*/
#define SPECIFIC_TYPE_ADV_ENERGY_CONTROL                                                 0x02 /*Whole Home Energy Meter (Advanced) Device Type*/
#define SPECIFIC_TYPE_WHOLE_HOME_METER_SIMPLE                                            0x03 /*Whole Home Meter (Simple) Device Type*/

/* Device class Meter Pulse */
#define GENERIC_TYPE_METER_PULSE                                                         0x30 /*Pulse Meter*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/

/* Device class Non Interoperable */
#define GENERIC_TYPE_NON_INTEROPERABLE                                                   0xFF /*Non interoperable*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/

/* Device class Repeater Slave */
#define GENERIC_TYPE_REPEATER_SLAVE                                                      0x0F /*Repeater Slave*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_REPEATER_SLAVE                                                     0x01 /*Basic Repeater Slave*/
#define SPECIFIC_TYPE_VIRTUAL_NODE                                                       0x02

/* Device class Security Panel */
#define GENERIC_TYPE_SECURITY_PANEL                                                      0x17
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ZONED_SECURITY_PANEL                                               0x01

/* Device class Semi Interoperable */
#define GENERIC_TYPE_SEMI_INTEROPERABLE                                                  0x50 /*Semi Interoperable*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ENERGY_PRODUCTION                                                  0x01 /*Energy Production*/

/* Device class Sensor Alarm */
#define GENERIC_TYPE_SENSOR_ALARM                                                        0xA1
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ADV_ZENSOR_NET_ALARM_SENSOR                                        0x05
#define SPECIFIC_TYPE_ADV_ZENSOR_NET_SMOKE_SENSOR                                        0x0A
#define SPECIFIC_TYPE_BASIC_ROUTING_ALARM_SENSOR                                         0x01
#define SPECIFIC_TYPE_BASIC_ROUTING_SMOKE_SENSOR                                         0x06
#define SPECIFIC_TYPE_BASIC_ZENSOR_NET_ALARM_SENSOR                                      0x03
#define SPECIFIC_TYPE_BASIC_ZENSOR_NET_SMOKE_SENSOR                                      0x08
#define SPECIFIC_TYPE_ROUTING_ALARM_SENSOR                                               0x02
#define SPECIFIC_TYPE_ROUTING_SMOKE_SENSOR                                               0x07
#define SPECIFIC_TYPE_ZENSOR_NET_ALARM_SENSOR                                            0x04
#define SPECIFIC_TYPE_ZENSOR_NET_SMOKE_SENSOR                                            0x09
#define SPECIFIC_TYPE_ALARM_SENSOR                                                       0x0B /*Sensor (Alarm) Device Type*/

/* Device class Sensor Binary */
#define GENERIC_TYPE_SENSOR_BINARY                                                       0x20 /*Binary Sensor*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ROUTING_SENSOR_BINARY                                              0x01 /*Routing Binary Sensor*/

/* Device class Sensor Multilevel */
#define GENERIC_TYPE_SENSOR_MULTILEVEL                                                   0x21 /*Multilevel Sensor*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ROUTING_SENSOR_MULTILEVEL                                          0x01 /*Sensor (Multilevel) Device Type*/
#define SPECIFIC_TYPE_CHIMNEY_FAN                                                        0x02

/* Device class Static Controller */
#define GENERIC_TYPE_STATIC_CONTROLLER                                                   0x02 /*Static Controller*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_PC_CONTROLLER                                                      0x01 /*Central Controller Device Type*/
#define SPECIFIC_TYPE_SCENE_CONTROLLER                                                   0x02 /*Scene Controller*/
#define SPECIFIC_TYPE_STATIC_INSTALLER_TOOL                                              0x03
#define SPECIFIC_TYPE_SET_TOP_BOX                                                        0x04 /*Set Top Box Device Type*/
#define SPECIFIC_TYPE_SUB_SYSTEM_CONTROLLER                                              0x05 /*Sub System Controller Device Type*/
#define SPECIFIC_TYPE_TV                                                                 0x06 /*TV Device Type*/
#define SPECIFIC_TYPE_GATEWAY                                                            0x07 /*Gateway Device Type*/

/* Device class Switch Binary */
#define GENERIC_TYPE_SWITCH_BINARY                                                       0x10 /*Binary Switch*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_POWER_SWITCH_BINARY                                                0x01 /*On/Off Power Switch Device Type*/
#define SPECIFIC_TYPE_SCENE_SWITCH_BINARY                                                0x03 /*Binary Scene Switch*/
#define SPECIFIC_TYPE_POWER_STRIP                                                        0x04 /*Power Strip Device Type*/
#define SPECIFIC_TYPE_SIREN                                                              0x05 /*Siren Device Type*/
#define SPECIFIC_TYPE_VALVE_OPEN_CLOSE                                                   0x06 /*Valve (open/close) Device Type*/
#define SPECIFIC_TYPE_COLOR_TUNABLE_BINARY                                               0x02
#define SPECIFIC_TYPE_IRRIGATION_CONTROLLER                                              0x07

/* Device class Switch Multilevel */
#define GENERIC_TYPE_SWITCH_MULTILEVEL                                                   0x11 /*Multilevel Switch*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_CLASS_A_MOTOR_CONTROL                                              0x05 /*Window Covering No Position/Endpoint Device Type*/
#define SPECIFIC_TYPE_CLASS_B_MOTOR_CONTROL                                              0x06 /*Window Covering Endpoint Aware Device Type*/
#define SPECIFIC_TYPE_CLASS_C_MOTOR_CONTROL                                              0x07 /*Window Covering Position/Endpoint Aware Device Type*/
#define SPECIFIC_TYPE_MOTOR_MULTIPOSITION                                                0x03 /*Multiposition Motor*/
#define SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL                                            0x01 /*Light Dimmer Switch Device Type*/
#define SPECIFIC_TYPE_SCENE_SWITCH_MULTILEVEL                                            0x04 /*Multilevel Scene Switch*/
#define SPECIFIC_TYPE_FAN_SWITCH                                                         0x08 /*Fan Switch Device Type*/
#define SPECIFIC_TYPE_COLOR_TUNABLE_MULTILEVEL                                           0x02

/* Device class Switch Remote */
#define GENERIC_TYPE_SWITCH_REMOTE                                                       0x12 /*Remote Switch*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SWITCH_REMOTE_BINARY                                               0x01 /*Binary Remote Switch*/
#define SPECIFIC_TYPE_SWITCH_REMOTE_MULTILEVEL                                           0x02 /*Multilevel Remote Switch*/
#define SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_BINARY                                        0x03 /*Binary Toggle Remote Switch*/
#define SPECIFIC_TYPE_SWITCH_REMOTE_TOGGLE_MULTILEVEL                                    0x04 /*Multilevel Toggle Remote Switch*/

/* Device class Switch Toggle */
#define GENERIC_TYPE_SWITCH_TOGGLE                                                       0x13 /*Toggle Switch*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SWITCH_TOGGLE_BINARY                                               0x01 /*Binary Toggle Switch*/
#define SPECIFIC_TYPE_SWITCH_TOGGLE_MULTILEVEL                                           0x02 /*Multilevel Toggle Switch*/

/* Device class Thermostat */
#define GENERIC_TYPE_THERMOSTAT                                                          0x08 /*Thermostat*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SETBACK_SCHEDULE_THERMOSTAT                                        0x03 /*Setback Schedule Thermostat*/
#define SPECIFIC_TYPE_SETBACK_THERMOSTAT                                                 0x05 /*Thermostat (Setback) Device Type*/
#define SPECIFIC_TYPE_SETPOINT_THERMOSTAT                                                0x04
#define SPECIFIC_TYPE_THERMOSTAT_GENERAL                                                 0x02 /*Thermostat General*/
#define SPECIFIC_TYPE_THERMOSTAT_GENERAL_V2                                              0x06 /*Thermostat (HVAC) Device Type*/
#define SPECIFIC_TYPE_THERMOSTAT_HEATING                                                 0x01 /*Thermostat Heating*/

/* Device class Ventilation */
#define GENERIC_TYPE_VENTILATION                                                         0x16
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_RESIDENTIAL_HRV                                                    0x01

/* Device class Window Covering */
#define GENERIC_TYPE_WINDOW_COVERING                                                     0x09 /*Window Covering*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SIMPLE_WINDOW_COVERING                                             0x01 /*Simple Window Covering Control*/

/* Device class Zip Node */
#define GENERIC_TYPE_ZIP_NODE                                                            0x15
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_ZIP_ADV_NODE                                                       0x02
#define SPECIFIC_TYPE_ZIP_TUN_NODE                                                       0x01

/* Device class Wall Controller */
#define GENERIC_TYPE_WALL_CONTROLLER                                                     0x18
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_BASIC_WALL_CONTROLLER                                              0x01 /*Wall Controller Device Type*/

/* Device class Network Extender */
#define GENERIC_TYPE_NETWORK_EXTENDER                                                    0x05 /*Network Extender Generic Device Class*/
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_SECURE_EXTENDER                                                    0x01 /*Specific Device Secure Extender*/

/* Device class Appliance */
#define GENERIC_TYPE_APPLIANCE                                                           0x06
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class Not Used*/
#define SPECIFIC_TYPE_GENERAL_APPLIANCE                                                  0x01
#define SPECIFIC_TYPE_KITCHEN_APPLIANCE                                                  0x02
#define SPECIFIC_TYPE_LAUNDRY_APPLIANCE                                                  0x03

/* Device class Sensor Notification */
#define GENERIC_TYPE_SENSOR_NOTIFICATION                                                 0x07
#define SPECIFIC_TYPE_NOT_USED                                                           0x00 /*Specific Device Class not used*/
#define SPECIFIC_TYPE_NOTIFICATION_SENSOR                                                0x01



/************* Z-Wave+ Role Type identifiers **************/
#define ROLE_TYPE_CONTROLLER_CENTRAL_STATIC                                              0x00
#define ROLE_TYPE_CONTROLLER_SUB_STATIC                                                  0x01
#define ROLE_TYPE_CONTROLLER_PORTABLE                                                    0x02
#define ROLE_TYPE_CONTROLLER_PORTABLE_REPORTING                                          0x03
#define ROLE_TYPE_SLAVE_PORTABLE                                                         0x04
#define ROLE_TYPE_SLAVE_ALWAYS_ON                                                        0x05
#define ROLE_TYPE_SLAVE_SLEEPING_REPORTING                                               0x06
#define ROLE_TYPE_SLAVE_SLEEPING_LISTENING                                               0x07


/************* Z-Wave+ Icon Type identifiers **************/
/* The Z-Wave+ Icon Types defined in this section is the  */
/* work of the Z-Wave Alliance.                           */
/* The most current list of Z-Wave+ Icon Types may be     */
/* found at Z-Wave Alliance web site along with           */
/* sample icons.                                          */
/* New Z-Wave+ Icon Types may be requested from the       */
/* Z-Wave Alliance.                                       */
/**********************************************************/
#define ICON_TYPE_UNASSIGNED                                                 0x0000   //MUST NOT be used by any product

#define ICON_TYPE_GENERIC_CENTRAL_CONTROLLER                                 0x0100   //Central Controller Device Type

#define ICON_TYPE_GENERIC_DISPLAY_SIMPLE                                     0x0200   //Display Simple Device Type

#define ICON_TYPE_GENERIC_DOOR_LOCK_KEYPAD                                   0x0300   //Door Lock Keypad  Device Type

#define ICON_TYPE_GENERIC_FAN_SWITCH                                         0x0400   //Fan Switch  Device Type

#define ICON_TYPE_GENERIC_GATEWAY                                            0x0500   //Gateway  Device Type

#define ICON_TYPE_GENERIC_LIGHT_DIMMER_SWITCH                                0x0600   //Light Dimmer Switch  Device Type
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_PLUGIN                        0x0601	  //Light Dimmer, implemented as a plugin device
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_WALL_OUTLET	                 0x0602	  //Light Dimmer, implemented as a wall outlet
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_CEILING_OUTLET	             0x0603	  //Light Dimmer, implemented as a ceiling outlet
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_WALL_LAMP     	             0x0604	  //Relay device, implemented as a wall mounted lamp
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_LAMP_POST_HIGH	             0x0605	  //Relay device, implemented as a ceiling outlet
#define ICON_TYPE_SPECIFIC_LIGHT_DIMMER_SWITCH_LAMP_POST_LOW	             0x0606	  //Relay device, implemented as a ceiling outlet

#define ICON_TYPE_GENERIC_ON_OFF_POWER_SWITCH                                0x0700   //On/Off Power Switch  Device Type
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_PLUGIN	                     0x0701	  //Relay device, implemented as a plugin device
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_WALL_OUTLET	                 0x0702	  //Relay device, implemented as a wall outlet
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_CEILING_OUTLET	             0x0703	  //Relay device, implemented as a ceiling outlet
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_WALL_LAMP	                 0x0704	  //Relay device, implemented as a wall mounted lamp
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_LAMP_POST_HIGH	             0x0705	  //Relay device, implemented as a ceiling outlet
#define ICON_TYPE_SPECIFIC_ON_OFF_POWER_SWITCH_LAMP_POST_LOW	             0x0706	  //Relay device, implemented as a ceiling outlet

#define ICON_TYPE_GENERIC_POWER_STRIP                                        0x0800   //Power Strip  Device Type
#define ICON_TYPE_SPECIFIC_POWER_STRIP_INDIVIDUAL_OUTLET	                 0x08FF	  //Individual outlet of a power strip for showing outlets in exploded view

#define ICON_TYPE_GENERIC_REMOTE_CONTROL_AV                                  0x0900   //Remote Control AV  Device Type

#define ICON_TYPE_GENERIC_REMOTE_CONTROL_MULTI_PURPOSE                       0x0A00   //Remote Control Multi Purpose Device Type

#define ICON_TYPE_GENERIC_REMOTE_CONTROL_SIMPLE                              0x0B00   //Remote Control Simple Device Type
#define ICON_TYPE_SPECIFIC_REMOTE_CONTROL_SIMPLE_KEYFOB                      0x0B01   //Remote Control Simple Device Type (Key fob)

#define ICON_TYPE_GENERIC_SENSOR_NOTIFICATION                                0x0C00   //Sensor Notification Device Type
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_SMOKE_ALARM                   0x0C01   //Sensor Notification Device Type (Notification type Smoke Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_CO_ALARM                      0x0C02   //Sensor Notification Device Type (Notification type CO Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_CO2_ALARM                     0x0C03   //Sensor Notification Device Type (Notification type CO2 Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_HEAT_ALARM                    0x0C04   //Sensor Notification Device Type (Notification type Heat Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_WATER_ALARM                   0x0C05   //Sensor Notification Device Type (Notification type Water Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_ACCESS_CONTROL                0x0C06   //Sensor Notification Device Type (Notification type Access Control)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_HOME_SECURITY                 0x0C07   //Sensor Notification Device Type (Notification type Home Security)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_POWER_MANAGEMENT              0x0C08   //Sensor Notification Device Type (Notification type Power Management)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_SYSTEM                        0x0C09   //Sensor Notification Device Type (Notification type System)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_EMERGENCY_ALARM               0x0C0A   //Sensor Notification Device Type (Notification type Emergency Alarm)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_CLOCK                         0x0C0B   //Sensor Notification Device Type (Notification type Clock)
#define ICON_TYPE_SPECIFIC_SENSOR_NOTIFICATION_MULTIDEVICE                   0x0CFF   //Sensor Notification Device Type (Bundled Notification functions)

#define ICON_TYPE_GENERIC_SENSOR_MULTILEVEL                                  0x0D00   //Sensor Multilevel Device Type
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_AIR_TEMPERATURE                 0x0D01   //Sensor Multilevel Device Type (Sensor type Air Temperature)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_GENERAL_PURPOSE_VALUE           0x0D02   //Sensor Multilevel Device Type (Sensor type General Purpose Value)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_LUMINANCE                       0x0D03   //Sensor Multilevel Device Type (Sensor type Luminance)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_POWER                           0x0D04   //Sensor Multilevel Device Type (Sensor type Power)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_HUMIDITY                        0x0D05   //Sensor Multilevel Device Type (Sensor type Humidity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_VELOCITY                        0x0D06   //Sensor Multilevel Device Type (Sensor type Velocity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_DIRECTION                       0x0D07   //Sensor Multilevel Device Type (Sensor type Direction)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ATMOSPHERIC_PRESSURE            0x0D08   //Sensor Multilevel Device Type (Sensor type Atmospheric Pressure)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_BAROMETRIC_PRESSURE             0x0D09   //Sensor Multilevel Device Type (Sensor type Barometric Pressure)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_SOLOR_RADIATION                 0x0D0A   //Sensor Multilevel Device Type (Sensor type Solar Radiation)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_DEW_POINT                       0x0D0B   //Sensor Multilevel Device Type (Sensor type Dew Point)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_RAIN_RATE                       0x0D0C   //Sensor Multilevel Device Type (Sensor type Rain Rate)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_TIDE_LEVEL                      0x0D0D   //Sensor Multilevel Device Type (Sensor type Tide Level)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_WEIGHT                          0x0D0E   //Sensor Multilevel Device Type (Sensor type Weight)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_VOLTAGE                         0x0D0F   //Sensor Multilevel Device Type (Sensor type Voltage)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_CURRENT                         0x0D10   //Sensor Multilevel Device Type (Sensor type Current)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_CO2_LEVEL                       0x0D11   //Sensor Multilevel Device Type (Sensor type CO2 Level)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_AIR_FLOW                        0x0D12   //Sensor Multilevel Device Type (Sensor type Air Flow)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_TANK_CAPACITY                   0x0D13   //Sensor Multilevel Device Type (Sensor type Tank Capacity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_DISTANCE                        0x0D14   //Sensor Multilevel Device Type (Sensor type Distance)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ANGLE_POSITION                  0x0D15   //Sensor Multilevel Device Type (Sensor type Angle Position)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ROTATION                        0x0D16   //Sensor Multilevel Device Type (Sensor type Rotation)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_WATER_TEMPERATURE               0x0D17   //Sensor Multilevel Device Type (Sensor type Water Temperature)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_SOIL_TEMPERATURE                0x0D18   //Sensor Multilevel Device Type (Sensor type Soil Temperature)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_SEISMIC_INTENSITY               0x0D19   //Sensor Multilevel Device Type (Sensor type Seismic Intensity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_SEISMIC_MAGNITUDE               0x0D1A   //Sensor Multilevel Device Type (Sensor type Seismic Magnitude)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ULTRAVIOLET                     0x0D1B   //Sensor Multilevel Device Type (Sensor type Ultraviolet)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ELECTRICAL_RESISTIVITY          0x0D1C   //Sensor Multilevel Device Type (Sensor type Electrical Resistivity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_ELECTRICAL_CONDUCTIVITY         0x0D1D   //Sensor Multilevel Device Type (Sensor type Electrical Conductivity)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_LOUDNESS                        0x0D1E   //Sensor Multilevel Device Type (Sensor type Loudness)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_MOISTURE                        0x0D1F   //Sensor Multilevel Device Type (Sensor type Moisture)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_FREQUENCY                       0x0D20   //Sensor Multilevel Device Type (Sensor type Frequency)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_TIME                            0x0D21   //Sensor Multilevel Device Type (Sensor type Time )
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_TARGET_TEMPERATURE              0x0D22   //Sensor Multilevel Device Type (Sensor type Target Temperature)
#define ICON_TYPE_SPECIFIC_SENSOR_MULTILEVEL_MULTIDEVICE                     0x0DFF   //Sensor Multilevel Device Type (Bundled Sensor functions)

#define ICON_TYPE_GENERIC_SET_TOP_BOX                                        0x0E00   //Set Top Box Device Type

#define ICON_TYPE_GENERIC_SIREN                                              0x0F00   //Siren Device Type

#define ICON_TYPE_GENERIC_SUB_ENERGY_METER                                   0x1000   //Sub Energy Meter Device Type

#define ICON_TYPE_GENERIC_SUB_SYSTEM_CONTROLLER                              0x1100   //Sub System Controller Device Type

#define ICON_TYPE_GENERIC_THERMOSTAT                                         0x1200   //Thermostat Device Type
#define ICON_TYPE_SPECIFIC_THERMOSTAT_LINE_VOLTAGE                           0x1201   //Thermostat Line Voltage Device Type
#define ICON_TYPE_SPECIFIC_THERMOSTAT_SETBACK                                0x1202   //Thermostat Setback Device Type

#define ICON_TYPE_GENERIC_THERMOSTAT_SETBACK_OBSOLETED                       0x1300   //Thermostat Setback [Obsoleted] Device Type

#define ICON_TYPE_GENERIC_TV                                                 0x1400   //TV Device Type

#define ICON_TYPE_GENERIC_VALVE_OPEN_CLOSE                                   0x1500   //Valve Open/Close Device Type

#define ICON_TYPE_GENERIC_WALL_CONTROLLER                                    0x1600   //Wall Controller Device Type

#define ICON_TYPE_GENERIC_WHOLE_HOME_METER_SIMPLE                            0x1700   //Whole Home Meter Simple Device Type

#define ICON_TYPE_GENERIC_WINDOW_COVERING_NO_POSITION_ENDPOINT               0x1800   //Window Covering No Position/Endpoint  Device Type

#define ICON_TYPE_GENERIC_WINDOW_COVERING_ENDPOINT_AWARE                     0x1900   //Window Covering Endpoint Aware Device Type

#define ICON_TYPE_GENERIC_WINDOW_COVERING_POSITION_ENDPOINT_AWARE            0x1A00   //Window Covering Position/Endpoint Aware Device Type

#define ICON_TYPE_GENERIC_REPEATER                                           0x1B00   //Repeater Device Type

#define ICON_TYPE_GENERIC_DIMMER_WALL_SWITCH	                             0x1C00	  //Wall Switch
#define ICON_TYPE_SPECIFIC_DIMMER_WALL_SWITCH_ONE_BUTTON	                 0x1C01	  //Wall Switch, 1 button
#define ICON_TYPE_SPECIFIC_DIMMER_WALL_SWITCH_TWO_BUTTONS	                 0x1C02	  //Wall Switch, 2 buttons
#define ICON_TYPE_SPECIFIC_DIMMER_WALL_SWITCH_THREE_BUTTONS	                 0x1C03	  //Wall Switch, 3 buttons
#define ICON_TYPE_SPECIFIC_DIMMER_WALL_SWITCH_FOUR_BUTTONS	                 0x1C04	  //Wall Switch, 4 buttons
#define ICON_TYPE_SPECIFIC_DIMMER_WALL_SWITCH_ONE_ROTARY	                 0x1CF1	  //Wall Switch, 1 rotary knob

#define ICON_TYPE_GENERIC_ON_OFF_WALL_SWITCH	                             0x1D00	  //Wall Switch
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_ONE_BUTTON	                 0x1D01	  //Wall Switch, 1 button
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_TWO_BUTTONS	                 0x1D02	  //Wall Switch, 2 buttons
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_THREE_BUTTONS	                 0x1D03	  //Wall Switch, 3 buttons
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_FOUR_BUTTONS	                 0x1D04	  //Wall Switch, 4 buttons
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_DOOR_BELL                      0x1DE1   //Door Bell (button)
#define ICON_TYPE_SPECIFIC_ON_OFF_WALL_SWITCH_ONE_ROTARY	                 0x1DF1	  //Wall Switch, 1 rotary knob

#define ICON_TYPE_GENERIC_BARRIER                                            0x1E00   //Barrier

#define ICON_TYPE_GENERIC_IRRIGATION                                         0x1F00   //Irrigation




/************* Manufacturer ID identifiers **************/
#define MFG_ID_NOT_DEFINED                                         0xFFFF   //Not defined
#define MFG_ID_2B_ELECTRONICS                                      0x0028   //2B Electronics
#define MFG_ID_2GIG_TECHNOLOGIES_INC                               0x009B   //2gig Technologies Inc.
#define MFG_ID_3E_TECHNOLOGIES                                     0x002A   //3e Technologies
#define MFG_ID_A1_COMPONENTS                                       0x0022   //A-1 Components
#define MFG_ID_ABILIA                                              0x0117   //Abilia
#define MFG_ID_ACT_ADVANCED_CONTROL_TECHNOLOGIES                   0x0001   //ACT - Advanced Control Technologies
#define MFG_ID_ADOX_INC                                            0x0101   //ADOX, Inc.
#define MFG_ID_ADVANCED_OPTRONIC_DEVICES_CO_LTD                    0x016C   //Advanced Optronic Devices Co.,Ltd
#define MFG_ID_ADVENTURE_INTERACTIVE                               0x009E   //Adventure Interactive
#define MFG_ID_AEON_LABS                                           0x0086   //AEON Labs
#define MFG_ID_AIRLINE_MECHANICAL_CO_LTD                           0x0111   //Airline Mechanical Co., Ltd.
#define MFG_ID_AIRVENT_SAM_SPA                                     0x0088   //Airvent SAM S.p.A.
#define MFG_ID_ALARMCOM                                            0x0094   //Alarm.com
#define MFG_ID_ALERTME                                             0x0126   //Alertme
#define MFG_ID_ALLEGION                                            0x003B   //Allegion
#define MFG_ID_ALPHONSUS_TECH                                      0x0230   //Alphonsus Tech
#define MFG_ID_AMDOCS                                              0x019C   //Amdocs
#define MFG_ID_AMERICAN_GRID_INC                                   0x005A   //American Grid, Inc.
#define MFG_ID_ANYCOMM_CORPORATION                                 0x0078   //anyCOMM Corporation
#define MFG_ID_APPLIED_MICRO_ELECTRONICS_AME_BV                    0x0144   //Applied Micro Electronics "AME" BV
#define MFG_ID_ASIA_HEADING                                        0x0029   //Asia Heading
#define MFG_ID_ASITEQ                                              0x0231   //ASITEQ
#define MFG_ID_ASSA_ABLOY                                          0x0129   //ASSA ABLOY
#define MFG_ID_ASTRALINK                                           0x013B   //AstraLink
#define MFG_ID_ATT                                                 0x0134   //AT&T
#define MFG_ID_ATECH                                               0x002B   //Atech
#define MFG_ID_ATHOM_BV                                            0x0244   //Athom BV
#define MFG_ID_AVADESIGN_TECHNOLOGY_CO_LTD                         0x0155   //Avadesign Technology Co., Ltd.
#define MFG_ID_AXESSTEL_INC                                        0x0146   //Axesstel Inc
#define MFG_ID_BALBOA_INSTRUMENTS                                  0x0018   //Balboa Instruments
#define MFG_ID_BANDI_COMM_TECH_INC                                 0x0236   //Bandi Comm Tech Inc.
#define MFG_ID_BEIJING_SINOAMERICAN_BOYI_SOFTWARE_DEVELOPMENT_CO_L 0x0204   //Beijing Sino-American Boyi Software Development Co., Ltd
#define MFG_ID_BEIJING_UNIVERSAL_ENERGY_HUAXIA_TECHNOLOGY_CO_LTD   0x0251   //Beijing Universal Energy Huaxia Technology Co.,Ltd
#define MFG_ID_BELLATRIX_SYSTEMS_INC                               0x0196   //Bellatrix Systems, Inc.
#define MFG_ID_BENEXT                                              0x008A   //BeNext
#define MFG_ID_BESAFER                                             0x002C   //BeSafer
#define MFG_ID_BFT_SPA                                             0x014B   //BFT S.p.A.
#define MFG_ID_BIT7_INC                                            0x0052   //Bit7 Inc.
#define MFG_ID_BLACK_DECKER                                        0x0090   //Black & Decker
#define MFG_ID_BMS_EVLER_LTD                                       0x0213   //BMS Evler LTD
#define MFG_ID_BOCA_DEVICES                                        0x0023   //Boca Devices
#define MFG_ID_BOSCH_SECURITY_SYSTEMS_INC                          0x015C   //Bosch Security Systems, Inc
#define MFG_ID_BRK_BRANDS_INC                                      0x0138   //BRK Brands, Inc.
#define MFG_ID_BROADBAND_ENERGY_NETWORKS_INC                       0x002D   //Broadband Energy Networks Inc.
#define MFG_ID_BTSTAR_HK_TECHNOLOGY_COMPANY_LIMITED                0x024A   //BTSTAR(HK) TECHNOLOGY COMPANY LIMITED
#define MFG_ID_BUFFALO_INC                                         0x0145   //Buffalo Inc.
#define MFG_ID_BUILDING_36_TECHNOLOGIES                            0x0190   //Building 36 Technologies
#define MFG_ID_BULOGICS                                            0x0026   //BuLogics
#define MFG_ID_BONIG_UND_KALLENBACH_OHG                            0x0169   //Bönig und Kallenbach oHG
#define MFG_ID_CAMEO_COMMUNICATIONS_INC                            0x009C   //Cameo Communications Inc.
#define MFG_ID_CARRIER                                             0x002E   //Carrier
#define MFG_ID_CASAWORKS                                           0x000B   //CasaWorks
#define MFG_ID_CASENIO_AG                                          0x0243   //casenio AG
#define MFG_ID_CBCC_DOMOTIQUE_SAS                                  0x0166   //CBCC Domotique SAS
#define MFG_ID_CENTRALITE_SYSTEMS_INC                              0x0246   //CentraLite Systems, Inc
#define MFG_ID_CHECKIT_SOLUTIONS_INC                               0x014E   //Check-It Solutions Inc.
#define MFG_ID_CHROMAGIC_TECHNOLOGIES_CORPORATION                  0x0116   //Chromagic Technologies Corporation
#define MFG_ID_CISCO_CONSUMER_BUSINESS_GROUP                       0x0082   //Cisco Consumer Business Group
#define MFG_ID_CLIMAX_TECHNOLOGY_LTD                               0x018E   //Climax Technology, Ltd.
#define MFG_ID_CLOUD_MEDIA                                         0x0200   //Cloud Media
#define MFG_ID_COLOR_KINETICS_INCORPORATED                         0x002F   //Color Kinetics Incorporated
#define MFG_ID_COMPUTIME                                           0x0140   //Computime
#define MFG_ID_CONNECTED_OBJECT                                    0x011B   //Connected Object
#define MFG_ID_CONNECTHOME                                         0x0179   //ConnectHome
#define MFG_ID_CONTROLTHINK_LC                                     0x0019   //ControlThink LC
#define MFG_ID_CONVERGEX_LTD                                       0x000F   //ConvergeX Ltd.
#define MFG_ID_COOLGUARD                                           0x007D   //CoolGuard
#define MFG_ID_COOPER_LIGHTING                                     0x0079   //Cooper Lighting
#define MFG_ID_COOPER_WIRING_DEVICES                               0x001A   //Cooper Wiring Devices
#define MFG_ID_COVENTIVE_TECHNOLOGIES_INC                          0x009D   //Coventive Technologies Inc.
#define MFG_ID_CYBERHOUSE                                          0x0014   //Cyberhouse
#define MFG_ID_CYBERTAN_TECHNOLOGY_INC                             0x0067   //CyberTAN Technology, Inc.
#define MFG_ID_CYTECH_TECHNOLOGY_PRE_LTD                           0x0030   //Cytech Technology Pre Ltd.
#define MFG_ID_DANFOSS                                             0x0002   //Danfoss
#define MFG_ID_DAWON_DNS                                           0x018C   //Dawon DNS
#define MFG_ID_DECORIS_INTELLIGENT_SYSTEM_LIMITED                  0x020A   //Decoris Intelligent System Limited
#define MFG_ID_DEFACONTROLS_BV                                     0x013F   //Defacontrols BV
#define MFG_ID_DESTINY_NETWORKS                                    0x0031   //Destiny Networks
#define MFG_ID_DEVOLO                                              0x0175   //Devolo
#define MFG_ID_DIEHL_AKO                                           0x0103   //Diehl AKO
#define MFG_ID_DIGITAL_5_INC                                       0x0032   //Digital 5, Inc.
#define MFG_ID_DIGITAL_HOME_SYSTEMS_PTY_LTD_                       0x024E   //Digital Home Systems Pty Ltd,
#define MFG_ID_DIGITALZONE                                         0x0228   //DigitalZone
#define MFG_ID_DLINK                                               0x0108   //D-Link
#define MFG_ID_DMP_DIGITAL_MONITORING_PRODUCTS                     0x0127   //DMP (Digital Monitoring Products)
#define MFG_ID_DOMINO_SISTEMI_DOO                                  0x0177   //Domino sistemi d.o.o.
#define MFG_ID_DOMITECH_PRODUCTS_LLC                               0x020E   //Domitech Products, LLC
#define MFG_ID_DONGGUAN_ZHOU_DA_ELECTRONICS_CO_LTD                 0x020C   //Dongguan Zhou Da Electronics Co.,Ltd
#define MFG_ID_DRACOR_INC                                          0x017D   //DRACOR Inc.
#define MFG_ID_DRAGON_TECH_INDUSTRIAL_LTD                          0x0184   //Dragon Tech Industrial, Ltd.
#define MFG_ID_DTV_RESEARCH_UNIPESSOAL_LDA                         0x0223   //DTV Research Unipessoal, Lda
#define MFG_ID_DYNAQUIP_CONTROLS                                   0x0132   //DynaQuip Controls
#define MFG_ID_EASY_SAVER_CO_INC                                   0x0247   //EASY SAVER Co., Inc
#define MFG_ID_EBV                                                 0x017C   //EbV
#define MFG_ID_ECHOSTAR                                            0x016B   //Echostar
#define MFG_ID_ECOLINK                                             0x014A   //Ecolink
#define MFG_ID_ECONET_CONTROLS                                     0x0157   //EcoNet Controls
#define MFG_ID_EHOME_AUTOMATION                                    0x010D   //e-Home AUTOMATION
#define MFG_ID_EKA_SYSTEMS                                         0x0087   //Eka Systems
#define MFG_ID_ELECTRONIC_SOLUTIONS                                0x0033   //Electronic Solutions
#define MFG_ID_ELEXA_CONSUMER_PRODUCTS_INC                         0x021F   //Elexa Consumer Products Inc.
#define MFG_ID_ELGEV_ELECTRONICS_LTD                               0x0034   //El-Gev Electronics LTD
#define MFG_ID_ELK_PRODUCTS_INC                                    0x001B   //ELK Products, Inc.
#define MFG_ID_EMBEDDED_SYSTEM_DESIGN_LIMITED                      0x020B   //Embedded System Design Limited
#define MFG_ID_EMBEDIT_AS                                          0x0035   //Embedit A/S
#define MFG_ID_ENBLINK_CO_LTD                                      0x014D   //Enblink Co. Ltd
#define MFG_ID_ENWOX_TECHNOLOGIES_SRO                              0x0219   //Enwox Technologies s.r.o.
#define MFG_ID_ERONE                                               0x006F   //Erone
#define MFG_ID_ESSENCE_SECURITY                                    0x0160   //Essence Security
#define MFG_ID_EUROTRONICS                                         0x0148   //Eurotronics
#define MFG_ID_EVERSPRING                                          0x0060   //Everspring
#define MFG_ID_EVOLVE                                              0x0113   //Evolve
#define MFG_ID_EXCEPTIONAL_INNOVATIONS                             0x0036   //Exceptional Innovations
#define MFG_ID_EXHAUSTO                                            0x0004   //Exhausto
#define MFG_ID_EXIGENT_SENSORS                                     0x009F   //Exigent Sensors
#define MFG_ID_EXPRESS_CONTROLS                                    0x001E   //Express Controls
#define MFG_ID_EZEX_CORPORATION                                    0x0233   //eZEX Corporation
#define MFG_ID_FAKRO                                               0x0085   //Fakro
#define MFG_ID_FANTEM                                              0x016A   //Fantem
#define MFG_ID_FIBARGROUP                                          0x010F   //Fibargroup
#define MFG_ID_FLEXTRONICS                                         0x018D   //Flextronics
#define MFG_ID_FLUE_SENTINEL                                       0x0024   //Flue Sentinel
#define MFG_ID_FOARD_SYSTEMS                                       0x0037   //Foard Systems
#define MFG_ID_FOCAL_POINT_LIMITED                                 0x018F   //Focal Point Limited
#define MFG_ID_FOLLOWGOOD_TECHNOLOGY_COMPANY_LTD                   0x0137   //FollowGood Technology Company Ltd.
#define MFG_ID_FOREST_GROUP_NEDERLAND_BV                           0x0207   //Forest Group Nederland B.V
#define MFG_ID_FORTREZZ_LLC                                        0x0084   //FortrezZ LLC
#define MFG_ID_FOXCONN                                             0x011D   //Foxconn
#define MFG_ID_FROSTDALE                                           0x0110   //Frostdale
#define MFG_ID_GES                                                 0x025A   //GES
#define MFG_ID_GKB_SECURITY_CORPORATION                            0x022B   //GKB Security Corporation
#define MFG_ID_GLOBALCHINATECH                                     0x018A   //Globalchina-Tech
#define MFG_ID_GOAP                                                0x0159   //Goap
#define MFG_ID_GOGGIN_RESEARCH                                     0x0076   //Goggin Research
#define MFG_ID_GOOD_WAY_TECHNOLOGY_CO_LTD                          0x0068   //Good Way Technology Co., Ltd
#define MFG_ID_GREENWAVE_REALITY_INC                               0x0099   //GreenWave Reality Inc.
#define MFG_ID_GRIB                                                0x018B   //Grib
#define MFG_ID_GUANGZHOU_RUIXIANG_ME_CO_LTD                        0x016D   //Guangzhou Ruixiang M&E Co., Ltd
#define MFG_ID_GUANGZHOU_ZEEWAVE_INFORMATION_TECHNOLOGY_CO_LTD     0x0158   //GuangZhou Zeewave Information Technology Co., Ltd.
#define MFG_ID_HANKOOK_GAS_KIKI_CO_LTD                             0x024C   //Hankook Gas Kiki CO.,LTD.
#define MFG_ID_HAUPPAUGE                                           0x025C   //Hauppauge
#define MFG_ID_HAWKING_TECHNOLOGIES_INC                            0x0073   //Hawking Technologies Inc.
#define MFG_ID_HERALD_DATANETICS_LIMITED                           0x020F   //Herald Datanetics Limited
#define MFG_ID_HITECH_AUTOMATION                                   0x0017   //HiTech Automation
#define MFG_ID_HOLION_ELECTRONIC_ENGINEERING_CO_LTD                0x0181   //Holion Electronic Engineering Co., Ltd
#define MFG_ID_HOLTEC_ELECTRONICS_BV                               0x013E   //Holtec Electronics BV
#define MFG_ID_HOME_AUTOMATED_LIVING                               0x000D   //Home Automated Living
#define MFG_ID_HOME_AUTOMATION_EUROPE                              0x009A   //Home Automation Europe
#define MFG_ID_HOME_AUTOMATION_INC                                 0x005B   //Home Automation Inc.
#define MFG_ID_HOME_DIRECTOR                                       0x0038   //Home Director
#define MFG_ID_HOMEMANAGEABLES_INC                                 0x0070   //Homemanageables, Inc.
#define MFG_ID_HOMEPRO                                             0x0050   //Homepro
#define MFG_ID_HOMESCENARIO                                        0x0162   //HomeScenario
#define MFG_ID_HOMESEER_TECHNOLOGIES                               0x000C   //HomeSeer Technologies
#define MFG_ID_HONEST_TECHNOLOGY_CO_LTD                            0x023D   //Honest Technology Co., Ltd.
#define MFG_ID_HONEYWELL                                           0x0039   //Honeywell
#define MFG_ID_HORSTMANN_CONTROLS_LIMITED                          0x0059   //Horstmann Controls Limited
#define MFG_ID_HOSEOTELNET                                         0x0221   //HOSEOTELNET
#define MFG_ID_HUAPIN_INFORMATION_TECHNOLOGY_CO_LTD                0x0180   //Huapin Information Technology Co.,Ltd
#define MFG_ID_HUAWEI_TECHNOLOGIES_CO_LTD                          0x024B   //Huawei Technologies Co., Ltd.
#define MFG_ID_HUNTER_DOUGLAS                                      0x007C   //Hunter Douglas
#define MFG_ID_IAUTOMADE_PTE_LTD                                   0x0218   //iAutomade Pte Ltd
#define MFG_ID_ICOM_TECHNOLOGY_BV                                  0x0011   //iCOM Technology b.v.
#define MFG_ID_ICONTROL                                            0x0106   //iControl
#define MFG_ID_ICONTROL_NETWORKS                                   0x0106   //Icontrol Networks
#define MFG_ID_IDRF                                                0x0165   //ID-RF
#define MFG_ID_IEXERGY_GMBH                                        0x019E   //iEXERGY GmbH
#define MFG_ID_IMPACT_TECHNOLOGIES_AND_PRODUCTS                    0x0056   //Impact Technologies and Products
#define MFG_ID_IMPACT_TECHNOLOGIES_BV                              0x0061   //Impact Technologies BV
#define MFG_ID_INFUSION_DEVELOPMENT                                0x012B   //Infusion Development
#define MFG_ID_INGERSOLL_RAND_SCHLAGE                              0x006C   //Ingersoll Rand (Schlage)
#define MFG_ID_INGERSOLL_RAND_ECOLINK                              0x011F   //Ingersoll Rand (was Ecolink)
#define MFG_ID_INKEL_CORP                                          0x0256   //Inkel Corp.
#define MFG_ID_INLON_SRL                                           0x003A   //Inlon Srl
#define MFG_ID_INNOBAND_TECHNOLOGIES_INC                           0x0141   //Innoband Technologies, Inc
#define MFG_ID_INNOVUS                                             0x0077   //INNOVUS
#define MFG_ID_INSIGNIA                                            0x0100   //Insignia
#define MFG_ID_INTEL                                               0x0006   //Intel
#define MFG_ID_INTELLICON                                          0x001C   //IntelliCon
#define MFG_ID_INTERACTIVE_ELECTRONICS_SYSTEMS_IES                 0x0072   //Interactive Electronics Systems (IES)
#define MFG_ID_INTERMATIC                                          0x0005   //Intermatic
#define MFG_ID_INTERNET_DOM                                        0x0013   //Internet Dom
#define MFG_ID_IQGROUP                                             0x005F   //IQ-Group
#define MFG_ID_IREVO                                               0x0212   //iRevo
#define MFG_ID_IUNGONL_BV                                          0x0253   //iungo.nl B.V.
#define MFG_ID_IWATSU                                              0x0123   //IWATSU
#define MFG_ID_JASCO_PRODUCTS                                      0x0063   //Jasco Products
#define MFG_ID_JIN_TAO_BAO                                         0x015A   //Jin Tao Bao
#define MFG_ID_JSW_PACIFIC_CORPORATION                             0x0164   //JSW Pacific Corporation
#define MFG_ID_KAIPULE_TECHNOLOGY_CO_LTD                           0x0214   //Kaipule Technology Co., Ltd.
#define MFG_ID_KAMSTRUP_AS                                         0x0091   //Kamstrup A/S
#define MFG_ID_KELLENDONK_ELEKTRONIK                               0x006A   //Kellendonk Elektronik
#define MFG_ID_KICHLER                                             0x0114   //Kichler
#define MFG_ID_KOPERA_DEVELOPMENT_INC                              0x0174   //Kopera Development Inc.
#define MFG_ID_KUMHO_ELECTRIC_INC                                  0x023A   //KUMHO ELECTRIC, INC
#define MFG_ID_LAGOTEK_CORPORATION                                 0x0051   //Lagotek Corporation
#define MFG_ID_LEAK_INTELLIGENCE_LLC                               0x0173   //Leak Intelligence, LLC
#define MFG_ID_LEVION_TECHNOLOGIES_GMBH                            0x0187   //LEVION Technologies Gmbh
#define MFG_ID_LEVITON                                             0x001D   //Leviton
#define MFG_ID_LEXEL                                               0x0015   //Lexel
#define MFG_ID_LG_ELECTRONICS                                      0x015B   //LG Electronics
#define MFG_ID_LIFESHIELD_LLC                                      0x0224   //LifeShield, LLC
#define MFG_ID_LIFESTYLE_NETWORKS                                  0x003C   //Lifestyle Networks
#define MFG_ID_LIGHT_ENGINE_LIMITED                                0x0210   //Light Engine Limited
#define MFG_ID_LINEAR_CORP                                         0x014F   //Linear Corp
#define MFG_ID_LIVEGUARD_LTD                                       0x017A   //Liveguard Ltd.
#define MFG_ID_LIVING_STYLE_ENTERPRISES_LTD                        0x013A   //Living Style Enterprises, Ltd.
#define MFG_ID_LOCSTAR_TECHNOLOGY_CO_LTD                           0x015E   //Locstar Technology Co., Ltd
#define MFG_ID_LOGITECH                                            0x007F   //Logitech
#define MFG_ID_LOUDWATER_TECHNOLOGIES_LLC                          0x0025   //Loudwater Technologies, LLC
#define MFG_ID_LS_CONTROL                                          0x0071   //LS Control
#define MFG_ID_LVI_PRODUKTER_AB                                    0x0062   //LVI Produkter AB
#define MFG_ID_M2M_SOLUTION                                        0x0195   //M2M Solution
#define MFG_ID_MANODO_KTC                                          0x006E   //Manodo / KTC
#define MFG_ID_MARMITEK_BV                                         0x003D   //Marmitek BV
#define MFG_ID_MARTEC_ACCESS_PRODUCTS                              0x003E   //Martec Access Products
#define MFG_ID_MARTIN_RENZ_GMBH                                    0x0092   //Martin Renz GmbH
#define MFG_ID_MB_TURN_KEY_DESIGN                                  0x008F   //MB Turn Key Design
#define MFG_ID_MCOHOME_TECHNOLOGY_CO_LTD                           0x015F   //McoHome Technology Co., Ltd
#define MFG_ID_MCT_CO_LTD                                          0x0222   //MCT CO., LTD
#define MFG_ID_MEEDIO_LLC                                          0x0027   //Meedio, LLC
#define MFG_ID_MEGACHIPS                                           0x0107   //MegaChips
#define MFG_ID_MERCURY_CORPORATION                                 0x022D   //Mercury Corporation
#define MFG_ID_MERTEN                                              0x007A   //Merten
#define MFG_ID_MILANITY_INC                                        0x0238   //Milanity, Inc.
#define MFG_ID_MITSUMI                                             0x0112   //MITSUMI
#define MFG_ID_MOBILUS_MOTOR_SPOLKA_Z_OO                           0x019D   //MOBILUS MOTOR Spólka z o.o.
#define MFG_ID_MODACOM_CO_LTD                                      0x0232   //MODACOM CO., LTD.
#define MFG_ID_MODSTROM                                            0x008D   //Modstrøm
#define MFG_ID_MOHITO_NETWORKS                                     0x000E   //Mohito Networks
#define MFG_ID_MONOPRICE                                           0x0202   //Monoprice
#define MFG_ID_MONSTER_CABLE                                       0x007E   //Monster Cable
#define MFG_ID_MOTION_CONTROL_SYSTEMS                              0x0125   //Motion Control Systems
#define MFG_ID_MOTOROLA                                            0x003F   //Motorola
#define MFG_ID_MSK_MIYAKAWA_SEISAKUSHO                             0x0122   //MSK - Miyakawa Seisakusho
#define MFG_ID_MTC_MAINTRONIC_GERMANY                              0x0083   //MTC Maintronic Germany
#define MFG_ID_MYSTROM                                             0x0143   //myStrom
#define MFG_ID_NANJING_EASTHOUSE_ELECTRICAL_CO_LTD                 0x016E   //Nanjing Easthouse Electrical Co., Ltd.
#define MFG_ID_NAPCO_SECURITY_TECHNOLOGIES_INC                     0x0121   //Napco Security Technologies, Inc.
#define MFG_ID_NEFIT                                               0x006D   //Nefit
#define MFG_ID_NESS_CORPORATION_PTY_LTD                            0x0189   //Ness Corporation Pty Ltd
#define MFG_ID_NETGEAR                                             0x0133   //Netgear
#define MFG_ID_NEWLAND_COMMUNICATION_SCIENCE_TECHNOLOGY_CO_LTD     0x0203   //Newland Communication Science Technology Co., Ltd.
#define MFG_ID_NEXIA_HOME_INTELLIGENCE                             0x0178   //Nexia Home Intelligence
#define MFG_ID_NEXTENERGY                                          0x0075   //NextEnergy
#define MFG_ID_NINGBO_SENTEK_ELECTRONICS_CO_LTD                    0x0185   //Ningbo Sentek Electronics Co., Ltd
#define MFG_ID_NORTH_CHINA_UNIVERSITY_OF_TECHNOLOGY                0x0252   //North China University of Technology
#define MFG_ID_NORTHQ                                              0x0096   //NorthQ
#define MFG_ID_NOVAR_ELECTRICAL_DEVICES_AND_SYSTEMS_EDS            0x0040   //Novar Electrical Devices and Systems (EDS)
#define MFG_ID_NOVATEQNI_HK_LTD                                    0x020D   //Novateqni HK Ltd
#define MFG_ID_OMNIMA_LIMITED                                      0x0119   //Omnima Limited
#define MFG_ID_ONSITE_PRO                                          0x014C   //OnSite Pro
#define MFG_ID_OPENPEAK_INC                                        0x0041   //OpenPeak Inc.
#define MFG_ID_PANASONIC_ELECTRIC_WORKS_CO_LTD                     0x0104   //Panasonic Electric Works Co., Ltd.
#define MFG_ID_PARATECH                                            0x0257   //PARATECH
#define MFG_ID_PASSIVSYSTEMS_LIMITED                               0x0172   //PassivSystems Limited
#define MFG_ID_PELLA                                               0x013D   //Pella
#define MFG_ID_PERMUNDO_GMBH                                       0x0245   //permundo GmbH
#define MFG_ID_PHILIO_TECHNOLOGY_CORP                              0x013C   //Philio Technology Corp
#define MFG_ID_PHYSICAL_GRAPH_CORPORATION                          0x0150   //Physical Graph Corporation
#define MFG_ID_PITECH                                              0x007B   //PiTech
#define MFG_ID_POLYCONTROL                                         0x010E   //Poly-control
#define MFG_ID_POPP_CO                                             0x0154   //Popp & Co
#define MFG_ID_POWERHOUSE_DYNAMICS                                 0x0170   //Powerhouse Dynamics
#define MFG_ID_POWERLINX                                           0x0074   //PowerLinx
#define MFG_ID_POWERLYNX                                           0x0016   //PowerLynx
#define MFG_ID_PRAGMATIC_CONSULTING_INC                            0x0042   //Pragmatic Consulting Inc.
#define MFG_ID_PRODRIVE_TECHNOLOGIES                               0x0128   //Prodrive Technologies
#define MFG_ID_PROMIXIS_LLC                                        0x0161   //Promixis, LLC
#define MFG_ID_PULSE_TECHNOLOGIES_ASPALIS                          0x005D   //Pulse Technologies (Aspalis)
#define MFG_ID_QEES                                                0x0095   //Qees
#define MFG_ID_QOLSYS                                              0x012A   //Qolsys
#define MFG_ID_QUBY                                                0x0130   //Quby
#define MFG_ID_QUEENLOCK_IND_CO_LTD                                0x0163   //Queenlock Ind. Co., Ltd.
#define MFG_ID_RADEMACHER_GERATEELEKTRONIK_GMBH_CO_KG              0x0142   //Rademacher Geräte-Elektronik GmbH & Co. KG
#define MFG_ID_RADIO_THERMOSTAT_COMPANY_OF_AMERICA_RTC             0x0098   //Radio Thermostat Company of America (RTC)
#define MFG_ID_RARITAN                                             0x008E   //Raritan
#define MFG_ID_RED_BEE_CO_LTD                                      0x021E   //Red Bee Co. Ltd
#define MFG_ID_REITZGROUPDE                                        0x0064   //Reitz-Group.de
#define MFG_ID_REMOTE_SOLUTION                                     0x022C   //Remote Solution
#define MFG_ID_REMOTEC                                             0x5254   //Remotec
#define MFG_ID_RESIDENTIAL_CONTROL_SYSTEMS_INC_RCS                 0x0010   //Residential Control Systems, Inc. (RCS)
#define MFG_ID_RET_NANJING_INTELLIGENCE_SYSTEM_CO_LTD              0x0216   //RET Nanjing Intelligence System CO.,Ltd
#define MFG_ID_REVOLV_INC                                          0x0153   //Revolv Inc
#define MFG_ID_RIMPORT_LTD                                         0x0147   //R-import Ltd.
#define MFG_ID_ROCCONNECT_INC                                      0x023B   //ROC-Connect, Inc.
#define MFG_ID_RPE_AJAX_LLC_DBS_SECUR_LTD                          0x0197   //RPE Ajax LLC (dbs Secur Ltd)
#define MFG_ID_RS_SCENE_AUTOMATION                                 0x0065   //RS Scene Automation
#define MFG_ID_SAFETECH_PRODUCTS                                   0x023C   //SafeTech Products
#define MFG_ID_SAMSUNG_ELECTRONICS_CO_LTD                          0x0201   //Samsung Electronics Co., Ltd.
#define MFG_ID_SAMSUNG_SDS                                         0x022E   //Samsung SDS
#define MFG_ID_SAN_SHIH_ELECTRICAL_ENTERPRISE_CO_LTD               0x0093   //San Shih Electrical Enterprise Co., Ltd.
#define MFG_ID_SANAV                                               0x012C   //SANAV
#define MFG_ID_SCIENTIA_TECHNOLOGIES_INC                           0x001F   //Scientia Technologies, Inc.
#define MFG_ID_SECURE_WIRELESS                                     0x011E   //Secure Wireless
#define MFG_ID_SECURENET_TECHNOLOGIES                              0x0167   //SecureNet Technologies
#define MFG_ID_SECURIFI_LTD                                        0x0182   //Securifi Ltd.
#define MFG_ID_SELUXIT                                             0x0069   //Seluxit
#define MFG_ID_SENMATIC_AS                                         0x0043   //Senmatic A/S
#define MFG_ID_SEQUOIA_TECHNOLOGY_LTD                              0x0044   //Sequoia Technology LTD
#define MFG_ID_SERCOMM_CORP                                        0x0151   //Sercomm Corp
#define MFG_ID_SHANGDONG_SMART_LIFE_DATA_SYSTEM_CO_LTD             0x0215   //Shangdong Smart Life Data System Co.,Ltd
#define MFG_ID_SHANGHAI_DORLINK_INTELLIGENT_TECHNOLOGIES_CO_LTD    0x023E   //Shanghai Dorlink Intelligent Technologies Co.,Ltd
#define MFG_ID_SHANGHAI_LONGCHUANG_ECOENERGY_SYSTEMS_CO_LTD        0x0205   //Shanghai Longchuang Eco-energy Systems Co., Ltd
#define MFG_ID_SHARP                                               0x010B   //Sharp
#define MFG_ID_SHENZHEN_AOYA_INDUSTRY_CO_LTD                       0x021A   //SHENZHEN AOYA INDUSTRY CO. LTD
#define MFG_ID_SHENZHEN_ISURPASS_TECHNOLOGY_CO_LTD                 0x021C   //Shenzhen iSurpass Technology Co. ,Ltd
#define MFG_ID_SHENZHEN_KAADAS_INTELLIGENT_TECHNOLOGY_CO_LTD       0x021D   //Shenzhen Kaadas Intelligent Technology Co., Ltd
#define MFG_ID_SHENZHEN_LIAO_WANG_TONG_DA_TECHNOLOGY_LTD           0x0211   //Shenzhen Liao Wang Tong Da Technology Ltd
#define MFG_ID_SHENZHEN_NEO_ELECTRONICS_CO_LTD                     0x0258   //Shenzhen Neo Electronics Co., Ltd
#define MFG_ID_SHENZHEN_TRIPATH_DIGITAL_AUDIO_EQUIPMENT_CO_LTD     0x0250   //Shenzhen Tripath Digital Audio Equipment Co.,Ltd
#define MFG_ID_SIEGENIAAUBI_KG                                     0x0081   //SIEGENIA-AUBI KG
#define MFG_ID_SIGMA_DESIGNS_ZENSYS                                0x0000   //Sigma Designs (Former Zensys)
#define MFG_ID_SINE_WIRELESS                                       0x0045   //Sine Wireless
#define MFG_ID_SMART_PRODUCTS_INC                                  0x0046   //Smart Products, Inc.
#define MFG_ID_SMARTLY_AS                                          0x024F   //Smartly AS
#define MFG_ID_SMK_MANUFACTURING_INC                               0x0102   //SMK Manufacturing Inc.
#define MFG_ID_SOMFY                                               0x0047   //Somfy
#define MFG_ID_SPECTRUM_BRANDS                                     0x0254   //Spectrum Brands
#define MFG_ID_SQUARE_CONNECT                                      0x0124   //Square Connect
#define MFG_ID_STT_ELECTRIC_CORPORATION                            0x021B   //ST&T Electric Corporation
#define MFG_ID_STARKOFF                                            0x0259   //Starkoff
#define MFG_ID_STELPRO                                             0x0239   //Stelpro
#define MFG_ID_STRATTEC_ADVANCED_LOGIC_LLC                         0x0217   //Strattec Advanced Logic,LLC
#define MFG_ID_STRATTEC_SECURITY_CORPORATION                       0x0168   //STRATTEC Security Corporation
#define MFG_ID_SUMITOMO                                            0x0105   //Sumitomo
#define MFG_ID_SUPERNA                                             0x0054   //Superna
#define MFG_ID_SWANN_COMMUNICATIONS_PTY_LTD                        0x0191   //Swann Communications Pty Ltd
#define MFG_ID_SYLVANIA                                            0x0009   //Sylvania
#define MFG_ID_SYSTECH_CORPORATION                                 0x0136   //Systech Corporation
#define MFG_ID_TAEWON_LIGHTING_CO_LTD                              0x0235   //TAEWON Lighting Co., Ltd.
#define MFG_ID_TEAM_DIGITAL_LIMITED                                0x0186   //Team Digital Limited
#define MFG_ID_TEAM_PRECISION_PCL                                  0x0089   //Team Precision PCL
#define MFG_ID_TECHNICOLOR                                         0x0240   //Technicolor
#define MFG_ID_TECHNIKU                                            0x000A   //Techniku
#define MFG_ID_TECOM_CO_LTD                                        0x012F   //Tecom Co., Ltd.
#define MFG_ID_TELL_IT_ONLINE                                      0x0012   //Tell It Online
#define MFG_ID_TELLDUS_TECHNOLOGIES_AB                             0x0176   //Telldus Technologies AB
#define MFG_ID_TELSEY                                              0x0048   //Telsey
#define MFG_ID_TELULAR                                             0x017E   //Telular
#define MFG_ID_TERRA_OPTIMA_BV_PRIMAIR_SERVICES                    0x005C   //Terra Optima B.V. (tidligere Primair Services)
#define MFG_ID_THERE_CORPORATION                                   0x010C   //There Corporation
#define MFG_ID_THERMOFLOOR                                         0x019B   //ThermoFloor
#define MFG_ID_TIMEVALVE_INC                                       0x022A   //TIMEVALVE, Inc.
#define MFG_ID_TKB_HOME                                            0x0118   //TKB Home
#define MFG_ID_TKH_GROUP_EMINENT                                   0x011C   //TKH Group / Eminent
#define MFG_ID_TRANE_CORPORATION                                   0x008B   //Trane Corporation
#define MFG_ID_TRICKLESTAR                                         0x0066   //TrickleStar
#define MFG_ID_TRICKLESTAR_LTD_EMPOWER_CONTROLS_LTD                0x006B   //Tricklestar Ltd. (former Empower Controls Ltd.)
#define MFG_ID_TRIDIUM                                             0x0055   //Tridium
#define MFG_ID_TWISTHINK                                           0x0049   //Twisthink
#define MFG_ID_UFAIRY_GR_TECH                                      0x0152   //UFairy G.R. Tech
#define MFG_ID_UNIVERSAL_DEVICES_INC                               0x0193   //Universal Devices, Inc
#define MFG_ID_UNIVERSAL_ELECTRONICS_INC                           0x0020   //Universal Electronics Inc.
#define MFG_ID_UNIVERSE_FUTURE                                     0x0183   //Universe Future
#define MFG_ID_UTC_FIRE_AND_SECURITY_AMERICAS_CORP                 0x0209   //UTC Fire and Security Americas Corp
#define MFG_ID_VDA                                                 0x010A   //VDA
#define MFG_ID_VENSTAR_INC                                         0x0198   //Venstar Inc.
#define MFG_ID_VERA_CONTROL                                        0x008C   //Vera Control
#define MFG_ID_VERO_DUCO                                           0x0080   //Vero Duco
#define MFG_ID_VESTEL_ELEKTRONIK_TICARET_VE_SANAYI_AS              0x0237   //Vestel Elektronik Ticaret ve Sanayi A.S.
#define MFG_ID_VIEWSONIC                                           0x0053   //Viewsonic
#define MFG_ID_VIEWSONIC_CORPORATION                               0x005E   //ViewSonic Corporation
#define MFG_ID_VIMAR_CRS                                           0x0007   //Vimar CRS
#define MFG_ID_VIPASTAR                                            0x0188   //Vipa-Star
#define MFG_ID_VISION_SECURITY                                     0x0109   //Vision Security
#define MFG_ID_VISUALIZE                                           0x004A   //Visualize
#define MFG_ID_VITELEC                                             0x0058   //Vitelec
#define MFG_ID_VIVINT                                              0x0156   //Vivint
#define MFG_ID_VSSAFETY_AS                                         0x017B   //Vs-Safety AS
#define MFG_ID_WATT_STOPPER                                        0x004B   //Watt Stopper
#define MFG_ID_WAYNE_DALTON                                        0x0008   //Wayne Dalton
#define MFG_ID_WEBEE_LIFE                                          0x019F   //Webee Life
#define MFG_ID_WEBEHOME_AB                                         0x0171   //WeBeHome AB
#define MFG_ID_WENZHOU_MTLC_ELECTRIC_APPLIANCES_CO_LTD             0x011A   //Wenzhou MTLC Electric Appliances Co.,Ltd.
#define MFG_ID_WHIRLPOOL                                           0x0057   //Whirlpool
#define MFG_ID_WIDOM                                               0x0149   //wiDom
#define MFG_ID_WILLIS_ELECTRIC_CO_LTD                              0x015D   //Willis Electric Co., Ltd.
#define MFG_ID_WILSHINE_HOLDING_CO_LTD                             0x012D   //Wilshine Holding Co., Ltd
#define MFG_ID_WINK_INC                                            0x017F   //Wink Inc.
#define MFG_ID_WINTOP                                              0x0097   //Wintop
#define MFG_ID_WINYTECHNOLOGY                                      0x0242   //Winytechnology
#define MFG_ID_WIRELESS_MAINGATE_AB                                0x0199   //Wireless Maingate AB
#define MFG_ID_WOODWARD_LABS                                       0x004C   //Woodward Labs
#define MFG_ID_WRAP                                                0x0003   //Wr@p
#define MFG_ID_WRT_INTELLIGENT_TECHNOLOGY_CO_LTD                   0x022F   //WRT Intelligent Technology CO., LTD.
#define MFG_ID_WUHAN_NWD_TECHNOLOGY_CO_LTD                         0x012E   //Wuhan NWD Technology Co., Ltd.
#define MFG_ID_XANBOO                                              0x004D   //Xanboo
#define MFG_ID_ZDATA_LLC                                           0x004E   //Zdata, LLC.
#define MFG_ID_ZHEJIANG_JIUXING_ELECTRIC_CO_LTD                    0x016F   //Zhejiang Jiuxing Electric Co Ltd
#define MFG_ID_ZHOME                                               0x0139   //Zhome
#define MFG_ID_ZIPATO                                              0x0131   //Zipato
#define MFG_ID_ZONOFF                                              0x0120   //Zonoff
#define MFG_ID_ZWAVE_TECHNOLOGIA                                   0x004F   //Z-Wave Technologia
#define MFG_ID_ZWAVEME                                             0x0115   //Z-Wave.Me
#define MFG_ID_ZWORKS_INC                                          0x024D   //Z-works Inc.
#define MFG_ID_ZYKRONIX                                            0x0021   //Zykronix
#define MFG_ID_ZYXEL                                               0x0135   //ZyXEL


/*************** command class identifiers ****************/
#define COMMAND_CLASS_EXT_TEST                                                           0xF212 //Testing of extended command #define COMMAND_CLASS_ALARM                                                              0x71 /*DEPRECATED*/
#define COMMAND_CLASS_ALARM                                                              0x71 /*DEPRECATED*/
#define COMMAND_CLASS_ALARM_V2                                                           0x71 /*DEPRECATED*/
#define COMMAND_CLASS_NOTIFICATION_V3                                                    0x71
#define COMMAND_CLASS_NOTIFICATION_V4                                                    0x71
#define COMMAND_CLASS_NOTIFICATION_V5                                                    0x71
#define COMMAND_CLASS_NOTIFICATION_V6                                                    0x71
#define COMMAND_CLASS_NOTIFICATION_V7                                                    0x71
#define COMMAND_CLASS_APPLICATION_STATUS                                                 0x22
#define COMMAND_CLASS_ASSOCIATION_COMMAND_CONFIGURATION                                  0x9B
#define COMMAND_CLASS_ASSOCIATION                                                        0x85
#define COMMAND_CLASS_ASSOCIATION_V2                                                     0x85
#define COMMAND_CLASS_AV_CONTENT_DIRECTORY_MD                                            0x95
#define COMMAND_CLASS_AV_CONTENT_SEARCH_MD                                               0x97
#define COMMAND_CLASS_AV_RENDERER_STATUS                                                 0x96
#define COMMAND_CLASS_AV_TAGGING_MD                                                      0x99
#define COMMAND_CLASS_BASIC_TARIFF_INFO                                                  0x36
#define COMMAND_CLASS_BASIC_WINDOW_COVERING                                              0x50 /*Obsoleted*/
#define COMMAND_CLASS_BASIC                                                              0x20
#define COMMAND_CLASS_BASIC_V2                                                           0x20
#define COMMAND_CLASS_BATTERY                                                            0x80
#define COMMAND_CLASS_CHIMNEY_FAN                                                        0x2A
#define COMMAND_CLASS_CLIMATE_CONTROL_SCHEDULE                                           0x46 /*DEPRECATED*/
#define COMMAND_CLASS_CLOCK                                                              0x81
#define COMMAND_CLASS_CONFIGURATION                                                      0x70
#define COMMAND_CLASS_CONFIGURATION_V2                                                   0x70
#define COMMAND_CLASS_CONFIGURATION_V3                                                   0x70
#define COMMAND_CLASS_CONTROLLER_REPLICATION                                             0x21
#define COMMAND_CLASS_CRC_16_ENCAP                                                       0x56
#define COMMAND_CLASS_DCP_CONFIG                                                         0x3A
#define COMMAND_CLASS_DCP_MONITOR                                                        0x3B
#define COMMAND_CLASS_DOOR_LOCK_LOGGING                                                  0x4C
#define COMMAND_CLASS_DOOR_LOCK                                                          0x62
#define COMMAND_CLASS_DOOR_LOCK_V2                                                       0x62
#define COMMAND_CLASS_DOOR_LOCK_V3                                                       0x62
#define COMMAND_CLASS_ENERGY_PRODUCTION                                                  0x90
#define COMMAND_CLASS_FIRMWARE_UPDATE_MD                                                 0x7A /*DEPRECATED*/
#define COMMAND_CLASS_FIRMWARE_UPDATE_MD_V2                                              0x7A
#define COMMAND_CLASS_FIRMWARE_UPDATE_MD_V3                                              0x7A
#define COMMAND_CLASS_FIRMWARE_UPDATE_MD_V4                                              0x7A
#define COMMAND_CLASS_GEOGRAPHIC_LOCATION                                                0x8C
#define COMMAND_CLASS_GROUPING_NAME                                                      0x7B /*DEPRECATED*/
#define COMMAND_CLASS_HAIL                                                               0x82 /*DEPRECATED*/
#define COMMAND_CLASS_HRV_CONTROL                                                        0x39
#define COMMAND_CLASS_HRV_STATUS                                                         0x37
#define COMMAND_CLASS_INDICATOR                                                          0x87
#define COMMAND_CLASS_INDICATOR_V2                                                       0x87
#define COMMAND_CLASS_IP_CONFIGURATION                                                   0x9A
#define COMMAND_CLASS_LANGUAGE                                                           0x89
#define COMMAND_CLASS_LOCK                                                               0x76
#define COMMAND_CLASS_MANUFACTURER_PROPRIETARY                                           0x91
#define COMMAND_CLASS_MANUFACTURER_SPECIFIC                                              0x72
#define COMMAND_CLASS_MANUFACTURER_SPECIFIC_V2                                           0x72
#define COMMAND_CLASS_MARK                                                               0xEF
#define COMMAND_CLASS_METER_PULSE                                                        0x35 /*DEPRECATED*/
#define COMMAND_CLASS_METER_TBL_CONFIG                                                   0x3C
#define COMMAND_CLASS_METER_TBL_MONITOR                                                  0x3D
#define COMMAND_CLASS_METER_TBL_MONITOR_V2                                               0x3D
#define COMMAND_CLASS_METER_TBL_PUSH                                                     0x3E
#define COMMAND_CLASS_METER                                                              0x32
#define COMMAND_CLASS_METER_V2                                                           0x32
#define COMMAND_CLASS_METER_V3                                                           0x32
#define COMMAND_CLASS_METER_V4                                                           0x32
#define COMMAND_CLASS_MTP_WINDOW_COVERING                                                0x51 /*OBSOLETED*/
#define COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V2                                       0x8E
#define COMMAND_CLASS_MULTI_CHANNEL_ASSOCIATION_V3                                       0x8E
#define COMMAND_CLASS_MULTI_CHANNEL_V2                                                   0x60
#define COMMAND_CLASS_MULTI_CHANNEL_V3                                                   0x60
#define COMMAND_CLASS_MULTI_CHANNEL_V4                                                   0x60
#define COMMAND_CLASS_MULTI_CMD                                                          0x8F
#define COMMAND_CLASS_MULTI_INSTANCE_ASSOCIATION                                         0x8E /*Discontinued*/
#define COMMAND_CLASS_MULTI_INSTANCE                                                     0x60 /*Discontinued*/
#define COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY                                           0x52
#define COMMAND_CLASS_NETWORK_MANAGEMENT_PROXY_V2                                        0x52
#define COMMAND_CLASS_NETWORK_MANAGEMENT_BASIC                                           0x4D
#define COMMAND_CLASS_NETWORK_MANAGEMENT_BASIC_V2                                        0x4D
#define COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION                                       0x34
#define COMMAND_CLASS_NETWORK_MANAGEMENT_INCLUSION_V2                                    0x34
#define COMMAND_CLASS_NO_OPERATION                                                       0x00
#define COMMAND_CLASS_NODE_NAMING                                                        0x77
#define COMMAND_CLASS_NON_INTEROPERABLE                                                  0xF0
#define COMMAND_CLASS_POWERLEVEL                                                         0x73
#define COMMAND_CLASS_PREPAYMENT_ENCAPSULATION                                           0x41
#define COMMAND_CLASS_PREPAYMENT                                                         0x3F
#define COMMAND_CLASS_PROPRIETARY                                                        0x88 /*DEPRECATED*/
#define COMMAND_CLASS_PROTECTION                                                         0x75
#define COMMAND_CLASS_PROTECTION_V2                                                      0x75
#define COMMAND_CLASS_RATE_TBL_CONFIG                                                    0x48
#define COMMAND_CLASS_RATE_TBL_MONITOR                                                   0x49
#define COMMAND_CLASS_REMOTE_ASSOCIATION_ACTIVATE                                        0x7C /*OBSOLETED*/
#define COMMAND_CLASS_REMOTE_ASSOCIATION                                                 0x7D /*OBSOLETED*/
#define COMMAND_CLASS_SCENE_ACTIVATION                                                   0x2B
#define COMMAND_CLASS_SCENE_ACTUATOR_CONF                                                0x2C
#define COMMAND_CLASS_SCENE_CONTROLLER_CONF                                              0x2D
#define COMMAND_CLASS_SCHEDULE_ENTRY_LOCK                                                0x4E /*DEPRECATED*/
#define COMMAND_CLASS_SCHEDULE_ENTRY_LOCK_V2                                             0x4E /*DEPRECATED*/
#define COMMAND_CLASS_SCHEDULE_ENTRY_LOCK_V3                                             0x4E /*DEPRECATED*/
#define COMMAND_CLASS_SCREEN_ATTRIBUTES                                                  0x93
#define COMMAND_CLASS_SCREEN_ATTRIBUTES_V2                                               0x93
#define COMMAND_CLASS_SCREEN_MD                                                          0x92
#define COMMAND_CLASS_SCREEN_MD_V2                                                       0x92
#define COMMAND_CLASS_SECURITY_PANEL_MODE                                                0x24
#define COMMAND_CLASS_SECURITY_PANEL_ZONE_SENSOR                                         0x2F
#define COMMAND_CLASS_SECURITY_PANEL_ZONE                                                0x2E
#define COMMAND_CLASS_SECURITY                                                           0x98
#define COMMAND_CLASS_SENSOR_ALARM                                                       0x9C /*SDS10963-4 The Sensor Alarm command class can be used to realize Sensor Alarms.*/
#define COMMAND_CLASS_SENSOR_BINARY                                                      0x30 /*DEPRECATED*/
#define COMMAND_CLASS_SENSOR_BINARY_V2                                                   0x30 /*DEPRECATED*/
#define COMMAND_CLASS_SENSOR_CONFIGURATION                                               0x9E /*This command class adds the possibility for sensors to act on either a measured value or on a*/
#define COMMAND_CLASS_SENSOR_MULTILEVEL                                                  0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V2                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V3                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V4                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V5                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V6                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V7                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V8                                               0x31
#define COMMAND_CLASS_SENSOR_MULTILEVEL_V9                                               0x31
#define COMMAND_CLASS_SILENCE_ALARM                                                      0x9D /*SDS10963-4 The Alarm Silence command class can be used to nuisance silence to temporarily disable the sounding*/
#define COMMAND_CLASS_SIMPLE_AV_CONTROL                                                  0x94
#define COMMAND_CLASS_SWITCH_ALL                                                         0x27
#define COMMAND_CLASS_SWITCH_BINARY                                                      0x25
#define COMMAND_CLASS_SWITCH_BINARY_V2                                                   0x25
#define COMMAND_CLASS_SWITCH_MULTILEVEL                                                  0x26
#define COMMAND_CLASS_SWITCH_MULTILEVEL_V2                                               0x26
#define COMMAND_CLASS_SWITCH_MULTILEVEL_V3                                               0x26
#define COMMAND_CLASS_SWITCH_MULTILEVEL_V4                                               0x26
#define COMMAND_CLASS_SWITCH_TOGGLE_BINARY                                               0x28 /*DEPRECATED*/
#define COMMAND_CLASS_SWITCH_TOGGLE_MULTILEVEL                                           0x29 /*DEPRECATED*/
#define COMMAND_CLASS_TARIFF_CONFIG                                                      0x4A
#define COMMAND_CLASS_TARIFF_TBL_MONITOR                                                 0x4B
#define COMMAND_CLASS_THERMOSTAT_FAN_MODE                                                0x44
#define COMMAND_CLASS_THERMOSTAT_FAN_MODE_V2                                             0x44
#define COMMAND_CLASS_THERMOSTAT_FAN_MODE_V3                                             0x44
#define COMMAND_CLASS_THERMOSTAT_FAN_MODE_V4                                             0x44
#define COMMAND_CLASS_THERMOSTAT_FAN_STATE                                               0x45
#define COMMAND_CLASS_THERMOSTAT_FAN_STATE_V2                                            0x45
#define COMMAND_CLASS_THERMOSTAT_HEATING                                                 0x38
#define COMMAND_CLASS_THERMOSTAT_MODE                                                    0x40
#define COMMAND_CLASS_THERMOSTAT_MODE_V2                                                 0x40
#define COMMAND_CLASS_THERMOSTAT_MODE_V3                                                 0x40
#define COMMAND_CLASS_THERMOSTAT_OPERATING_STATE                                         0x42
#define COMMAND_CLASS_THERMOSTAT_OPERATING_STATE_V2                                      0x42
#define COMMAND_CLASS_THERMOSTAT_SETBACK                                                 0x47
#define COMMAND_CLASS_THERMOSTAT_SETPOINT                                                0x43
#define COMMAND_CLASS_THERMOSTAT_SETPOINT_V2                                             0x43
#define COMMAND_CLASS_THERMOSTAT_SETPOINT_V3                                             0x43
#define COMMAND_CLASS_TIME_PARAMETERS                                                    0x8B
#define COMMAND_CLASS_TIME                                                               0x8A
#define COMMAND_CLASS_TIME_V2                                                            0x8A
#define COMMAND_CLASS_TRANSPORT_SERVICE_V2                                               0x55
#define COMMAND_CLASS_TRANSPORT_SERVICE                                                  0x55
#define COMMAND_CLASS_USER_CODE                                                          0x63
#define COMMAND_CLASS_VERSION                                                            0x86
#define COMMAND_CLASS_VERSION_V2                                                         0x86
#define COMMAND_CLASS_WAKE_UP                                                            0x84
#define COMMAND_CLASS_WAKE_UP_V2                                                         0x84
#define COMMAND_CLASS_ZIP_6LOWPAN                                                        0x4F
#define COMMAND_CLASS_ZIP                                                                0x23 /*OBSOLETED*/
#define COMMAND_CLASS_ZIP_V2                                                             0x23
#define COMMAND_CLASS_ZIP_V3                                                             0x23
#define COMMAND_CLASS_APPLICATION_CAPABILITY                                             0x57
#define COMMAND_CLASS_SWITCH_COLOR                                                       0x33
#define COMMAND_CLASS_SWITCH_COLOR_V2                                                    0x33
#define COMMAND_CLASS_SWITCH_COLOR_V3                                                    0x33
#define COMMAND_CLASS_SCHEDULE                                                           0x53
#define COMMAND_CLASS_SCHEDULE_V2                                                        0x53
#define COMMAND_CLASS_SCHEDULE_V3                                                        0x53
#define COMMAND_CLASS_NETWORK_MANAGEMENT_PRIMARY                                         0x54
#define COMMAND_CLASS_ZIP_ND                                                             0x58
#define COMMAND_CLASS_ASSOCIATION_GRP_INFO                                               0x59
#define COMMAND_CLASS_ASSOCIATION_GRP_INFO_V2                                            0x59
#define COMMAND_CLASS_ASSOCIATION_GRP_INFO_V3                                            0x59
#define COMMAND_CLASS_DEVICE_RESET_LOCALLY                                               0x5A
#define COMMAND_CLASS_CENTRAL_SCENE                                                      0x5B
#define COMMAND_CLASS_CENTRAL_SCENE_V2                                                   0x5B
#define COMMAND_CLASS_CENTRAL_SCENE_V3                                                   0x5B
#define COMMAND_CLASS_IP_ASSOCIATION                                                     0x5C
#define COMMAND_CLASS_ANTITHEFT                                                          0x5D
#define COMMAND_CLASS_ANTITHEFT_V2                                                       0x5D
#define COMMAND_CLASS_ZWAVEPLUS_INFO                                                     0x5E /*SDS11907-3*/
#define COMMAND_CLASS_ZWAVEPLUS_INFO_V2                                                  0x5E /*SDS11907-3*/
#define COMMAND_CLASS_ZIP_GATEWAY                                                        0x5F
#define COMMAND_CLASS_ZIP_PORTAL                                                         0x61
#define COMMAND_CLASS_DMX                                                                0x65
#define COMMAND_CLASS_BARRIER_OPERATOR                                                   0x66
#define COMMAND_CLASS_NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE                        0x67
#define COMMAND_CLASS_ZIP_NAMING                                                         0x68
#define COMMAND_CLASS_MAILBOX                                                            0x69
#define COMMAND_CLASS_WINDOW_COVERING                                                    0x6A
#define COMMAND_CLASS_SECURITY_2                                                         0x9F
#define COMMAND_CLASS_IRRIGATION                                                         0x6B
#define COMMAND_CLASS_SUPERVISION                                                        0x6C
#define COMMAND_CLASS_HUMIDITY_CONTROL_SETPOINT                                          0x64
#define COMMAND_CLASS_HUMIDITY_CONTROL_MODE                                              0x6D
#define COMMAND_CLASS_HUMIDITY_CONTROL_OPERATING_STATE                                   0x6E
#define COMMAND_CLASS_ENTRY_CONTROL                                                      0x6F
#define COMMAND_CLASS_INCLUSION_CONTROLLER                                               0x74
#define COMMAND_CLASS_NODE_PROVISIONING                                                  0x78 /*a.k.a. Node Provisioning Command Class*/

/*************** command class extended identifiers ****************/
#define COMMAND_CLASS_SECURITY_SCHEME0_MARK                                              0xF100

/* Unknown command class commands */
#define UNKNOWN_VERSION                                                                  0x00

/* Alarm command class commands */
#define ALARM_VERSION                                                                    0x01
#define ALARM_GET                                                                        0x04
#define ALARM_REPORT                                                                     0x05

/* Alarm command class commands */
#define ALARM_VERSION_V2                                                                 0x02
#define ALARM_GET_V2                                                                     0x04
#define ALARM_REPORT_V2                                                                  0x05
#define ALARM_SET_V2                                                                     0x06
#define ALARM_TYPE_SUPPORTED_GET_V2                                                      0x07
#define ALARM_TYPE_SUPPORTED_REPORT_V2                                                   0x08
/* Values used for Alarm Get command */
#define ALARM_GET_RESERVED_V2                                                            0x00
#define ALARM_GET_SMOKE_V2                                                               0x01
#define ALARM_GET_CO_V2                                                                  0x02
#define ALARM_GET_CO2_V2                                                                 0x03
#define ALARM_GET_HEAT_V2                                                                0x04
#define ALARM_GET_WATER_V2                                                               0x05
#define ALARM_GET_ACCESS_CONTROL_V2                                                      0x06
#define ALARM_GET_BURGLAR_V2                                                             0x07
#define ALARM_GET_POWER_MANAGEMENT_V2                                                    0x08
#define ALARM_GET_SYSTEM_V2                                                              0x09
#define ALARM_GET_EMERGENCY_V2                                                           0x0A
#define ALARM_GET_CLOCK_V2                                                               0x0B
#define ALARM_GET_FIRST_V2                                                               0xFF
/* Values used for Alarm Report command */
#define ALARM_REPORT_RESERVED_V2                                                         0x00
#define ALARM_REPORT_SMOKE_V2                                                            0x01
#define ALARM_REPORT_CO_V2                                                               0x02
#define ALARM_REPORT_CO2_V2                                                              0x03
#define ALARM_REPORT_HEAT_V2                                                             0x04
#define ALARM_REPORT_WATER_V2                                                            0x05
#define ALARM_REPORT_ACCESS_CONTROL_V2                                                   0x06
#define ALARM_REPORT_BURGLAR_V2                                                          0x07
#define ALARM_REPORT_POWER_MANAGEMENT_V2                                                 0x08
#define ALARM_REPORT_SYSTEM_V2                                                           0x09
#define ALARM_REPORT_EMERGENCY_V2                                                        0x0A
#define ALARM_REPORT_CLOCK_V2                                                            0x0B
#define ALARM_REPORT_FIRST_V2                                                            0xFF
/* Values used for Alarm Set command */
#define ALARM_SET_RESERVED_V2                                                            0x00
#define ALARM_SET_SMOKE_V2                                                               0x01
#define ALARM_SET_CO_V2                                                                  0x02
#define ALARM_SET_CO2_V2                                                                 0x03
#define ALARM_SET_HEAT_V2                                                                0x04
#define ALARM_SET_WATER_V2                                                               0x05
#define ALARM_SET_ACCESS_CONTROL_V2                                                      0x06
#define ALARM_SET_BURGLAR_V2                                                             0x07
#define ALARM_SET_POWER_MANAGEMENT_V2                                                    0x08
#define ALARM_SET_SYSTEM_V2                                                              0x09
#define ALARM_SET_EMERGENCY_V2                                                           0x0A
#define ALARM_SET_CLOCK_V2                                                               0x0B
#define ALARM_SET_FIRST_V2                                                               0xFF
/* Values used for Alarm Type Supported Report command */
#define ALARM_TYPE_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V2              0x1F
#define ALARM_TYPE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V2                         0x60
#define ALARM_TYPE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                        0x05
#define ALARM_TYPE_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V2                     0x80

/* Notification command class commands */
#define NOTIFICATION_VERSION_V3                                                          0x03
#define NOTIFICATION_GET_V3                                                              0x04
#define NOTIFICATION_REPORT_V3                                                           0x05
#define NOTIFICATION_SET_V3                                                              0x06
#define NOTIFICATION_SUPPORTED_GET_V3                                                    0x07
#define NOTIFICATION_SUPPORTED_REPORT_V3                                                 0x08
#define EVENT_SUPPORTED_GET_V3                                                           0x01
#define EVENT_SUPPORTED_REPORT_V3                                                        0x02
/* Values used for Notification Get command */
#define NOTIFICATION_GET_RESERVED_V3                                                     0x00
#define NOTIFICATION_GET_SMOKE_V3                                                        0x01
#define NOTIFICATION_GET_CO_V3                                                           0x02
#define NOTIFICATION_GET_CO2_V3                                                          0x03
#define NOTIFICATION_GET_HEAT_V3                                                         0x04
#define NOTIFICATION_GET_WATER_V3                                                        0x05
#define NOTIFICATION_GET_ACCESS_CONTROL_V3                                               0x06
#define NOTIFICATION_GET_BURGLAR_V3                                                      0x07
#define NOTIFICATION_GET_POWER_MANAGEMENT_V3                                             0x08
#define NOTIFICATION_GET_SYSTEM_V3                                                       0x09
#define NOTIFICATION_GET_EMERGENCY_V3                                                    0x0A
#define NOTIFICATION_GET_CLOCK_V3                                                        0x0B
#define NOTIFICATION_GET_FIRST_V3                                                        0xFF
/* Values used for Notification Report command */
#define NOTIFICATION_REPORT_RESERVED_V3                                                  0x00
#define NOTIFICATION_REPORT_SMOKE_V3                                                     0x01
#define NOTIFICATION_REPORT_CO_V3                                                        0x02
#define NOTIFICATION_REPORT_CO2_V3                                                       0x03
#define NOTIFICATION_REPORT_HEAT_V3                                                      0x04
#define NOTIFICATION_REPORT_WATER_V3                                                     0x05
#define NOTIFICATION_REPORT_ACCESS_CONTROL_V3                                            0x06
#define NOTIFICATION_REPORT_BURGLAR_V3                                                   0x07
#define NOTIFICATION_REPORT_POWER_MANAGEMENT_V3                                          0x08
#define NOTIFICATION_REPORT_SYSTEM_V3                                                    0x09
#define NOTIFICATION_REPORT_EMERGENCY_V3                                                 0x0A
#define NOTIFICATION_REPORT_CLOCK_V3                                                     0x0B
#define NOTIFICATION_REPORT_FIRST_V3                                                     0xFF
#define NOTIFICATION_REPORT_PROPERTIES1_EVENT_PARAMETERS_LENGTH_MASK_V3                  0x1F
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_MASK_V3                                0x60
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_SHIFT_V3                               0x05
#define NOTIFICATION_REPORT_PROPERTIES1_SEQUENCE_BIT_MASK_V3                             0x80
/* Values used for Notification Set command */
#define NOTIFICATION_SET_RESERVED_V3                                                     0x00
#define NOTIFICATION_SET_SMOKE_V3                                                        0x01
#define NOTIFICATION_SET_CO_V3                                                           0x02
#define NOTIFICATION_SET_CO2_V3                                                          0x03
#define NOTIFICATION_SET_HEAT_V3                                                         0x04
#define NOTIFICATION_SET_WATER_V3                                                        0x05
#define NOTIFICATION_SET_ACCESS_CONTROL_V3                                               0x06
#define NOTIFICATION_SET_BURGLAR_V3                                                      0x07
#define NOTIFICATION_SET_POWER_MANAGEMENT_V3                                             0x08
#define NOTIFICATION_SET_SYSTEM_V3                                                       0x09
#define NOTIFICATION_SET_EMERGENCY_V3                                                    0x0A
#define NOTIFICATION_SET_CLOCK_V3                                                        0x0B
#define NOTIFICATION_SET_FIRST_V3                                                        0xFF
/* Values used for Notification Supported Report command */
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V3            0x1F
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V3                       0x60
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                      0x05
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V3                   0x80
/* Values used for Event Supported Get command */
#define EVENT_SUPPORTED_GET_RESERVED_V3                                                  0x00
#define EVENT_SUPPORTED_GET_SMOKE_V3                                                     0x01
#define EVENT_SUPPORTED_GET_CO_V3                                                        0x02
#define EVENT_SUPPORTED_GET_CO2_V3                                                       0x03
#define EVENT_SUPPORTED_GET_HEAT_V3                                                      0x04
#define EVENT_SUPPORTED_GET_WATER_V3                                                     0x05
#define EVENT_SUPPORTED_GET_ACCESS_CONTROL_V3                                            0x06
#define EVENT_SUPPORTED_GET_BURGLAR_V3                                                   0x07
#define EVENT_SUPPORTED_GET_POWER_MANAGEMENT_V3                                          0x08
#define EVENT_SUPPORTED_GET_SYSTEM_V3                                                    0x09
#define EVENT_SUPPORTED_GET_EMERGENCY_V3                                                 0x0A
#define EVENT_SUPPORTED_GET_CLOCK_V3                                                     0x0B
#define EVENT_SUPPORTED_GET_FIRST_V3                                                     0xFF
/* Values used for Event Supported Report command */
#define EVENT_SUPPORTED_REPORT_RESERVED_V3                                               0x00
#define EVENT_SUPPORTED_REPORT_SMOKE_V3                                                  0x01
#define EVENT_SUPPORTED_REPORT_CO_V3                                                     0x02
#define EVENT_SUPPORTED_REPORT_CO2_V3                                                    0x03
#define EVENT_SUPPORTED_REPORT_HEAT_V3                                                   0x04
#define EVENT_SUPPORTED_REPORT_WATER_V3                                                  0x05
#define EVENT_SUPPORTED_REPORT_ACCESS_CONTROL_V3                                         0x06
#define EVENT_SUPPORTED_REPORT_BURGLAR_V3                                                0x07
#define EVENT_SUPPORTED_REPORT_POWER_MANAGEMENT_V3                                       0x08
#define EVENT_SUPPORTED_REPORT_SYSTEM_V3                                                 0x09
#define EVENT_SUPPORTED_REPORT_EMERGENCY_V3                                              0x0A
#define EVENT_SUPPORTED_REPORT_CLOCK_V3                                                  0x0B
#define EVENT_SUPPORTED_REPORT_FIRST_V3                                                  0xFF
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V3                   0x1F
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V3                              0xE0
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                             0x05

/* Notification command class commands */
#define NOTIFICATION_VERSION_V4                                                          0x04
#define NOTIFICATION_GET_V4                                                              0x04
#define NOTIFICATION_REPORT_V4                                                           0x05
#define NOTIFICATION_SET_V4                                                              0x06
#define NOTIFICATION_SUPPORTED_GET_V4                                                    0x07
#define NOTIFICATION_SUPPORTED_REPORT_V4                                                 0x08
#define EVENT_SUPPORTED_GET_V4                                                           0x01
#define EVENT_SUPPORTED_REPORT_V4                                                        0x02
/* Values used for Notification Get command */
#define NOTIFICATION_GET_RESERVED_V4                                                     0x00
#define NOTIFICATION_GET_SMOKE_V4                                                        0x01
#define NOTIFICATION_GET_CO_V4                                                           0x02
#define NOTIFICATION_GET_CO2_V4                                                          0x03
#define NOTIFICATION_GET_HEAT_V4                                                         0x04
#define NOTIFICATION_GET_WATER_V4                                                        0x05
#define NOTIFICATION_GET_ACCESS_CONTROL_V4                                               0x06
#define NOTIFICATION_GET_HOME_SECURITY_V4                                                0x07
#define NOTIFICATION_GET_POWER_MANAGEMENT_V4                                             0x08
#define NOTIFICATION_GET_SYSTEM_V4                                                       0x09
#define NOTIFICATION_GET_EMERGENCY_V4                                                    0x0A
#define NOTIFICATION_GET_CLOCK_V4                                                        0x0B
#define NOTIFICATION_GET_APPLIANCE_V4                                                    0x0C
#define NOTIFICATION_GET_HOME_HEALTH_V4                                                  0x0D
#define NOTIFICATION_GET_FIRST_V4                                                        0xFF
/* Values used for Notification Report command */
#define NOTIFICATION_REPORT_OFF_V4                                                       0x00
#define NOTIFICATION_REPORT_NO_PENDING_NOTIFICATIONS_V4                                  0xFE
#define NOTIFICATION_REPORT_ON_V4                                                        0xFF
#define NOTIFICATION_REPORT_RESERVED_V4                                                  0x00
#define NOTIFICATION_REPORT_SMOKE_V4                                                     0x01
#define NOTIFICATION_REPORT_CO_V4                                                        0x02
#define NOTIFICATION_REPORT_CO2_V4                                                       0x03
#define NOTIFICATION_REPORT_HEAT_V4                                                      0x04
#define NOTIFICATION_REPORT_WATER_V4                                                     0x05
#define NOTIFICATION_REPORT_ACCESS_CONTROL_V4                                            0x06
#define NOTIFICATION_REPORT_HOME_SECURITY_V4                                             0x07
#define NOTIFICATION_REPORT_POWER_MANAGEMENT_V4                                          0x08
#define NOTIFICATION_REPORT_SYSTEM_V4                                                    0x09
#define NOTIFICATION_REPORT_EMERGENCY_V4                                                 0x0A
#define NOTIFICATION_REPORT_CLOCK_V4                                                     0x0B
#define NOTIFICATION_REPORT_APPLIANCE_V4                                                 0x0C
#define NOTIFICATION_REPORT_HOME_HEALTH_V4                                               0x0D
#define NOTIFICATION_REPORT_FIRST_V4                                                     0xFF
#define NOTIFICATION_REPORT_PROPERTIES1_EVENT_PARAMETERS_LENGTH_MASK_V4                  0x1F
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_MASK_V4                                0x60
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_SHIFT_V4                               0x05
#define NOTIFICATION_REPORT_PROPERTIES1_SEQUENCE_BIT_MASK_V4                             0x80
/* Values used for Notification Set command */
#define NOTIFICATION_SET_RESERVED_V4                                                     0x00
#define NOTIFICATION_SET_SMOKE_V4                                                        0x01
#define NOTIFICATION_SET_CO_V4                                                           0x02
#define NOTIFICATION_SET_CO2_V4                                                          0x03
#define NOTIFICATION_SET_HEAT_V4                                                         0x04
#define NOTIFICATION_SET_WATER_V4                                                        0x05
#define NOTIFICATION_SET_ACCESS_CONTROL_V4                                               0x06
#define NOTIFICATION_SET_HOME_SECURITY_V4                                                0x07
#define NOTIFICATION_SET_POWER_MANAGEMENT_V4                                             0x08
#define NOTIFICATION_SET_SYSTEM_V4                                                       0x09
#define NOTIFICATION_SET_EMERGENCY_V4                                                    0x0A
#define NOTIFICATION_SET_CLOCK_V4                                                        0x0B
#define NOTIFICATION_SET_APPLIANCE_V4                                                    0x0C
#define NOTIFICATION_SET_HOME_HEALTH_V4                                                  0x0D
#define NOTIFICATION_SET_FIRST_V4                                                        0xFF
#define NOTIFICATION_SET_OFF_V4                                                          0x00
#define NOTIFICATION_SET_NO_PENDING_NOTIFICATIONS_V4                                     0xFE
#define NOTIFICATION_SET_ON_V4                                                           0xFF
/* Values used for Notification Supported Report command */
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V4            0x1F
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V4                       0x60
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V4                      0x05
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V4                   0x80
/* Values used for Event Supported Get command */
#define EVENT_SUPPORTED_GET_RESERVED_V4                                                  0x00
#define EVENT_SUPPORTED_GET_SMOKE_V4                                                     0x01
#define EVENT_SUPPORTED_GET_CO_V4                                                        0x02
#define EVENT_SUPPORTED_GET_CO2_V4                                                       0x03
#define EVENT_SUPPORTED_GET_HEAT_V4                                                      0x04
#define EVENT_SUPPORTED_GET_WATER_V4                                                     0x05
#define EVENT_SUPPORTED_GET_ACCESS_CONTROL_V4                                            0x06
#define EVENT_SUPPORTED_GET_HOME_SECURITY_V4                                             0x07
#define EVENT_SUPPORTED_GET_POWER_MANAGEMENT_V4                                          0x08
#define EVENT_SUPPORTED_GET_SYSTEM_V4                                                    0x09
#define EVENT_SUPPORTED_GET_EMERGENCY_V4                                                 0x0A
#define EVENT_SUPPORTED_GET_CLOCK_V4                                                     0x0B
#define EVENT_SUPPORTED_GET_APPLIANCE_V4                                                 0x0C
#define EVENT_SUPPORTED_GET_HOME_HEALTH_V4                                               0x0D
#define EVENT_SUPPORTED_GET_FIRST_V4                                                     0xFF
/* Values used for Event Supported Report command */
#define EVENT_SUPPORTED_REPORT_RESERVED_V4                                               0x00
#define EVENT_SUPPORTED_REPORT_SMOKE_V4                                                  0x01
#define EVENT_SUPPORTED_REPORT_CO_V4                                                     0x02
#define EVENT_SUPPORTED_REPORT_CO2_V4                                                    0x03
#define EVENT_SUPPORTED_REPORT_HEAT_V4                                                   0x04
#define EVENT_SUPPORTED_REPORT_WATER_V4                                                  0x05
#define EVENT_SUPPORTED_REPORT_ACCESS_CONTROL_V4                                         0x06
#define EVENT_SUPPORTED_REPORT_HOME_SECURITY_V4                                          0x07
#define EVENT_SUPPORTED_REPORT_POWER_MANAGEMENT_V4                                       0x08
#define EVENT_SUPPORTED_REPORT_SYSTEM_V4                                                 0x09
#define EVENT_SUPPORTED_REPORT_EMERGENCY_V4                                              0x0A
#define EVENT_SUPPORTED_REPORT_CLOCK_V4                                                  0x0B
#define EVENT_SUPPORTED_REPORT_APPLIANCE_V4                                              0x0C
#define EVENT_SUPPORTED_REPORT_HOME_HEALTH_V4                                            0x0D
#define EVENT_SUPPORTED_REPORT_FIRST_V4                                                  0xFF
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V4                   0x1F
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V4                              0xE0
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V4                             0x05

/* Notification command class commands */
#define NOTIFICATION_VERSION_V5                                                          0x05
#define NOTIFICATION_GET_V5                                                              0x04
#define NOTIFICATION_REPORT_V5                                                           0x05
#define NOTIFICATION_SET_V5                                                              0x06
#define NOTIFICATION_SUPPORTED_GET_V5                                                    0x07
#define NOTIFICATION_SUPPORTED_REPORT_V5                                                 0x08
#define EVENT_SUPPORTED_GET_V5                                                           0x01
#define EVENT_SUPPORTED_REPORT_V5                                                        0x02
/* Values used for Notification Get command */
#define NOTIFICATION_GET_RESERVED_V5                                                     0x00
#define NOTIFICATION_GET_SMOKE_V5                                                        0x01
#define NOTIFICATION_GET_CO_V5                                                           0x02
#define NOTIFICATION_GET_CO2_V5                                                          0x03
#define NOTIFICATION_GET_HEAT_V5                                                         0x04
#define NOTIFICATION_GET_WATER_V5                                                        0x05
#define NOTIFICATION_GET_ACCESS_CONTROL_V5                                               0x06
#define NOTIFICATION_GET_HOME_SECURITY_V5                                                0x07
#define NOTIFICATION_GET_POWER_MANAGEMENT_V5                                             0x08
#define NOTIFICATION_GET_SYSTEM_V5                                                       0x09
#define NOTIFICATION_GET_EMERGENCY_V5                                                    0x0A
#define NOTIFICATION_GET_CLOCK_V5                                                        0x0B
#define NOTIFICATION_GET_APPLIANCE_V5                                                    0x0C
#define NOTIFICATION_GET_HOME_HEALTH_V5                                                  0x0D
#define NOTIFICATION_GET_FIRST_V5                                                        0xFF
/* Values used for Notification Report command */
#define NOTIFICATION_REPORT_OFF_V5                                                       0x00
#define NOTIFICATION_REPORT_NO_PENDING_NOTIFICATIONS_V5                                  0xFE
#define NOTIFICATION_REPORT_ON_V5                                                        0xFF
#define NOTIFICATION_REPORT_RESERVED_V5                                                  0x00
#define NOTIFICATION_REPORT_SMOKE_V5                                                     0x01
#define NOTIFICATION_REPORT_CO_V5                                                        0x02
#define NOTIFICATION_REPORT_CO2_V5                                                       0x03
#define NOTIFICATION_REPORT_HEAT_V5                                                      0x04
#define NOTIFICATION_REPORT_WATER_V5                                                     0x05
#define NOTIFICATION_REPORT_ACCESS_CONTROL_V5                                            0x06
#define NOTIFICATION_REPORT_HOME_SECURITY_V5                                             0x07
#define NOTIFICATION_REPORT_POWER_MANAGEMENT_V5                                          0x08
#define NOTIFICATION_REPORT_SYSTEM_V5                                                    0x09
#define NOTIFICATION_REPORT_EMERGENCY_V5                                                 0x0A
#define NOTIFICATION_REPORT_CLOCK_V5                                                     0x0B
#define NOTIFICATION_REPORT_APPLIANCE_V5                                                 0x0C
#define NOTIFICATION_REPORT_HOME_HEALTH_V5                                               0x0D
#define NOTIFICATION_REPORT_FIRST_V5                                                     0xFF
#define NOTIFICATION_REPORT_PROPERTIES1_EVENT_PARAMETERS_LENGTH_MASK_V5                  0x1F
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_MASK_V5                                0x60
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_SHIFT_V5                               0x05
#define NOTIFICATION_REPORT_PROPERTIES1_SEQUENCE_BIT_MASK_V5                             0x80
/* Values used for Notification Set command */
#define NOTIFICATION_SET_RESERVED_V5                                                     0x00
#define NOTIFICATION_SET_SMOKE_V5                                                        0x01
#define NOTIFICATION_SET_CO_V5                                                           0x02
#define NOTIFICATION_SET_CO2_V5                                                          0x03
#define NOTIFICATION_SET_HEAT_V5                                                         0x04
#define NOTIFICATION_SET_WATER_V5                                                        0x05
#define NOTIFICATION_SET_ACCESS_CONTROL_V5                                               0x06
#define NOTIFICATION_SET_HOME_SECURITY_V5                                                0x07
#define NOTIFICATION_SET_POWER_MANAGEMENT_V5                                             0x08
#define NOTIFICATION_SET_SYSTEM_V5                                                       0x09
#define NOTIFICATION_SET_EMERGENCY_V5                                                    0x0A
#define NOTIFICATION_SET_CLOCK_V5                                                        0x0B
#define NOTIFICATION_SET_APPLIANCE_V5                                                    0x0C
#define NOTIFICATION_SET_HOME_HEALTH_V5                                                  0x0D
#define NOTIFICATION_SET_FIRST_V5                                                        0xFF
#define NOTIFICATION_SET_OFF_V5                                                          0x00
#define NOTIFICATION_SET_NO_PENDING_NOTIFICATIONS_V5                                     0xFE
#define NOTIFICATION_SET_ON_V5                                                           0xFF
/* Values used for Notification Supported Report command */
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V5            0x1F
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V5                       0x60
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V5                      0x05
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V5                   0x80
/* Values used for Event Supported Get command */
#define EVENT_SUPPORTED_GET_RESERVED_V5                                                  0x00
#define EVENT_SUPPORTED_GET_SMOKE_V5                                                     0x01
#define EVENT_SUPPORTED_GET_CO_V5                                                        0x02
#define EVENT_SUPPORTED_GET_CO2_V5                                                       0x03
#define EVENT_SUPPORTED_GET_HEAT_V5                                                      0x04
#define EVENT_SUPPORTED_GET_WATER_V5                                                     0x05
#define EVENT_SUPPORTED_GET_ACCESS_CONTROL_V5                                            0x06
#define EVENT_SUPPORTED_GET_HOME_SECURITY_V5                                             0x07
#define EVENT_SUPPORTED_GET_POWER_MANAGEMENT_V5                                          0x08
#define EVENT_SUPPORTED_GET_SYSTEM_V5                                                    0x09
#define EVENT_SUPPORTED_GET_EMERGENCY_V5                                                 0x0A
#define EVENT_SUPPORTED_GET_CLOCK_V5                                                     0x0B
#define EVENT_SUPPORTED_GET_APPLIANCE_V5                                                 0x0C
#define EVENT_SUPPORTED_GET_HOME_HEALTH_V5                                               0x0D
#define EVENT_SUPPORTED_GET_FIRST_V5                                                     0xFF
/* Values used for Event Supported Report command */
#define EVENT_SUPPORTED_REPORT_RESERVED_V5                                               0x00
#define EVENT_SUPPORTED_REPORT_SMOKE_V5                                                  0x01
#define EVENT_SUPPORTED_REPORT_CO_V5                                                     0x02
#define EVENT_SUPPORTED_REPORT_CO2_V5                                                    0x03
#define EVENT_SUPPORTED_REPORT_HEAT_V5                                                   0x04
#define EVENT_SUPPORTED_REPORT_WATER_V5                                                  0x05
#define EVENT_SUPPORTED_REPORT_ACCESS_CONTROL_V5                                         0x06
#define EVENT_SUPPORTED_REPORT_HOME_SECURITY_V5                                          0x07
#define EVENT_SUPPORTED_REPORT_POWER_MANAGEMENT_V5                                       0x08
#define EVENT_SUPPORTED_REPORT_SYSTEM_V5                                                 0x09
#define EVENT_SUPPORTED_REPORT_EMERGENCY_V5                                              0x0A
#define EVENT_SUPPORTED_REPORT_CLOCK_V5                                                  0x0B
#define EVENT_SUPPORTED_REPORT_APPLIANCE_V5                                              0x0C
#define EVENT_SUPPORTED_REPORT_HOME_HEALTH_V5                                            0x0D
#define EVENT_SUPPORTED_REPORT_FIRST_V5                                                  0xFF
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V5                   0x1F
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V5                              0xE0
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V5                             0x05

/* Notification command class commands */
#define NOTIFICATION_VERSION_V6                                                          0x06
#define NOTIFICATION_GET_V6                                                              0x04
#define NOTIFICATION_REPORT_V6                                                           0x05
#define NOTIFICATION_SET_V6                                                              0x06
#define NOTIFICATION_SUPPORTED_GET_V6                                                    0x07
#define NOTIFICATION_SUPPORTED_REPORT_V6                                                 0x08
#define EVENT_SUPPORTED_GET_V6                                                           0x01
#define EVENT_SUPPORTED_REPORT_V6                                                        0x02
/* Values used for Notification Get command */
#define NOTIFICATION_GET_RESERVED_V6                                                     0x00
#define NOTIFICATION_GET_SMOKE_V6                                                        0x01
#define NOTIFICATION_GET_CO_V6                                                           0x02
#define NOTIFICATION_GET_CO2_V6                                                          0x03
#define NOTIFICATION_GET_HEAT_V6                                                         0x04
#define NOTIFICATION_GET_WATER_V6                                                        0x05
#define NOTIFICATION_GET_ACCESS_CONTROL_V6                                               0x06
#define NOTIFICATION_GET_HOME_SECURITY_V6                                                0x07
#define NOTIFICATION_GET_POWER_MANAGEMENT_V6                                             0x08
#define NOTIFICATION_GET_SYSTEM_V6                                                       0x09
#define NOTIFICATION_GET_EMERGENCY_V6                                                    0x0A
#define NOTIFICATION_GET_CLOCK_V6                                                        0x0B
#define NOTIFICATION_GET_APPLIANCE_V6                                                    0x0C
#define NOTIFICATION_GET_HOME_HEALTH_V6                                                  0x0D
#define NOTIFICATION_GET_SIREN_V6                                                        0x0E
#define NOTIFICATION_GET_FIRST_V6                                                        0xFF
/* Values used for Notification Report command */
#define NOTIFICATION_REPORT_OFF_V6                                                       0x00
#define NOTIFICATION_REPORT_NO_PENDING_NOTIFICATIONS_V6                                  0xFE
#define NOTIFICATION_REPORT_ON_V6                                                        0xFF
#define NOTIFICATION_REPORT_RESERVED_V6                                                  0x00
#define NOTIFICATION_REPORT_SMOKE_V6                                                     0x01
#define NOTIFICATION_REPORT_CO_V6                                                        0x02
#define NOTIFICATION_REPORT_CO2_V6                                                       0x03
#define NOTIFICATION_REPORT_HEAT_V6                                                      0x04
#define NOTIFICATION_REPORT_WATER_V6                                                     0x05
#define NOTIFICATION_REPORT_ACCESS_CONTROL_V6                                            0x06
#define NOTIFICATION_REPORT_HOME_SECURITY_V6                                             0x07
#define NOTIFICATION_REPORT_POWER_MANAGEMENT_V6                                          0x08
#define NOTIFICATION_REPORT_SYSTEM_V6                                                    0x09
#define NOTIFICATION_REPORT_EMERGENCY_V6                                                 0x0A
#define NOTIFICATION_REPORT_CLOCK_V6                                                     0x0B
#define NOTIFICATION_REPORT_APPLIANCE_V6                                                 0x0C
#define NOTIFICATION_REPORT_HOME_HEALTH_V6                                               0x0D
#define NOTIFICATION_REPORT_SIREN_V6                                                     0x0E
#define NOTIFICATION_REPORT_FIRST_V6                                                     0xFF
#define NOTIFICATION_REPORT_PROPERTIES1_EVENT_PARAMETERS_LENGTH_MASK_V6                  0x1F
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_MASK_V6                                0x60
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_SHIFT_V6                               0x05
#define NOTIFICATION_REPORT_PROPERTIES1_SEQUENCE_BIT_MASK_V6                             0x80
/* Values used for Notification Set command */
#define NOTIFICATION_SET_RESERVED_V6                                                     0x00
#define NOTIFICATION_SET_SMOKE_V6                                                        0x01
#define NOTIFICATION_SET_CO_V6                                                           0x02
#define NOTIFICATION_SET_CO2_V6                                                          0x03
#define NOTIFICATION_SET_HEAT_V6                                                         0x04
#define NOTIFICATION_SET_WATER_V6                                                        0x05
#define NOTIFICATION_SET_ACCESS_CONTROL_V6                                               0x06
#define NOTIFICATION_SET_HOME_SECURITY_V6                                                0x07
#define NOTIFICATION_SET_POWER_MANAGEMENT_V6                                             0x08
#define NOTIFICATION_SET_SYSTEM_V6                                                       0x09
#define NOTIFICATION_SET_EMERGENCY_V6                                                    0x0A
#define NOTIFICATION_SET_CLOCK_V6                                                        0x0B
#define NOTIFICATION_SET_APPLIANCE_V6                                                    0x0C
#define NOTIFICATION_SET_HOME_HEALTH_V6                                                  0x0D
#define NOTIFICATION_SET_SIREN_V6                                                        0x0E
#define NOTIFICATION_SET_FIRST_V6                                                        0xFF
#define NOTIFICATION_SET_OFF_V6                                                          0x00
#define NOTIFICATION_SET_NO_PENDING_NOTIFICATIONS_V6                                     0xFE
#define NOTIFICATION_SET_ON_V6                                                           0xFF
/* Values used for Notification Supported Report command */
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V6            0x1F
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V6                       0x60
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V6                      0x05
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V6                   0x80
/* Values used for Event Supported Get command */
#define EVENT_SUPPORTED_GET_RESERVED_V6                                                  0x00
#define EVENT_SUPPORTED_GET_SMOKE_V6                                                     0x01
#define EVENT_SUPPORTED_GET_CO_V6                                                        0x02
#define EVENT_SUPPORTED_GET_CO2_V6                                                       0x03
#define EVENT_SUPPORTED_GET_HEAT_V6                                                      0x04
#define EVENT_SUPPORTED_GET_WATER_V6                                                     0x05
#define EVENT_SUPPORTED_GET_ACCESS_CONTROL_V6                                            0x06
#define EVENT_SUPPORTED_GET_HOME_SECURITY_V6                                             0x07
#define EVENT_SUPPORTED_GET_POWER_MANAGEMENT_V6                                          0x08
#define EVENT_SUPPORTED_GET_SYSTEM_V6                                                    0x09
#define EVENT_SUPPORTED_GET_EMERGENCY_V6                                                 0x0A
#define EVENT_SUPPORTED_GET_CLOCK_V6                                                     0x0B
#define EVENT_SUPPORTED_GET_APPLIANCE_V6                                                 0x0C
#define EVENT_SUPPORTED_GET_HOME_HEALTH_V6                                               0x0D
#define EVENT_SUPPORTED_GET_SIREN_V6                                                     0x0E
#define EVENT_SUPPORTED_GET_FIRST_V6                                                     0xFF
/* Values used for Event Supported Report command */
#define EVENT_SUPPORTED_REPORT_RESERVED_V6                                               0x00
#define EVENT_SUPPORTED_REPORT_SMOKE_V6                                                  0x01
#define EVENT_SUPPORTED_REPORT_CO_V6                                                     0x02
#define EVENT_SUPPORTED_REPORT_CO2_V6                                                    0x03
#define EVENT_SUPPORTED_REPORT_HEAT_V6                                                   0x04
#define EVENT_SUPPORTED_REPORT_WATER_V6                                                  0x05
#define EVENT_SUPPORTED_REPORT_ACCESS_CONTROL_V6                                         0x06
#define EVENT_SUPPORTED_REPORT_HOME_SECURITY_V6                                          0x07
#define EVENT_SUPPORTED_REPORT_POWER_MANAGEMENT_V6                                       0x08
#define EVENT_SUPPORTED_REPORT_SYSTEM_V6                                                 0x09
#define EVENT_SUPPORTED_REPORT_EMERGENCY_V6                                              0x0A
#define EVENT_SUPPORTED_REPORT_CLOCK_V6                                                  0x0B
#define EVENT_SUPPORTED_REPORT_APPLIANCE_V6                                              0x0C
#define EVENT_SUPPORTED_REPORT_HOME_HEALTH_V6                                            0x0D
#define EVENT_SUPPORTED_REPORT_SIREN_V6                                                  0x0E
#define EVENT_SUPPORTED_REPORT_FIRST_V6                                                  0xFF
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V6                   0x1F
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V6                              0xE0
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V6                             0x05

/* Notification command class commands */
#define NOTIFICATION_VERSION_V7                                                          0x07
#define NOTIFICATION_GET_V7                                                              0x04
#define NOTIFICATION_REPORT_V7                                                           0x05
#define NOTIFICATION_SET_V7                                                              0x06
#define NOTIFICATION_SUPPORTED_GET_V7                                                    0x07
#define NOTIFICATION_SUPPORTED_REPORT_V7                                                 0x08
#define EVENT_SUPPORTED_GET_V7                                                           0x01
#define EVENT_SUPPORTED_REPORT_V7                                                        0x02
/* Values used for Notification Get command */
#define NOTIFICATION_GET_RESERVED_V7                                                     0x00
#define NOTIFICATION_GET_SMOKE_V7                                                        0x01
#define NOTIFICATION_GET_CO_V7                                                           0x02
#define NOTIFICATION_GET_CO2_V7                                                          0x03
#define NOTIFICATION_GET_HEAT_V7                                                         0x04
#define NOTIFICATION_GET_WATER_V7                                                        0x05
#define NOTIFICATION_GET_ACCESS_CONTROL_V7                                               0x06
#define NOTIFICATION_GET_HOME_SECURITY_V7                                                0x07
#define NOTIFICATION_GET_POWER_MANAGEMENT_V7                                             0x08
#define NOTIFICATION_GET_SYSTEM_V7                                                       0x09
#define NOTIFICATION_GET_EMERGENCY_V7                                                    0x0A
#define NOTIFICATION_GET_CLOCK_V7                                                        0x0B
#define NOTIFICATION_GET_APPLIANCE_V7                                                    0x0C
#define NOTIFICATION_GET_HOME_HEALTH_V7                                                  0x0D
#define NOTIFICATION_GET_SIREN_V7                                                        0x0E
#define NOTIFICATION_GET_WATER_VALVE_V7                                                  0x0F
#define NOTIFICATION_GET_WEATHER_ALARM_V7                                                0x10
#define NOTIFICATION_GET_IRRIGATION_V7                                                   0x11
#define NOTIFICATION_GET_GAS_ALARM_V7                                                    0x12
#define NOTIFICATION_GET_FIRST_V7                                                        0xFF
/* Values used for Notification Report command */
#define NOTIFICATION_REPORT_OFF_V7                                                       0x00
#define NOTIFICATION_REPORT_NO_PENDING_NOTIFICATIONS_V7                                  0xFE
#define NOTIFICATION_REPORT_ON_V7                                                        0xFF
#define NOTIFICATION_REPORT_RESERVED_V7                                                  0x00
#define NOTIFICATION_REPORT_SMOKE_V7                                                     0x01
#define NOTIFICATION_REPORT_CO_V7                                                        0x02
#define NOTIFICATION_REPORT_CO2_V7                                                       0x03
#define NOTIFICATION_REPORT_HEAT_V7                                                      0x04
#define NOTIFICATION_REPORT_WATER_V7                                                     0x05
#define NOTIFICATION_REPORT_ACCESS_CONTROL_V7                                            0x06
#define NOTIFICATION_REPORT_HOME_SECURITY_V7                                             0x07
#define NOTIFICATION_REPORT_POWER_MANAGEMENT_V7                                          0x08
#define NOTIFICATION_REPORT_SYSTEM_V7                                                    0x09
#define NOTIFICATION_REPORT_EMERGENCY_V7                                                 0x0A
#define NOTIFICATION_REPORT_CLOCK_V7                                                     0x0B
#define NOTIFICATION_REPORT_APPLIANCE_V7                                                 0x0C
#define NOTIFICATION_REPORT_HOME_HEALTH_V7                                               0x0D
#define NOTIFICATION_REPORT_SIREN_V7                                                     0x0E
#define NOTIFICATION_REPORT_WATER_VALVE_V7                                               0x0F
#define NOTIFICATION_REPORT_WEATHER_ALARM_V7                                             0x10
#define NOTIFICATION_REPORT_IRRIGATION_V7                                                0x11
#define NOTIFICATION_REPORT_GAS_ALARM_V7                                                 0x12
#define NOTIFICATION_REPORT_FIRST_V7                                                     0xFF
#define NOTIFICATION_REPORT_PROPERTIES1_EVENT_PARAMETERS_LENGTH_MASK_V7                  0x1F
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_MASK_V7                                0x60
#define NOTIFICATION_REPORT_PROPERTIES1_RESERVED2_SHIFT_V7                               0x05
#define NOTIFICATION_REPORT_PROPERTIES1_SEQUENCE_BIT_MASK_V7                             0x80
/* Values used for Notification Set command */
#define NOTIFICATION_SET_RESERVED_V7                                                     0x00
#define NOTIFICATION_SET_SMOKE_V7                                                        0x01
#define NOTIFICATION_SET_CO_V7                                                           0x02
#define NOTIFICATION_SET_CO2_V7                                                          0x03
#define NOTIFICATION_SET_HEAT_V7                                                         0x04
#define NOTIFICATION_SET_WATER_V7                                                        0x05
#define NOTIFICATION_SET_ACCESS_CONTROL_V7                                               0x06
#define NOTIFICATION_SET_HOME_SECURITY_V7                                                0x07
#define NOTIFICATION_SET_POWER_MANAGEMENT_V7                                             0x08
#define NOTIFICATION_SET_SYSTEM_V7                                                       0x09
#define NOTIFICATION_SET_EMERGENCY_V7                                                    0x0A
#define NOTIFICATION_SET_CLOCK_V7                                                        0x0B
#define NOTIFICATION_SET_APPLIANCE_V7                                                    0x0C
#define NOTIFICATION_SET_HOME_HEALTH_V7                                                  0x0D
#define NOTIFICATION_SET_SIREN_V7                                                        0x0E
#define NOTIFICATION_SET_WATER_VALVE_V7                                                  0x0F
#define NOTIFICATION_SET_WEATHER_ALARM_V7                                                0x10
#define NOTIFICATION_SET_IRRIGATION_V7                                                   0x11
#define NOTIFICATION_SET_GAS_ALARM_V7                                                    0x12
#define NOTIFICATION_SET_FIRST_V7                                                        0xFF
#define NOTIFICATION_SET_OFF_V7                                                          0x00
#define NOTIFICATION_SET_NO_PENDING_NOTIFICATIONS_V7                                     0xFE
#define NOTIFICATION_SET_ON_V7                                                           0xFF
/* Values used for Notification Supported Report command */
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V7            0x1F
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V7                       0x60
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V7                      0x05
#define NOTIFICATION_SUPPORTED_REPORT_PROPERTIES1_V1_ALARM_BIT_MASK_V7                   0x80
/* Values used for Event Supported Get command */
#define EVENT_SUPPORTED_GET_RESERVED_V7                                                  0x00
#define EVENT_SUPPORTED_GET_SMOKE_V7                                                     0x01
#define EVENT_SUPPORTED_GET_CO_V7                                                        0x02
#define EVENT_SUPPORTED_GET_CO2_V7                                                       0x03
#define EVENT_SUPPORTED_GET_HEAT_V7                                                      0x04
#define EVENT_SUPPORTED_GET_WATER_V7                                                     0x05
#define EVENT_SUPPORTED_GET_ACCESS_CONTROL_V7                                            0x06
#define EVENT_SUPPORTED_GET_HOME_SECURITY_V7                                             0x07
#define EVENT_SUPPORTED_GET_POWER_MANAGEMENT_V7                                          0x08
#define EVENT_SUPPORTED_GET_SYSTEM_V7                                                    0x09
#define EVENT_SUPPORTED_GET_EMERGENCY_V7                                                 0x0A
#define EVENT_SUPPORTED_GET_CLOCK_V7                                                     0x0B
#define EVENT_SUPPORTED_GET_APPLIANCE_V7                                                 0x0C
#define EVENT_SUPPORTED_GET_HOME_HEALTH_V7                                               0x0D
#define EVENT_SUPPORTED_GET_SIREN_V7                                                     0x0E
#define EVENT_SUPPORTED_GET_WATER_VALVE_V7                                               0x0F
#define EVENT_SUPPORTED_GET_WEATHER_ALARM_V7                                             0x10
#define EVENT_SUPPORTED_GET_IRRIGATION_V7                                                0x11
#define EVENT_SUPPORTED_GET_GAS_ALARM_V7                                                 0x12
#define EVENT_SUPPORTED_GET_FIRST_V7                                                     0xFF
/* Values used for Event Supported Report command */
#define EVENT_SUPPORTED_REPORT_RESERVED_V7                                               0x00
#define EVENT_SUPPORTED_REPORT_SMOKE_V7                                                  0x01
#define EVENT_SUPPORTED_REPORT_CO_V7                                                     0x02
#define EVENT_SUPPORTED_REPORT_CO2_V7                                                    0x03
#define EVENT_SUPPORTED_REPORT_HEAT_V7                                                   0x04
#define EVENT_SUPPORTED_REPORT_WATER_V7                                                  0x05
#define EVENT_SUPPORTED_REPORT_ACCESS_CONTROL_V7                                         0x06
#define EVENT_SUPPORTED_REPORT_HOME_SECURITY_V7                                          0x07
#define EVENT_SUPPORTED_REPORT_POWER_MANAGEMENT_V7                                       0x08
#define EVENT_SUPPORTED_REPORT_SYSTEM_V7                                                 0x09
#define EVENT_SUPPORTED_REPORT_EMERGENCY_V7                                              0x0A
#define EVENT_SUPPORTED_REPORT_CLOCK_V7                                                  0x0B
#define EVENT_SUPPORTED_REPORT_APPLIANCE_V7                                              0x0C
#define EVENT_SUPPORTED_REPORT_HOME_HEALTH_V7                                            0x0D
#define EVENT_SUPPORTED_REPORT_SIREN_V7                                                  0x0E
#define EVENT_SUPPORTED_REPORT_WATER_VALVE_V7                                            0x0F
#define EVENT_SUPPORTED_REPORT_WEATHER_ALARM_V7                                          0x10
#define EVENT_SUPPORTED_REPORT_IRRIGATION_V7                                             0x11
#define EVENT_SUPPORTED_REPORT_GAS_ALARM_V7                                              0x12
#define EVENT_SUPPORTED_REPORT_FIRST_V7                                                  0xFF
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASKS_MASK_V7                   0x1F
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V7                              0xE0
#define EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V7                             0x05

/* Application Status command class commands */
#define APPLICATION_STATUS_VERSION                                                       0x01
#define APPLICATION_BUSY                                                                 0x01
#define APPLICATION_REJECTED_REQUEST                                                     0x02
/* Values used for Application Busy command */
#define APPLICATION_BUSY_TRY_AGAIN_LATER                                                 0x00
#define APPLICATION_BUSY_TRY_AGAIN_IN_WAIT_TIME_SECONDS                                  0x01
#define APPLICATION_BUSY_REQUEST_QUEUED_EXECUTED_LATER                                   0x02

/* Association Command Configuration command class commands */
#define ASSOCIATION_COMMAND_CONFIGURATION_VERSION                                        0x01
#define COMMAND_CONFIGURATION_GET                                                        0x04
#define COMMAND_CONFIGURATION_REPORT                                                     0x05
#define COMMAND_CONFIGURATION_SET                                                        0x03
#define COMMAND_RECORDS_SUPPORTED_GET                                                    0x01
#define COMMAND_RECORDS_SUPPORTED_REPORT                                                 0x02
/* Values used for Command Configuration Report command */
#define COMMAND_CONFIGURATION_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_MASK                  0x0F
#define COMMAND_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_MASK                           0x70
#define COMMAND_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_SHIFT                          0x04
#define COMMAND_CONFIGURATION_REPORT_PROPERTIES1_FIRST_BIT_MASK                          0x80
/* Values used for Command Records Supported Report command */
#define COMMAND_RECORDS_SUPPORTED_REPORT_PROPERTIES1_CONF_CMD_BIT_MASK                   0x01
#define COMMAND_RECORDS_SUPPORTED_REPORT_PROPERTIES1_V_C_BIT_MASK                        0x02
#define COMMAND_RECORDS_SUPPORTED_REPORT_PROPERTIES1_MAX_COMMAND_LENGTH_MASK             0xFC
#define COMMAND_RECORDS_SUPPORTED_REPORT_PROPERTIES1_MAX_COMMAND_LENGTH_SHIFT            0x02

/* Association command class commands */
#define ASSOCIATION_VERSION                                                              0x01
#define ASSOCIATION_GET                                                                  0x02
#define ASSOCIATION_GROUPINGS_GET                                                        0x05
#define ASSOCIATION_GROUPINGS_REPORT                                                     0x06
#define ASSOCIATION_REMOVE                                                               0x04
#define ASSOCIATION_REPORT                                                               0x03
#define ASSOCIATION_SET                                                                  0x01

/* Association command class commands */
#define ASSOCIATION_VERSION_V2                                                           0x02
#define ASSOCIATION_GET_V2                                                               0x02
#define ASSOCIATION_GROUPINGS_GET_V2                                                     0x05
#define ASSOCIATION_GROUPINGS_REPORT_V2                                                  0x06
#define ASSOCIATION_REMOVE_V2                                                            0x04
#define ASSOCIATION_REPORT_V2                                                            0x03
#define ASSOCIATION_SET_V2                                                               0x01
#define ASSOCIATION_SPECIFIC_GROUP_GET_V2                                                0x0B
#define ASSOCIATION_SPECIFIC_GROUP_REPORT_V2                                             0x0C

/* Av Content Directory Md command class commands */
#define AV_CONTENT_DIRECTORY_MD_VERSION                                                  0x01
#define AV_CONTENT_BROWSE_MD_BY_LETTER_GET                                               0x03
#define AV_CONTENT_BROWSE_MD_BY_LETTER_REPORT                                            0x04
#define AV_CONTENT_BROWSE_MD_CHILD_COUNT_GET                                             0x05
#define AV_CONTENT_BROWSE_MD_CHILD_COUNT_REPORT                                          0x06
#define AV_CONTENT_BROWSE_MD_GET                                                         0x01
#define AV_CONTENT_BROWSE_MD_REPORT                                                      0x02
#define AV_MATCH_ITEM_TO_RENDERER_MD_GET                                                 0x07
#define AV_MATCH_ITEM_TO_RENDERER_MD_REPORT                                              0x08

/* Av Content Search Md command class commands */
#define AV_CONTENT_SEARCH_MD_VERSION                                                     0x01
#define AV_CONTENT_SEARCH_MD_GET                                                         0x01
#define AV_CONTENT_SEARCH_MD_REPORT                                                      0x02

/* Av Renderer Status command class commands */
#define AV_RENDERER_STATUS_VERSION                                                       0x01
#define AV_RENDERER_STATUS_GET                                                           0x01
#define AV_RENDERER_STATUS_REPORT                                                        0x02

/* Av Tagging Md command class commands */
#define AV_TAGGING_MD_VERSION                                                            0x01
#define AV_TAGGING_MD_GET                                                                0x01
#define AV_TAGGING_MD_REPORT                                                             0x02

/* Basic Tariff Info command class commands */
#define BASIC_TARIFF_INFO_VERSION                                                        0x01
#define BASIC_TARIFF_INFO_GET                                                            0x01
#define BASIC_TARIFF_INFO_REPORT                                                         0x02
/* Values used for Basic Tariff Info Report command */
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES1_TOTAL_NO_IMPORT_RATES_MASK                  0x0F
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES1_RESERVED1_MASK                              0x70
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES1_RESERVED1_SHIFT                             0x04
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES1_DUAL_BIT_MASK                               0x80
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES2_E1_CURRENT_RATE_IN_USE_MASK                 0x0F
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES2_RESERVED2_MASK                              0xF0
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES2_RESERVED2_SHIFT                             0x04
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES3_E2_CURRENT_RATE_IN_USE_MASK                 0x0F
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES3_RESERVED3_MASK                              0xF0
#define BASIC_TARIFF_INFO_REPORT_PROPERTIES3_RESERVED3_SHIFT                             0x04

/* Basic Window Covering command class commands */
#define BASIC_WINDOW_COVERING_VERSION                                                    0x01
#define BASIC_WINDOW_COVERING_START_LEVEL_CHANGE                                         0x01
#define BASIC_WINDOW_COVERING_STOP_LEVEL_CHANGE                                          0x02
/* Values used for Basic Window Covering Start Level Change command */
#define BASIC_WINDOW_COVERING_START_LEVEL_CHANGE_LEVEL_RESERVED1_MASK                    0x3F
#define BASIC_WINDOW_COVERING_START_LEVEL_CHANGE_LEVEL_OPEN_CLOSE_BIT_MASK               0x40
#define BASIC_WINDOW_COVERING_START_LEVEL_CHANGE_LEVEL_RESERVED2_BIT_MASK                0x80

/* Basic command class commands */
#define BASIC_VERSION                                                                    0x01
#define BASIC_GET                                                                        0x02
#define BASIC_REPORT                                                                     0x03
#define BASIC_SET                                                                        0x01

/* Basic command class commands */
#define BASIC_VERSION_V2                                                                 0x02
#define BASIC_GET_V2                                                                     0x02
#define BASIC_REPORT_V2                                                                  0x03
#define BASIC_SET_V2                                                                     0x01

/* Battery command class commands */
#define BATTERY_VERSION                                                                  0x01
#define BATTERY_GET                                                                      0x02
#define BATTERY_REPORT                                                                   0x03

/* Chimney Fan command class commands */
#define CHIMNEY_FAN_VERSION                                                              0x01
#define CHIMNEY_FAN_ALARM_LOG_GET                                                        0x20
#define CHIMNEY_FAN_ALARM_LOG_REPORT                                                     0x21
#define CHIMNEY_FAN_ALARM_LOG_SET                                                        0x1F
#define CHIMNEY_FAN_ALARM_STATUS_GET                                                     0x23
#define CHIMNEY_FAN_ALARM_STATUS_REPORT                                                  0x24
#define CHIMNEY_FAN_ALARM_STATUS_SET                                                     0x22
#define CHIMNEY_FAN_ALARM_TEMP_GET                                                       0x0E
#define CHIMNEY_FAN_ALARM_TEMP_REPORT                                                    0x0F
#define CHIMNEY_FAN_ALARM_TEMP_SET                                                       0x0D
#define CHIMNEY_FAN_BOOST_TIME_GET                                                       0x11
#define CHIMNEY_FAN_BOOST_TIME_REPORT                                                    0x12
#define CHIMNEY_FAN_BOOST_TIME_SET                                                       0x10
#define CHIMNEY_FAN_DEFAULT_SET                                                          0x28
#define CHIMNEY_FAN_MIN_SPEED_GET                                                        0x26
#define CHIMNEY_FAN_MIN_SPEED_REPORT                                                     0x27
#define CHIMNEY_FAN_MIN_SPEED_SET                                                        0x25
#define CHIMNEY_FAN_MODE_GET                                                             0x17
#define CHIMNEY_FAN_MODE_REPORT                                                          0x18
#define CHIMNEY_FAN_MODE_SET                                                             0x16
#define CHIMNEY_FAN_SETUP_GET                                                            0x1A
#define CHIMNEY_FAN_SETUP_REPORT                                                         0x1B
#define CHIMNEY_FAN_SETUP_SET                                                            0x19
#define CHIMNEY_FAN_SPEED_GET                                                            0x05
#define CHIMNEY_FAN_SPEED_REPORT                                                         0x06
#define CHIMNEY_FAN_SPEED_SET                                                            0x04
#define CHIMNEY_FAN_START_TEMP_GET                                                       0x08
#define CHIMNEY_FAN_START_TEMP_REPORT                                                    0x09
#define CHIMNEY_FAN_START_TEMP_SET                                                       0x07
#define CHIMNEY_FAN_STATE_GET                                                            0x02
#define CHIMNEY_FAN_STATE_REPORT                                                         0x03
#define CHIMNEY_FAN_STATE_SET                                                            0x01
#define CHIMNEY_FAN_STATUS_GET                                                           0x1D
#define CHIMNEY_FAN_STATUS_REPORT                                                        0x1E
#define CHIMNEY_FAN_STOP_TEMP_GET                                                        0x0B
#define CHIMNEY_FAN_STOP_TEMP_REPORT                                                     0x0C
#define CHIMNEY_FAN_STOP_TEMP_SET                                                        0x0A
#define CHIMNEY_FAN_STOP_TIME_GET                                                        0x14
#define CHIMNEY_FAN_STOP_TIME_REPORT                                                     0x15
#define CHIMNEY_FAN_STOP_TIME_SET                                                        0x13
/* Values used for Chimney Fan Alarm Log Report command */
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_RESERVED11_BIT_MASK                   0x01
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_EXTERNAL_ALARM_1_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_SENSOR_ERROR_1_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_ALARM_TEMPERATURE_EXCEEDED_1_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_RESERVED12_MASK                       0x70
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_RESERVED12_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_1_ALARM_STILL_ACTIVE_1_BIT_MASK         0x80
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_RESERVED21_BIT_MASK                   0x01
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_EXTERNAL_ALARM_2_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_SENSOR_ERROR_2_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_ALARM_TEMPERATURE_EXCEEDED_2_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_RESERVED22_MASK                       0x70
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_RESERVED22_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_2_ALARM_STILL_ACTIVE_2_BIT_MASK         0x80
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_RESERVED31_BIT_MASK                   0x01
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_EXTERNAL_ALARM_3_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_SENSOR_ERROR_3_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_ALARM_TEMPERATURE_EXCEEDED_3_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_RESERVED32_MASK                       0x70
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_RESERVED32_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_3_ALARM_STILL_ACTIVE_3_BIT_MASK         0x80
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_RESERVED41_BIT_MASK                   0x01
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_EXTERNAL_ALARM_4_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_SENSOR_ERROR_4_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_ALARM_TEMPERATURE_EXCEEDED_4_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_RESERVED42_MASK                       0x70
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_RESERVED42_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_4_ALARM_STILL_ACTIVE_4_BIT_MASK         0x80
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_RESERVED51_BIT_MASK                   0x01
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_EXTERNAL_ALARM_5_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_SENSOR_ERROR_5_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_ALARM_TEMPERATURE_EXCEEDED_5_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_RESERVED52_MASK                       0x70
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_RESERVED52_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_LOG_REPORT_ALARM_EVENT_5_ALARM_STILL_ACTIVE_5_BIT_MASK         0x80
/* Values used for Chimney Fan Alarm Log Set command */
#define CHIMNEY_FAN_ALARM_LOG_SET_RESET_LOG                                              0x08
/* Values used for Chimney Fan Alarm Status Report command */
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_SERVICE_BIT_MASK                    0x01
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_EXTERNAL_ALARM_BIT_MASK             0x02
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_SENSOR_ERROR_BIT_MASK               0x04
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_ALARM_TEMPERATURE_EXCEEDED_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_NOT_USED_MASK                       0x30
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_NOT_USED_SHIFT                      0x04
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_SPEED_CHANGE_ENABLE_BIT_MASK        0x40
#define CHIMNEY_FAN_ALARM_STATUS_REPORT_ALARM_STATUS_START_TEMPERATURE_EXCEEDED_BIT_MASK 0x80
/* Values used for Chimney Fan Alarm Status Set command */
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_NOT_USED1_BIT_MASK                          0x01
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_ACKNOWLEDGE_EXTERNAL_ALARM_BIT_MASK         0x02
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_ACKNOWLEDGE_SENSOR_ERROR_BIT_MASK           0x04
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_ACKNOWLEDGE_ALARM_TEMPERATURE_EXCEEDED_BIT_MASK 0x08
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_NOT_USED2_MASK                              0xF0
#define CHIMNEY_FAN_ALARM_STATUS_SET_MESSAGE_NOT_USED2_SHIFT                             0x04
/* Values used for Chimney Fan Alarm Temp Report command */
#define CHIMNEY_FAN_ALARM_TEMP_REPORT_PROPERTIES1_SIZE_MASK                              0x07
#define CHIMNEY_FAN_ALARM_TEMP_REPORT_PROPERTIES1_SCALE_MASK                             0x18
#define CHIMNEY_FAN_ALARM_TEMP_REPORT_PROPERTIES1_SCALE_SHIFT                            0x03
#define CHIMNEY_FAN_ALARM_TEMP_REPORT_PROPERTIES1_PRECISION_MASK                         0xE0
#define CHIMNEY_FAN_ALARM_TEMP_REPORT_PROPERTIES1_PRECISION_SHIFT                        0x05
/* Values used for Chimney Fan Alarm Temp Set command */
#define CHIMNEY_FAN_ALARM_TEMP_SET_PROPERTIES1_SIZE_MASK                                 0x07
#define CHIMNEY_FAN_ALARM_TEMP_SET_PROPERTIES1_SCALE_MASK                                0x18
#define CHIMNEY_FAN_ALARM_TEMP_SET_PROPERTIES1_SCALE_SHIFT                               0x03
#define CHIMNEY_FAN_ALARM_TEMP_SET_PROPERTIES1_PRECISION_MASK                            0xE0
#define CHIMNEY_FAN_ALARM_TEMP_SET_PROPERTIES1_PRECISION_SHIFT                           0x05
/* Values used for Chimney Fan Mode Report command */
#define CHIMNEY_FAN_MODE_REPORT_OFF                                                      0x00
#define CHIMNEY_FAN_MODE_REPORT_ON                                                       0xFF
/* Values used for Chimney Fan Mode Set command */
#define CHIMNEY_FAN_MODE_SET_OFF                                                         0x00
#define CHIMNEY_FAN_MODE_SET_ON                                                          0xFF
/* Values used for Chimney Fan Setup Report command */
#define CHIMNEY_FAN_SETUP_REPORT_OFF                                                     0x00
#define CHIMNEY_FAN_SETUP_REPORT_ON                                                      0xFF
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES1_SIZE_1_MASK                                 0x07
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES1_SCALE_1_MASK                                0x18
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES1_SCALE_1_SHIFT                               0x03
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES1_PRECISION_1_MASK                            0xE0
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES1_PRECISION_1_SHIFT                           0x05
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES2_SIZE_2_MASK                                 0x07
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES2_SCALE_2_MASK                                0x18
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES2_SCALE_2_SHIFT                               0x03
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES2_PRECISION_2_MASK                            0xE0
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES2_PRECISION_2_SHIFT                           0x05
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES3_SIZE_3_MASK                                 0x07
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES3_SCALE_3_MASK                                0x18
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES3_SCALE_3_SHIFT                               0x03
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES3_PRECISION_3_MASK                            0xE0
#define CHIMNEY_FAN_SETUP_REPORT_PROPERTIES3_PRECISION_3_SHIFT                           0x05
/* Values used for Chimney Fan Setup Set command */
#define CHIMNEY_FAN_SETUP_SET_OFF                                                        0x00
#define CHIMNEY_FAN_SETUP_SET_ON                                                         0xFF
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES1_SIZE_1_MASK                                    0x07
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES1_SCALE_1_MASK                                   0x18
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES1_SCALE_1_SHIFT                                  0x03
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES1_PRECISION_1_MASK                               0xE0
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES1_PRECISION_1_SHIFT                              0x05
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES2_SIZE_2_MASK                                    0x07
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES2_SCALE_2_MASK                                   0x18
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES2_SCALE_2_SHIFT                                  0x03
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES2_PRECISION_2_MASK                               0xE0
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES2_PRECISION_2_SHIFT                              0x05
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES3_SIZE_3_MASK                                    0x07
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES3_SCALE_3_MASK                                   0x18
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES3_SCALE_3_SHIFT                                  0x03
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES3_PRECISION_3_MASK                               0xE0
#define CHIMNEY_FAN_SETUP_SET_PROPERTIES3_PRECISION_3_SHIFT                              0x05
/* Values used for Chimney Fan Start Temp Report command */
#define CHIMNEY_FAN_START_TEMP_REPORT_PROPERTIES1_SIZE_MASK                              0x07
#define CHIMNEY_FAN_START_TEMP_REPORT_PROPERTIES1_SCALE_MASK                             0x18
#define CHIMNEY_FAN_START_TEMP_REPORT_PROPERTIES1_SCALE_SHIFT                            0x03
#define CHIMNEY_FAN_START_TEMP_REPORT_PROPERTIES1_PRECISION_MASK                         0xE0
#define CHIMNEY_FAN_START_TEMP_REPORT_PROPERTIES1_PRECISION_SHIFT                        0x05
/* Values used for Chimney Fan Start Temp Set command */
#define CHIMNEY_FAN_START_TEMP_SET_PROPERTIES1_SIZE_MASK                                 0x07
#define CHIMNEY_FAN_START_TEMP_SET_PROPERTIES1_SCALE_MASK                                0x18
#define CHIMNEY_FAN_START_TEMP_SET_PROPERTIES1_SCALE_SHIFT                               0x03
#define CHIMNEY_FAN_START_TEMP_SET_PROPERTIES1_PRECISION_MASK                            0xE0
#define CHIMNEY_FAN_START_TEMP_SET_PROPERTIES1_PRECISION_SHIFT                           0x05
/* Values used for Chimney Fan State Report command */
#define CHIMNEY_FAN_STATE_REPORT_OFF                                                     0x00
#define CHIMNEY_FAN_STATE_REPORT_BOOST                                                   0x01
#define CHIMNEY_FAN_STATE_REPORT_EXHAUST                                                 0x02
#define CHIMNEY_FAN_STATE_REPORT_RELOAD                                                  0x03
#define CHIMNEY_FAN_STATE_REPORT_VENTING                                                 0x04
#define CHIMNEY_FAN_STATE_REPORT_STOP                                                    0x05
#define CHIMNEY_FAN_STATE_REPORT_VENTING_EX                                              0x06
#define CHIMNEY_FAN_STATE_REPORT_SERVICE                                                 0x07
#define CHIMNEY_FAN_STATE_REPORT_SENSOR_FAILURE                                          0x08
#define CHIMNEY_FAN_STATE_REPORT_CHIMNEY_FIRE                                            0x09
#define CHIMNEY_FAN_STATE_REPORT_EXTERNAL_ALARM                                          0x0A
/* Values used for Chimney Fan State Set command */
#define CHIMNEY_FAN_STATE_SET_NEXT_STATE                                                 0x01
/* Values used for Chimney Fan Status Report command */
#define CHIMNEY_FAN_STATUS_REPORT_OFF                                                    0x00
#define CHIMNEY_FAN_STATUS_REPORT_BOOST                                                  0x01
#define CHIMNEY_FAN_STATUS_REPORT_EXHAUST                                                0x02
#define CHIMNEY_FAN_STATUS_REPORT_RELOAD                                                 0x03
#define CHIMNEY_FAN_STATUS_REPORT_VENTING                                                0x04
#define CHIMNEY_FAN_STATUS_REPORT_STOP                                                   0x05
#define CHIMNEY_FAN_STATUS_REPORT_VENTING_EX                                             0x06
#define CHIMNEY_FAN_STATUS_REPORT_SERVICE                                                0x07
#define CHIMNEY_FAN_STATUS_REPORT_SENSOR_FAILURE                                         0x08
#define CHIMNEY_FAN_STATUS_REPORT_CHIMNEY_FIRE                                           0x09
#define CHIMNEY_FAN_STATUS_REPORT_EXTERNAL_ALARM                                         0x0A
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_SERVICE_BIT_MASK                          0x01
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_EXTERNAL_ALARM_BIT_MASK                   0x02
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_SENSOR_ERROR_BIT_MASK                     0x04
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_ALARM_TEMPERATURE_EXCEEDED_BIT_MASK       0x08
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_NOT_USED_MASK                             0x30
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_NOT_USED_SHIFT                            0x04
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_SPEED_CHANGE_ENABLE_BIT_MASK              0x40
#define CHIMNEY_FAN_STATUS_REPORT_ALARM_STATUS_START_TEMPERATURE_EXCEEDED_BIT_MASK       0x80
#define CHIMNEY_FAN_STATUS_REPORT_PROPERTIES1_SIZE_MASK                                  0x07
#define CHIMNEY_FAN_STATUS_REPORT_PROPERTIES1_SCALE_MASK                                 0x18
#define CHIMNEY_FAN_STATUS_REPORT_PROPERTIES1_SCALE_SHIFT                                0x03
#define CHIMNEY_FAN_STATUS_REPORT_PROPERTIES1_PRECISION_MASK                             0xE0
#define CHIMNEY_FAN_STATUS_REPORT_PROPERTIES1_PRECISION_SHIFT                            0x05
/* Values used for Chimney Fan Stop Temp Report command */
#define CHIMNEY_FAN_STOP_TEMP_REPORT_PROPERTIES1_SIZE_MASK                               0x07
#define CHIMNEY_FAN_STOP_TEMP_REPORT_PROPERTIES1_SCALE_MASK                              0x18
#define CHIMNEY_FAN_STOP_TEMP_REPORT_PROPERTIES1_SCALE_SHIFT                             0x03
#define CHIMNEY_FAN_STOP_TEMP_REPORT_PROPERTIES1_PRECISION_MASK                          0xE0
#define CHIMNEY_FAN_STOP_TEMP_REPORT_PROPERTIES1_PRECISION_SHIFT                         0x05
/* Values used for Chimney Fan Stop Temp Set command */
#define CHIMNEY_FAN_STOP_TEMP_SET_PROPERTIES1_SIZE_MASK                                  0x07
#define CHIMNEY_FAN_STOP_TEMP_SET_PROPERTIES1_SCALE_MASK                                 0x18
#define CHIMNEY_FAN_STOP_TEMP_SET_PROPERTIES1_SCALE_SHIFT                                0x03
#define CHIMNEY_FAN_STOP_TEMP_SET_PROPERTIES1_PRECISION_MASK                             0xE0
#define CHIMNEY_FAN_STOP_TEMP_SET_PROPERTIES1_PRECISION_SHIFT                            0x05

/* Climate Control Schedule command class commands */
#define CLIMATE_CONTROL_SCHEDULE_VERSION                                                 0x01
#define SCHEDULE_CHANGED_GET                                                             0x04
#define SCHEDULE_CHANGED_REPORT                                                          0x05
#define SCHEDULE_GET                                                                     0x02
#define SCHEDULE_OVERRIDE_GET                                                            0x07
#define SCHEDULE_OVERRIDE_REPORT                                                         0x08
#define SCHEDULE_OVERRIDE_SET                                                            0x06
#define SCHEDULE_REPORT                                                                  0x03
#define SCHEDULE_SET                                                                     0x01
/* Values used for Schedule Get command */
#define SCHEDULE_GET_PROPERTIES1_WEEKDAY_MASK                                            0x07
#define SCHEDULE_GET_PROPERTIES1_RESERVED_MASK                                           0xF8
#define SCHEDULE_GET_PROPERTIES1_RESERVED_SHIFT                                          0x03
/* Values used for Schedule Override Report command */
#define SCHEDULE_OVERRIDE_REPORT_PROPERTIES1_OVERRIDE_TYPE_MASK                          0x03
#define SCHEDULE_OVERRIDE_REPORT_PROPERTIES1_RESERVED_MASK                               0xFC
#define SCHEDULE_OVERRIDE_REPORT_PROPERTIES1_RESERVED_SHIFT                              0x02
#define SCHEDULE_OVERRIDE_REPORT_NO_OVERRIDE                                             0x00
#define SCHEDULE_OVERRIDE_REPORT_TEMPORARY_OVERRIDE                                      0x01
#define SCHEDULE_OVERRIDE_REPORT_PERMANENT_OVERRIDE                                      0x02
#define SCHEDULE_OVERRIDE_REPORT_RESERVED                                                0x03
/* Values used for Schedule Override Set command */
#define SCHEDULE_OVERRIDE_SET_PROPERTIES1_OVERRIDE_TYPE_MASK                             0x03
#define SCHEDULE_OVERRIDE_SET_PROPERTIES1_RESERVED_MASK                                  0xFC
#define SCHEDULE_OVERRIDE_SET_PROPERTIES1_RESERVED_SHIFT                                 0x02
#define SCHEDULE_OVERRIDE_SET_NO_OVERRIDE                                                0x00
#define SCHEDULE_OVERRIDE_SET_TEMPORARY_OVERRIDE                                         0x01
#define SCHEDULE_OVERRIDE_SET_PERMANENT_OVERRIDE                                         0x02
#define SCHEDULE_OVERRIDE_SET_RESERVED                                                   0x03
/* Values used for Schedule Report command */
#define SCHEDULE_REPORT_PROPERTIES1_WEEKDAY_MASK                                         0x07
#define SCHEDULE_REPORT_PROPERTIES1_RESERVED_MASK                                        0xF8
#define SCHEDULE_REPORT_PROPERTIES1_RESERVED_SHIFT                                       0x03
/* Values used for Schedule Set command */
#define SCHEDULE_SET_PROPERTIES1_WEEKDAY_MASK                                            0x07
#define SCHEDULE_SET_PROPERTIES1_RESERVED_MASK                                           0xF8
#define SCHEDULE_SET_PROPERTIES1_RESERVED_SHIFT                                          0x03

/* Clock command class commands */
#define CLOCK_VERSION                                                                    0x01
#define CLOCK_GET                                                                        0x05
#define CLOCK_REPORT                                                                     0x06
#define CLOCK_SET                                                                        0x04
/* Values used for Clock Report command */
#define CLOCK_REPORT_LEVEL_HOUR_MASK                                                     0x1F
#define CLOCK_REPORT_LEVEL_WEEKDAY_MASK                                                  0xE0
#define CLOCK_REPORT_LEVEL_WEEKDAY_SHIFT                                                 0x05
/* Values used for Clock Set command */
#define CLOCK_SET_LEVEL_HOUR_MASK                                                        0x1F
#define CLOCK_SET_LEVEL_WEEKDAY_MASK                                                     0xE0
#define CLOCK_SET_LEVEL_WEEKDAY_SHIFT                                                    0x05

/* Configuration command class commands */
#define CONFIGURATION_VERSION                                                            0x01
#define CONFIGURATION_GET                                                                0x05
#define CONFIGURATION_REPORT                                                             0x06
#define CONFIGURATION_SET                                                                0x04
/* Values used for Configuration Report command */
#define CONFIGURATION_REPORT_LEVEL_SIZE_MASK                                             0x07
#define CONFIGURATION_REPORT_LEVEL_RESERVED_MASK                                         0xF8
#define CONFIGURATION_REPORT_LEVEL_RESERVED_SHIFT                                        0x03
/* Values used for Configuration Set command */
#define CONFIGURATION_SET_LEVEL_SIZE_MASK                                                0x07
#define CONFIGURATION_SET_LEVEL_RESERVED_MASK                                            0x78
#define CONFIGURATION_SET_LEVEL_RESERVED_SHIFT                                           0x03
#define CONFIGURATION_SET_LEVEL_DEFAULT_BIT_MASK                                         0x80

/* Configuration command class commands */
#define CONFIGURATION_VERSION_V2                                                         0x02
#define CONFIGURATION_BULK_GET_V2                                                        0x08
#define CONFIGURATION_BULK_REPORT_V2                                                     0x09
#define CONFIGURATION_BULK_SET_V2                                                        0x07
#define CONFIGURATION_GET_V2                                                             0x05
#define CONFIGURATION_REPORT_V2                                                          0x06
#define CONFIGURATION_SET_V2                                                             0x04
/* Values used for Configuration Bulk Report command */
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_SIZE_MASK_V2                               0x07
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_MASK_V2                           0x38
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                          0x03
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_HANDSHAKE_BIT_MASK_V2                      0x40
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_DEFAULT_BIT_MASK_V2                        0x80
/* Values used for Configuration Bulk Set command */
#define CONFIGURATION_BULK_SET_PROPERTIES1_SIZE_MASK_V2                                  0x07
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_MASK_V2                              0x38
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_SHIFT_V2                             0x03
#define CONFIGURATION_BULK_SET_PROPERTIES1_HANDSHAKE_BIT_MASK_V2                         0x40
#define CONFIGURATION_BULK_SET_PROPERTIES1_DEFAULT_BIT_MASK_V2                           0x80
/* Values used for Configuration Report command */
#define CONFIGURATION_REPORT_LEVEL_SIZE_MASK_V2                                          0x07
#define CONFIGURATION_REPORT_LEVEL_RESERVED_MASK_V2                                      0xF8
#define CONFIGURATION_REPORT_LEVEL_RESERVED_SHIFT_V2                                     0x03
/* Values used for Configuration Set command */
#define CONFIGURATION_SET_LEVEL_SIZE_MASK_V2                                             0x07
#define CONFIGURATION_SET_LEVEL_RESERVED_MASK_V2                                         0x78
#define CONFIGURATION_SET_LEVEL_RESERVED_SHIFT_V2                                        0x03
#define CONFIGURATION_SET_LEVEL_DEFAULT_BIT_MASK_V2                                      0x80

/* Configuration command class commands */
#define CONFIGURATION_VERSION_V3                                                         0x03
#define CONFIGURATION_BULK_GET_V3                                                        0x08
#define CONFIGURATION_BULK_REPORT_V3                                                     0x09
#define CONFIGURATION_BULK_SET_V3                                                        0x07
#define CONFIGURATION_GET_V3                                                             0x05
#define CONFIGURATION_REPORT_V3                                                          0x06
#define CONFIGURATION_SET_V3                                                             0x04
#define CONFIGURATION_NAME_GET_V3                                                        0x0A
#define CONFIGURATION_NAME_REPORT_V3                                                     0x0B
#define CONFIGURATION_INFO_GET_V3                                                        0x0C
#define CONFIGURATION_INFO_REPORT_V3                                                     0x0D
#define CONFIGURATION_PROPERTIES_GET_V3                                                  0x0E
#define CONFIGURATION_PROPERTIES_REPORT_V3                                               0x0F
/* Values used for Configuration Bulk Report command */
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_SIZE_MASK_V3                               0x07
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_MASK_V3                           0x38
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                          0x03
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_HANDSHAKE_BIT_MASK_V3                      0x40
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_DEFAULT_BIT_MASK_V3                        0x80
/* Values used for Configuration Bulk Set command */
#define CONFIGURATION_BULK_SET_PROPERTIES1_SIZE_MASK_V3                                  0x07
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_MASK_V3                              0x38
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_SHIFT_V3                             0x03
#define CONFIGURATION_BULK_SET_PROPERTIES1_HANDSHAKE_BIT_MASK_V3                         0x40
#define CONFIGURATION_BULK_SET_PROPERTIES1_DEFAULT_BIT_MASK_V3                           0x80
/* Values used for Configuration Report command */
#define CONFIGURATION_REPORT_LEVEL_SIZE_MASK_V3                                          0x07
#define CONFIGURATION_REPORT_LEVEL_RESERVED_MASK_V3                                      0xF8
#define CONFIGURATION_REPORT_LEVEL_RESERVED_SHIFT_V3                                     0x03
/* Values used for Configuration Set command */
#define CONFIGURATION_SET_LEVEL_SIZE_MASK_V3                                             0x07
#define CONFIGURATION_SET_LEVEL_RESERVED_MASK_V3                                         0x78
#define CONFIGURATION_SET_LEVEL_RESERVED_SHIFT_V3                                        0x03
#define CONFIGURATION_SET_LEVEL_DEFAULT_BIT_MASK_V3                                      0x80
/* Values used for Configuration Properties Report command */
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_SIZE_MASK_V3                         0x07
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_FORMAT_MASK_V3                       0x38
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_FORMAT_SHIFT_V3                      0x03
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_SIGNED_INTEGER_V3                         0x00
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_UNSIGNED_INTEGER_V3                       0x01
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_ENUMERATED_V3                             0x02
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_BIT_FIELD_V3                              0x03
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_RESERVED_MASK_V3                     0xC0
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                    0x06

/* Configuration command class commands */
#define CONFIGURATION_VERSION_V4                                                         0x04
#define CONFIGURATION_BULK_GET_V4                                                        0x08
#define CONFIGURATION_BULK_REPORT_V4                                                     0x09
#define CONFIGURATION_BULK_SET_V4                                                        0x07
#define CONFIGURATION_GET_V4                                                             0x05
#define CONFIGURATION_REPORT_V4                                                          0x06
#define CONFIGURATION_SET_V4                                                             0x04
#define CONFIGURATION_NAME_GET_V4                                                        0x0A
#define CONFIGURATION_NAME_REPORT_V4                                                     0x0B
#define CONFIGURATION_INFO_GET_V4                                                        0x0C
#define CONFIGURATION_INFO_REPORT_V4                                                     0x0D
#define CONFIGURATION_PROPERTIES_GET_V4                                                  0x0E
#define CONFIGURATION_PROPERTIES_REPORT_V4                                               0x0F
#define CONFIGURATION_DEFAULT_RESET_V4                                                   0x01
/* Values used for Configuration Bulk Report command */
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_SIZE_MASK_V4                               0x07
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_MASK_V4                           0x38
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_RESERVED_SHIFT_V4                          0x03
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_HANDSHAKE_BIT_MASK_V4                      0x40
#define CONFIGURATION_BULK_REPORT_PROPERTIES1_DEFAULT_BIT_MASK_V4                        0x80
/* Values used for Configuration Bulk Set command */
#define CONFIGURATION_BULK_SET_PROPERTIES1_SIZE_MASK_V4                                  0x07
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_MASK_V4                              0x38
#define CONFIGURATION_BULK_SET_PROPERTIES1_RESERVED_SHIFT_V4                             0x03
#define CONFIGURATION_BULK_SET_PROPERTIES1_HANDSHAKE_BIT_MASK_V4                         0x40
#define CONFIGURATION_BULK_SET_PROPERTIES1_DEFAULT_BIT_MASK_V4                           0x80
/* Values used for Configuration Report command */
#define CONFIGURATION_REPORT_LEVEL_SIZE_MASK_V4                                          0x07
#define CONFIGURATION_REPORT_LEVEL_RESERVED_MASK_V4                                      0xF8
#define CONFIGURATION_REPORT_LEVEL_RESERVED_SHIFT_V4                                     0x03
/* Values used for Configuration Set command */
#define CONFIGURATION_SET_LEVEL_SIZE_MASK_V4                                             0x07
#define CONFIGURATION_SET_LEVEL_RESERVED_MASK_V4                                         0x78
#define CONFIGURATION_SET_LEVEL_RESERVED_SHIFT_V4                                        0x03
#define CONFIGURATION_SET_LEVEL_DEFAULT_BIT_MASK_V4                                      0x80
/* Values used for Configuration Properties Report command */
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_SIZE_MASK_V4                         0x07
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_FORMAT_MASK_V4                       0x38
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_FORMAT_SHIFT_V4                      0x03
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_SIGNED_INTEGER_V4                         0x00
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_UNSIGNED_INTEGER_V4                       0x01
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_ENUMERATED_V4                             0x02
#define CONFIGURATION_PROPERTIES_REPORT_FORMAT_BIT_FIELD_V4                              0x03
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_READONLY_BIT_MASK_V4                 0x40
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES1_RE_INCLUSION_REQUIRED_BIT_MASK_V4    0x80
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES2_ADVANCED_BIT_MASK_V4                 0x01
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES2_NO_BULK_SUPPORT_BIT_MASK_V4          0x02
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES2_RESERVED1_MASK_V4                    0xFC
#define CONFIGURATION_PROPERTIES_REPORT_PROPERTIES2_RESERVED1_SHIFT_V4                   0x02

/* Controller Replication command class commands */
#define CONTROLLER_REPLICATION_VERSION                                                   0x01
#define CTRL_REPLICATION_TRANSFER_GROUP                                                  0x31
#define CTRL_REPLICATION_TRANSFER_GROUP_NAME                                             0x32
#define CTRL_REPLICATION_TRANSFER_SCENE                                                  0x33
#define CTRL_REPLICATION_TRANSFER_SCENE_NAME                                             0x34

/* Crc 16 Encap command class commands */
#define CRC_16_ENCAP_VERSION                                                             0x01
#define CRC_16_ENCAP                                                                     0x01

/* Dcp Config command class commands */
#define DCP_CONFIG_VERSION                                                               0x01
#define DCP_LIST_REMOVE                                                                  0x04
#define DCP_LIST_SET                                                                     0x03
#define DCP_LIST_SUPPORTED_GET                                                           0x01
#define DCP_LIST_SUPPORTED_REPORT                                                        0x02
/* Values used for Dcp List Set command */
#define DCP_LIST_SET_PROPERTIES1_NUMBER_OF_DC_MASK                                       0x03
#define DCP_LIST_SET_PROPERTIES1_RESERVED_MASK                                           0xFC
#define DCP_LIST_SET_PROPERTIES1_RESERVED_SHIFT                                          0x02

/* Dcp Monitor command class commands */
#define DCP_MONITOR_VERSION                                                              0x01
#define DCP_EVENT_STATUS_GET                                                             0x03
#define DCP_EVENT_STATUS_REPORT                                                          0x04
#define DCP_LIST_GET                                                                     0x01
#define DCP_LIST_REPORT                                                                  0x02
/* Values used for Dcp List Report command */
#define DCP_LIST_REPORT_PROPERTIES1_NUMBER_OF_DC_MASK                                    0x03
#define DCP_LIST_REPORT_PROPERTIES1_RESERVED_MASK                                        0xFC
#define DCP_LIST_REPORT_PROPERTIES1_RESERVED_SHIFT                                       0x02

/* Door Lock Logging command class commands */
#define DOOR_LOCK_LOGGING_VERSION                                                        0x01
#define DOOR_LOCK_LOGGING_RECORDS_SUPPORTED_GET                                          0x01
#define DOOR_LOCK_LOGGING_RECORDS_SUPPORTED_REPORT                                       0x02
#define RECORD_GET                                                                       0x03
#define RECORD_REPORT                                                                    0x04
/* Values used for Record Report command */
#define RECORD_REPORT_PROPERTIES1_HOUR_LOCAL_TIME_MASK                                   0x1F
#define RECORD_REPORT_PROPERTIES1_RECORD_STATUS_MASK                                     0xE0
#define RECORD_REPORT_PROPERTIES1_RECORD_STATUS_SHIFT                                    0x05

/* Door Lock command class commands */
#define DOOR_LOCK_VERSION                                                                0x01
#define DOOR_LOCK_CONFIGURATION_GET                                                      0x05
#define DOOR_LOCK_CONFIGURATION_REPORT                                                   0x06
#define DOOR_LOCK_CONFIGURATION_SET                                                      0x04
#define DOOR_LOCK_OPERATION_GET                                                          0x02
#define DOOR_LOCK_OPERATION_REPORT                                                       0x03
#define DOOR_LOCK_OPERATION_SET                                                          0x01
/* Values used for Door Lock Configuration Report command */
#define DOOR_LOCK_CONFIGURATION_REPORT_CONSTANT_OPERATION                                0x01
#define DOOR_LOCK_CONFIGURATION_REPORT_TIMED_OPERATION                                   0x02
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK        0x0F
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK       0xF0
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT      0x04
/* Values used for Door Lock Configuration Set command */
#define DOOR_LOCK_CONFIGURATION_SET_CONSTANT_OPERATION                                   0x01
#define DOOR_LOCK_CONFIGURATION_SET_TIMED_OPERATION                                      0x02
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK           0x0F
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK          0xF0
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT         0x04
/* Values used for Door Lock Operation Report command */
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED                                        0x00
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_WITH_TIMEOUT                           0x01
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES                0x10
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT   0x11
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES               0x20
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT  0x21
#define DOOR_LOCK_OPERATION_REPORT_DOOR_SECURED                                          0xFF
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_MODE_MASK             0x0F
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_MASK            0xF0
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_SHIFT           0x04
/* Values used for Door Lock Operation Set command */
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED                                           0x00
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_WITH_TIMEOUT                              0x01
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES                   0x10
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT      0x11
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES                  0x20
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT     0x21
#define DOOR_LOCK_OPERATION_SET_DOOR_SECURED                                             0xFF

/* Door Lock command class commands */
#define DOOR_LOCK_VERSION_V2                                                             0x02
#define DOOR_LOCK_CONFIGURATION_GET_V2                                                   0x05
#define DOOR_LOCK_CONFIGURATION_REPORT_V2                                                0x06
#define DOOR_LOCK_CONFIGURATION_SET_V2                                                   0x04
#define DOOR_LOCK_OPERATION_GET_V2                                                       0x02
#define DOOR_LOCK_OPERATION_REPORT_V2                                                    0x03
#define DOOR_LOCK_OPERATION_SET_V2                                                       0x01
/* Values used for Door Lock Configuration Report command */
#define DOOR_LOCK_CONFIGURATION_REPORT_CONSTANT_OPERATION_V2                             0x01
#define DOOR_LOCK_CONFIGURATION_REPORT_TIMED_OPERATION_V2                                0x02
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK_V2     0x0F
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK_V2    0xF0
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT_V2   0x04
/* Values used for Door Lock Configuration Set command */
#define DOOR_LOCK_CONFIGURATION_SET_CONSTANT_OPERATION_V2                                0x01
#define DOOR_LOCK_CONFIGURATION_SET_TIMED_OPERATION_V2                                   0x02
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK_V2        0x0F
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK_V2       0xF0
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT_V2      0x04
/* Values used for Door Lock Operation Report command */
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_V2                                     0x00
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_WITH_TIMEOUT_V2                        0x01
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_V2             0x10
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT_V2 0x11
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_V2            0x20
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT_V2 0x21
#define DOOR_LOCK_OPERATION_REPORT_DOOR_LOCK_STATE_UNKNOWN_V2                            0xFE
#define DOOR_LOCK_OPERATION_REPORT_DOOR_SECURED_V2                                       0xFF
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_MODE_MASK_V2          0x0F
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_MASK_V2         0xF0
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_SHIFT_V2        0x04
/* Values used for Door Lock Operation Set command */
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_V2                                        0x00
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_WITH_TIMEOUT_V2                           0x01
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_V2                0x10
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT_V2   0x11
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_V2               0x20
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT_V2  0x21
#define DOOR_LOCK_OPERATION_SET_DOOR_LOCK_STATE_UNKNOWN_V2                               0xFE
#define DOOR_LOCK_OPERATION_SET_DOOR_SECURED_V2                                          0xFF

/* Door Lock command class commands */
#define DOOR_LOCK_VERSION_V3                                                             0x03
#define DOOR_LOCK_CONFIGURATION_GET_V3                                                   0x05
#define DOOR_LOCK_CONFIGURATION_REPORT_V3                                                0x06
#define DOOR_LOCK_CONFIGURATION_SET_V3                                                   0x04
#define DOOR_LOCK_OPERATION_GET_V3                                                       0x02
#define DOOR_LOCK_OPERATION_REPORT_V3                                                    0x03
#define DOOR_LOCK_OPERATION_SET_V3                                                       0x01
/* Values used for Door Lock Configuration Report command */
#define DOOR_LOCK_CONFIGURATION_REPORT_CONSTANT_OPERATION_V3                             0x01
#define DOOR_LOCK_CONFIGURATION_REPORT_TIMED_OPERATION_V3                                0x02
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK_V3     0x0F
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK_V3    0xF0
#define DOOR_LOCK_CONFIGURATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT_V3   0x04
/* Values used for Door Lock Configuration Set command */
#define DOOR_LOCK_CONFIGURATION_SET_CONSTANT_OPERATION_V3                                0x01
#define DOOR_LOCK_CONFIGURATION_SET_TIMED_OPERATION_V3                                   0x02
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_INSIDE_DOOR_HANDLES_STATE_MASK_V3        0x0F
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_MASK_V3       0xF0
#define DOOR_LOCK_CONFIGURATION_SET_PROPERTIES1_OUTSIDE_DOOR_HANDLES_STATE_SHIFT_V3      0x04
/* Values used for Door Lock Operation Report command */
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_V3                                     0x00
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_WITH_TIMEOUT_V3                        0x01
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_V3             0x10
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3 0x11
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_V3            0x20
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3 0x21
#define DOOR_LOCK_OPERATION_REPORT_DOOR_LOCK_STATE_UNKNOWN_V3                            0xFE
#define DOOR_LOCK_OPERATION_REPORT_DOOR_SECURED_V3                                       0xFF
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_INSIDE_DOOR_HANDLES_MODE_MASK_V3          0x0F
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_MASK_V3         0xF0
#define DOOR_LOCK_OPERATION_REPORT_PROPERTIES1_OUTSIDE_DOOR_HANDLES_MODE_SHIFT_V3        0x04
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_V3                                     0x00
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_WITH_TIMEOUT_V3                        0x01
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_V3             0x10
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3 0x11
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_V3            0x20
#define DOOR_LOCK_OPERATION_REPORT_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3 0x21
#define DOOR_LOCK_OPERATION_REPORT_DOOR_LOCK_STATE_UNKNOWN_V3                            0xFE
#define DOOR_LOCK_OPERATION_REPORT_DOOR_SECURED_V3                                       0xFF
#define DOOR_LOCK_OPERATION_REPORT_ALREADY_AT_THE_TARGET_VALUE_V3                        0x00
#define DOOR_LOCK_OPERATION_REPORT_UNKNOWN_DURATION_V3                                   0xFE
#define DOOR_LOCK_OPERATION_REPORT_RESERVED_V3                                           0xFF
/* Values used for Door Lock Operation Set command */
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_V3                                        0x00
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_WITH_TIMEOUT_V3                           0x01
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_V3                0x10
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_INSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3   0x11
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_V3               0x20
#define DOOR_LOCK_OPERATION_SET_DOOR_UNSECURED_FOR_OUTSIDE_DOOR_HANDLES_WITH_TIMEOUT_V3  0x21
#define DOOR_LOCK_OPERATION_SET_DOOR_LOCK_STATE_UNKNOWN_V3                               0xFE
#define DOOR_LOCK_OPERATION_SET_DOOR_SECURED_V3                                          0xFF

/* Energy Production command class commands */
#define ENERGY_PRODUCTION_VERSION                                                        0x01
#define ENERGY_PRODUCTION_GET                                                            0x02
#define ENERGY_PRODUCTION_REPORT                                                         0x03
/* Values used for Energy Production Get command */
#define ENERGY_PRODUCTION_GET_INSTANT_ENERGY_PRODUCTION                                  0x00
#define ENERGY_PRODUCTION_GET_TOTAL_ENERGY_PRODUCTION                                    0x01
#define ENERGY_PRODUCTION_GET_ENERGY_PRODUCTION_TODAY                                    0x02
#define ENERGY_PRODUCTION_GET_TOTAL_PRODUCTION_TIME                                      0x03
/* Values used for Energy Production Report command */
#define ENERGY_PRODUCTION_REPORT_INSTANT_ENERGY_PRODUCTION                               0x00
#define ENERGY_PRODUCTION_REPORT_TOTAL_ENERGY_PRODUCTION                                 0x01
#define ENERGY_PRODUCTION_REPORT_ENERGY_PRODUCTION_TODAY                                 0x02
#define ENERGY_PRODUCTION_REPORT_TOTAL_PRODUCTION_TIME                                   0x03
#define ENERGY_PRODUCTION_REPORT_LEVEL_SIZE_MASK                                         0x07
#define ENERGY_PRODUCTION_REPORT_LEVEL_SCALE_MASK                                        0x18
#define ENERGY_PRODUCTION_REPORT_LEVEL_SCALE_SHIFT                                       0x03
#define ENERGY_PRODUCTION_REPORT_LEVEL_PRECISION_MASK                                    0xE0
#define ENERGY_PRODUCTION_REPORT_LEVEL_PRECISION_SHIFT                                   0x05

/* Firmware Update Md command class commands */
#define FIRMWARE_UPDATE_MD_VERSION                                                       0x01
#define FIRMWARE_MD_GET                                                                  0x01
#define FIRMWARE_MD_REPORT                                                               0x02
#define FIRMWARE_UPDATE_MD_GET                                                           0x05
#define FIRMWARE_UPDATE_MD_REPORT                                                        0x06
#define FIRMWARE_UPDATE_MD_REQUEST_GET                                                   0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT                                                0x04
#define FIRMWARE_UPDATE_MD_STATUS_REPORT                                                 0x07
/* Values used for Firmware Update Md Get command */
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_REPORT_NUMBER_1_MASK                          0x7F
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_ZERO_BIT_MASK                                 0x80
/* Values used for Firmware Update Md Report command */
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_REPORT_NUMBER_1_MASK                       0x7F
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_LAST_BIT_MASK                              0x80
/* Values used for Firmware Update Md Request Report command */
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_COMBINATION                            0x00
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_REQUIRES_AUTHENTICATION                        0x01
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_VALID_COMBINATION                              0xFF
/* Values used for Firmware Update Md Status Report command */
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_WITHOUT_CHECKSUM_ERROR        0x00
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE                               0x01
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY                                    0xFF

/* Firmware Update Md command class commands */
#define FIRMWARE_UPDATE_MD_VERSION_V2                                                    0x02
#define FIRMWARE_MD_GET_V2                                                               0x01
#define FIRMWARE_MD_REPORT_V2                                                            0x02
#define FIRMWARE_UPDATE_MD_GET_V2                                                        0x05
#define FIRMWARE_UPDATE_MD_REPORT_V2                                                     0x06
#define FIRMWARE_UPDATE_MD_REQUEST_GET_V2                                                0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_V2                                             0x04
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_V2                                              0x07
/* Values used for Firmware Update Md Get command */
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_REPORT_NUMBER_1_MASK_V2                       0x7F
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_ZERO_BIT_MASK_V2                              0x80
/* Values used for Firmware Update Md Report command */
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_REPORT_NUMBER_1_MASK_V2                    0x7F
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_LAST_BIT_MASK_V2                           0x80
/* Values used for Firmware Update Md Request Report command */
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_COMBINATION_V2                         0x00
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_REQUIRES_AUTHENTICATION_V2                     0x01
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_VALID_COMBINATION_V2                           0xFF
/* Values used for Firmware Update Md Status Report command */
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_WITHOUT_CHECKSUM_ERROR_V2     0x00
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_V2                            0x01
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_V2                                 0xFF

/* Firmware Update Md command class commands */
#define FIRMWARE_UPDATE_MD_VERSION_V3                                                    0x03
#define FIRMWARE_MD_GET_V3                                                               0x01
#define FIRMWARE_MD_REPORT_V3                                                            0x02
#define FIRMWARE_UPDATE_MD_GET_V3                                                        0x05
#define FIRMWARE_UPDATE_MD_REPORT_V3                                                     0x06
#define FIRMWARE_UPDATE_MD_REQUEST_GET_V3                                                0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_V3                                             0x04
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_V3                                              0x07
/* Values used for Firmware Update Md Get command */
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_REPORT_NUMBER_1_MASK_V3                       0x7F
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_ZERO_BIT_MASK_V3                              0x80
/* Values used for Firmware Update Md Report command */
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_REPORT_NUMBER_1_MASK_V3                    0x7F
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_LAST_BIT_MASK_V3                           0x80
/* Values used for Firmware Update Md Request Report command */
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_COMBINATION_V3                         0x00
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_REQUIRES_AUTHENTICATION_V3                     0x01
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_FRAGMENT_SIZE_V3                       0x02
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_NOT_UPGRADABLE_V3                              0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_VALID_COMBINATION_V3                           0xFF
/* Values used for Firmware Update Md Status Report command */
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_WITHOUT_CHECKSUM_ERROR_V3     0x00
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_V3                            0x01
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_STORED_V3                          0xFE
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_V3                                 0xFF

/* Firmware Update Md command class commands */
#define FIRMWARE_UPDATE_MD_VERSION_V4                                                    0x04
#define FIRMWARE_MD_GET_V4                                                               0x01
#define FIRMWARE_MD_REPORT_V4                                                            0x02
#define FIRMWARE_UPDATE_MD_GET_V4                                                        0x05
#define FIRMWARE_UPDATE_MD_REPORT_V4                                                     0x06
#define FIRMWARE_UPDATE_MD_REQUEST_GET_V4                                                0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_V4                                             0x04
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_V4                                              0x07
#define FIRMWARE_UPDATE_ACTIVATION_SET_V4                                                0x08
#define FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_V4                                      0x09
/* Values used for Firmware Update Md Get command */
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_REPORT_NUMBER_1_MASK_V4                       0x7F
#define FIRMWARE_UPDATE_MD_GET_PROPERTIES1_ZERO_BIT_MASK_V4                              0x80
/* Values used for Firmware Update Md Report command */
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_REPORT_NUMBER_1_MASK_V4                    0x7F
#define FIRMWARE_UPDATE_MD_REPORT_PROPERTIES1_LAST_BIT_MASK_V4                           0x80
/* Values used for Firmware Update Md Request Get command */
#define FIRMWARE_UPDATE_MD_REQUEST_GET_PROPERTIES1_ACTIVATION_BIT_MASK_V4                0x01
#define FIRMWARE_UPDATE_MD_REQUEST_GET_PROPERTIES1_RESERVED_MASK_V4                      0xFE
#define FIRMWARE_UPDATE_MD_REQUEST_GET_PROPERTIES1_RESERVED_SHIFT_V4                     0x01
/* Values used for Firmware Update Md Request Report command */
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_COMBINATION_V4                         0x00
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_REQUIRES_AUTHENTICATION_V4                     0x01
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_INVALID_FRAGMENT_SIZE_V4                       0x02
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_NOT_UPGRADABLE_V4                              0x03
#define FIRMWARE_UPDATE_MD_REQUEST_REPORT_VALID_COMBINATION_V4                           0xFF
/* Values used for Firmware Update Md Status Report command */
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_WITHOUT_CHECKSUM_ERROR_V4     0x00
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_UNABLE_TO_RECEIVE_V4                            0x01
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_DOES_NOT_MATCH_THE_MANUFACTURER_ID_V4           0x02
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_DOES_NOT_MATCH_THE_FIRMWARE_ID_V4               0x03
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_DOES_NOT_MATCH_THE_FIRMWARE_TARGET_V4           0x04
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_INVALID_FILE_HEADER_INFORMATION_V4              0x05
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_INVALID_FILE_HEADER_FORMAT_V4                   0x06
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_INSUFFICIENT_MEMORY_V4                          0x07
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_WAITING_FOR_ACTIVATION_V4          0xFD
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_STORED_V4                          0xFE
#define FIRMWARE_UPDATE_MD_STATUS_REPORT_SUCCESSFULLY_V4                                 0xFF
/* Values used for Firmware Update Activation Status Report command */
#define FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_INVALID_COMBINATION_V4                  0x00
#define FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_ERROR_ACTIVATING_THE_FIRMWARE_V4        0x01
#define FIRMWARE_UPDATE_ACTIVATION_STATUS_REPORT_FIRMWARE_UPDATE_COMPLETED_SUCCESSFULLY_V4 0xFF

/* Geographic Location command class commands */
#define GEOGRAPHIC_LOCATION_VERSION                                                      0x01
#define GEOGRAPHIC_LOCATION_GET                                                          0x02
#define GEOGRAPHIC_LOCATION_REPORT                                                       0x03
#define GEOGRAPHIC_LOCATION_SET                                                          0x01
/* Values used for Geographic Location Report command */
#define GEOGRAPHIC_LOCATION_REPORT_LEVEL_LONGITUDE_MINUTES_MASK                          0x7F
#define GEOGRAPHIC_LOCATION_REPORT_LEVEL_LONG_SIGN_BIT_MASK                              0x80
#define GEOGRAPHIC_LOCATION_REPORT_LEVEL2_LATITUDE_MINUTES_MASK                          0x7F
#define GEOGRAPHIC_LOCATION_REPORT_LEVEL2_LAT_SIGN_BIT_MASK                              0x80
/* Values used for Geographic Location Set command */
#define GEOGRAPHIC_LOCATION_SET_LEVEL_LONGITUDE_MINUTES_MASK                             0x7F
#define GEOGRAPHIC_LOCATION_SET_LEVEL_LONG_SIGN_BIT_MASK                                 0x80
#define GEOGRAPHIC_LOCATION_SET_LEVEL2_LATITUDE_MINUTES_MASK                             0x7F
#define GEOGRAPHIC_LOCATION_SET_LEVEL2_LAT_SIGN_BIT_MASK                                 0x80

/* Grouping Name command class commands */
#define GROUPING_NAME_VERSION                                                            0x01
#define GROUPING_NAME_GET                                                                0x02
#define GROUPING_NAME_REPORT                                                             0x03
#define GROUPING_NAME_SET                                                                0x01
/* Values used for Grouping Name Report command */
#define GROUPING_NAME_REPORT_PROPERTIES1_CHAR_PRESENTATION_MASK                          0x07
#define GROUPING_NAME_REPORT_PROPERTIES1_RESERVED_MASK                                   0xF8
#define GROUPING_NAME_REPORT_PROPERTIES1_RESERVED_SHIFT                                  0x03
/* Values used for Grouping Name Set command */
#define GROUPING_NAME_SET_PROPERTIES1_CHAR_PRESENTATION_MASK                             0x07
#define GROUPING_NAME_SET_PROPERTIES1_RESERVED_MASK                                      0xF8
#define GROUPING_NAME_SET_PROPERTIES1_RESERVED_SHIFT                                     0x03

/* Hail command class commands */
#define HAIL_VERSION                                                                     0x01
#define HAIL                                                                             0x01

/* Hrv Control command class commands */
#define HRV_CONTROL_VERSION                                                              0x01
#define HRV_CONTROL_BYPASS_GET                                                           0x05
#define HRV_CONTROL_BYPASS_REPORT                                                        0x06
#define HRV_CONTROL_BYPASS_SET                                                           0x04
#define HRV_CONTROL_MODE_GET                                                             0x02
#define HRV_CONTROL_MODE_REPORT                                                          0x03
#define HRV_CONTROL_MODE_SET                                                             0x01
#define HRV_CONTROL_MODE_SUPPORTED_GET                                                   0x0A
#define HRV_CONTROL_MODE_SUPPORTED_REPORT                                                0x0B
#define HRV_CONTROL_VENTILATION_RATE_GET                                                 0x08
#define HRV_CONTROL_VENTILATION_RATE_REPORT                                              0x09
#define HRV_CONTROL_VENTILATION_RATE_SET                                                 0x07
/* Values used for Hrv Control Mode Report command */
#define HRV_CONTROL_MODE_REPORT_PROPERTIES1_MODE_MASK                                    0x1F
#define HRV_CONTROL_MODE_REPORT_MODE_OFF                                                 0x00
#define HRV_CONTROL_MODE_REPORT_MODE_DEMAND_AUTOMATIC                                    0x01
#define HRV_CONTROL_MODE_REPORT_MODE_SCHEDULE                                            0x02
#define HRV_CONTROL_MODE_REPORT_MODE_ENERGY_SAVINGS_MODE                                 0x03
#define HRV_CONTROL_MODE_REPORT_MODE_MANUAL                                              0x04
#define HRV_CONTROL_MODE_REPORT_PROPERTIES1_RESERVED_MASK                                0xE0
#define HRV_CONTROL_MODE_REPORT_PROPERTIES1_RESERVED_SHIFT                               0x05
/* Values used for Hrv Control Mode Set command */
#define HRV_CONTROL_MODE_SET_PROPERTIES1_MODE_MASK                                       0x1F
#define HRV_CONTROL_MODE_SET_MODE_OFF                                                    0x00
#define HRV_CONTROL_MODE_SET_MODE_DEMAND_AUTOMATIC                                       0x01
#define HRV_CONTROL_MODE_SET_MODE_SCHEDULE                                               0x02
#define HRV_CONTROL_MODE_SET_MODE_ENERGY_SAVINGS_MODE                                    0x03
#define HRV_CONTROL_MODE_SET_MODE_MANUAL                                                 0x04
#define HRV_CONTROL_MODE_SET_PROPERTIES1_RESERVED_MASK                                   0xE0
#define HRV_CONTROL_MODE_SET_PROPERTIES1_RESERVED_SHIFT                                  0x05
/* Values used for Hrv Control Mode Supported Report command */
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_PROPERTIES1_MANUAL_CONTROL_SUPPORTED_MASK      0x0F
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_MANUAL_CONTROL_SUPPORTED_BYPASS_OPEN_CLOSE     0x00
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_MANUAL_CONTROL_SUPPORTED_BYPASS_AUTO           0x01
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_MANUAL_CONTROL_SUPPORTED_MODULATED_BYPASS      0x02
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_MANUAL_CONTROL_SUPPORTED_VENTILATION_RATE      0x03
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK                      0xF0
#define HRV_CONTROL_MODE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT                     0x04

/* Hrv Status command class commands */
#define HRV_STATUS_VERSION                                                               0x01
#define HRV_STATUS_GET                                                                   0x01
#define HRV_STATUS_REPORT                                                                0x02
#define HRV_STATUS_SUPPORTED_GET                                                         0x03
#define HRV_STATUS_SUPPORTED_REPORT                                                      0x04
/* Values used for Hrv Status Get command */
#define HRV_STATUS_GET_OUTDOOR_AIR_TEMPERATURE                                           0x00
#define HRV_STATUS_GET_SUPPLY_AIR_TEMPERATURE                                            0x01
#define HRV_STATUS_GET_EXHAUST_AIR_TEMPERATURE                                           0x02
#define HRV_STATUS_GET_DISCHARGE_AIR_TEMPERATURE                                         0x03
#define HRV_STATUS_GET_ROOM_TEMPERATURE                                                  0x04
#define HRV_STATUS_GET_RELATIVE_HUMIDITY_IN_ROOM                                         0x05
#define HRV_STATUS_GET_REMAINING_FILTER_LIFE                                             0x06
/* Values used for Hrv Status Report command */
#define HRV_STATUS_REPORT_OUTDOOR_AIR_TEMPERATURE                                        0x00
#define HRV_STATUS_REPORT_SUPPLY_AIR_TEMPERATURE                                         0x01
#define HRV_STATUS_REPORT_EXHAUST_AIR_TEMPERATURE                                        0x02
#define HRV_STATUS_REPORT_DISCHARGE_AIR_TEMPERATURE                                      0x03
#define HRV_STATUS_REPORT_ROOM_TEMPERATURE                                               0x04
#define HRV_STATUS_REPORT_RELATIVE_HUMIDITY_IN_ROOM                                      0x05
#define HRV_STATUS_REPORT_REMAINING_FILTER_LIFE                                          0x06
#define HRV_STATUS_REPORT_PROPERTIES1_SIZE_MASK                                          0x07
#define HRV_STATUS_REPORT_PROPERTIES1_SCALE_MASK                                         0x18
#define HRV_STATUS_REPORT_PROPERTIES1_SCALE_SHIFT                                        0x03
#define HRV_STATUS_REPORT_PROPERTIES1_PRECISION_MASK                                     0xE0
#define HRV_STATUS_REPORT_PROPERTIES1_PRECISION_SHIFT                                    0x05

/* Indicator command class commands */
#define INDICATOR_VERSION                                                                0x01
#define INDICATOR_GET                                                                    0x02
#define INDICATOR_REPORT                                                                 0x03
#define INDICATOR_SET                                                                    0x01

/* Indicator command class commands */
#define INDICATOR_VERSION_V2                                                             0x02
#define INDICATOR_GET_V2                                                                 0x02
#define INDICATOR_REPORT_V2                                                              0x03
#define INDICATOR_SET_V2                                                                 0x01
#define INDICATOR_SUPPORTED_GET_V2                                                       0x04
#define INDICATOR_SUPPORTED_REPORT_V2                                                    0x05
/* Values used for Indicator Get command */
#define INDICATOR_GET_NA_V2                                                              0x00
#define INDICATOR_GET_ARMED_V2                                                           0x01
#define INDICATOR_GET_NOT_ARMED_V2                                                       0x02
#define INDICATOR_GET_READY_V2                                                           0x03
#define INDICATOR_GET_FAULT_V2                                                           0x04
#define INDICATOR_GET_BUSY_V2                                                            0x05
#define INDICATOR_GET_ENTER_ID_V2                                                        0x06
#define INDICATOR_GET_ENTER_PIN_V2                                                       0x07
#define INDICATOR_GET_OK_V2                                                              0x08
#define INDICATOR_GET_NOT_OK_V2                                                          0x09
#define INDICATOR_GET_ZONE1_ARMED_V2                                                     0x20
#define INDICATOR_GET_ZONE2_ARMED_V2                                                     0x21
#define INDICATOR_GET_ZONE3_ARMED_V2                                                     0x22
#define INDICATOR_GET_ZONE4_ARMED_V2                                                     0x23
#define INDICATOR_GET_ZONE5_ARMED_V2                                                     0x24
#define INDICATOR_GET_ZONE6_ARMED_V2                                                     0x25
#define INDICATOR_GET_LCD_BACKLIGHT_V2                                                   0x30
#define INDICATOR_GET_BUTTON_BACKLIGHT_LETTERS_V2                                        0x40
#define INDICATOR_GET_BUTTON_BACKLIGHT_DIGITS_V2                                         0x41
#define INDICATOR_GET_BUTTON_BACKLIGHT_COMMAND_V2                                        0x42
#define INDICATOR_GET_BUTTON1_INDICATION_V2                                              0x43
#define INDICATOR_GET_BUTTON2_INDICATION_V2                                              0x44
#define INDICATOR_GET_BUTTON3_INDICATION_V2                                              0x45
#define INDICATOR_GET_BUTTON4_INDICATION_V2                                              0x46
#define INDICATOR_GET_BUTTON5_INDICATION_V2                                              0x47
#define INDICATOR_GET_BUTTON6_INDICATION_V2                                              0x48
#define INDICATOR_GET_BUTTON7_INDICATION_V2                                              0x49
#define INDICATOR_GET_BUTTON8_INDICATION_V2                                              0x4A
#define INDICATOR_GET_BUTTON9_INDICATION_V2                                              0x4B
#define INDICATOR_GET_BUTTON10_INDICATION_V2                                             0x4C
#define INDICATOR_GET_BUTTON11_INDICATION_V2                                             0x4D
#define INDICATOR_GET_BUTTON12_INDICATION_V2                                             0x4E
#define INDICATOR_GET_BUZZER_V2                                                          0xF0
/* Values used for Indicator Report command */
#define INDICATOR_REPORT_PROPERTIES1_INDICATOR_OBJECT_COUNT_MASK_V2                      0x1F
#define INDICATOR_REPORT_PROPERTIES1_RESERVED_MASK_V2                                    0xE0
#define INDICATOR_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                                   0x05
#define INDICATOR_REPORT_NA_V2                                                           0x00
#define INDICATOR_REPORT_ARMED_V2                                                        0x01
#define INDICATOR_REPORT_NOT_ARMED_V2                                                    0x02
#define INDICATOR_REPORT_READY_V2                                                        0x03
#define INDICATOR_REPORT_FAULT_V2                                                        0x04
#define INDICATOR_REPORT_BUSY_V2                                                         0x05
#define INDICATOR_REPORT_ENTER_ID_V2                                                     0x06
#define INDICATOR_REPORT_ENTER_PIN_V2                                                    0x07
#define INDICATOR_REPORT_OK_V2                                                           0x08
#define INDICATOR_REPORT_NOT_OK_V2                                                       0x09
#define INDICATOR_REPORT_ZONE1_ARMED_V2                                                  0x20
#define INDICATOR_REPORT_ZONE2_ARMED_V2                                                  0x21
#define INDICATOR_REPORT_ZONE3_ARMED_V2                                                  0x22
#define INDICATOR_REPORT_ZONE4_ARMED_V2                                                  0x23
#define INDICATOR_REPORT_ZONE5_ARMED_V2                                                  0x24
#define INDICATOR_REPORT_ZONE6_ARMED_V2                                                  0x25
#define INDICATOR_REPORT_LCD_BACKLIGHT_V2                                                0x30
#define INDICATOR_REPORT_BUTTON_BACKLIGHT_LETTERS_V2                                     0x40
#define INDICATOR_REPORT_BUTTON_BACKLIGHT_DIGITS_V2                                      0x41
#define INDICATOR_REPORT_BUTTON_BACKLIGHT_COMMAND_V2                                     0x42
#define INDICATOR_REPORT_BUTTON1_INDICATION_V2                                           0x43
#define INDICATOR_REPORT_BUTTON2_INDICATION_V2                                           0x44
#define INDICATOR_REPORT_BUTTON3_INDICATION_V2                                           0x45
#define INDICATOR_REPORT_BUTTON4_INDICATION_V2                                           0x46
#define INDICATOR_REPORT_BUTTON5_INDICATION_V2                                           0x47
#define INDICATOR_REPORT_BUTTON6_INDICATION_V2                                           0x48
#define INDICATOR_REPORT_BUTTON7_INDICATION_V2                                           0x49
#define INDICATOR_REPORT_BUTTON8_INDICATION_V2                                           0x4A
#define INDICATOR_REPORT_BUTTON9_INDICATION_V2                                           0x4B
#define INDICATOR_REPORT_BUTTON10_INDICATION_V2                                          0x4C
#define INDICATOR_REPORT_BUTTON11_INDICATION_V2                                          0x4D
#define INDICATOR_REPORT_BUTTON12_INDICATION_V2                                          0x4E
#define INDICATOR_REPORT_BUZZER_V2                                                       0xF0
#define INDICATOR_REPORT_MULTILEVEL_V2                                                   0x01
#define INDICATOR_REPORT_BINARY_V2                                                       0x02
#define INDICATOR_REPORT_ON_OFF_PERIOD_V2                                                0x03
#define INDICATOR_REPORT_ON_OFF_CYCLES_V2                                                0x04
#define INDICATOR_REPORT_LOW_POWER_V2                                                    0x10
/* Values used for Indicator Set command */
#define INDICATOR_SET_PROPERTIES1_INDICATOR_OBJECT_COUNT_MASK_V2                         0x1F
#define INDICATOR_SET_PROPERTIES1_RESERVED_MASK_V2                                       0xE0
#define INDICATOR_SET_PROPERTIES1_RESERVED_SHIFT_V2                                      0x05
#define INDICATOR_SET_NA_V2                                                              0x00
#define INDICATOR_SET_ARMED_V2                                                           0x01
#define INDICATOR_SET_NOT_ARMED_V2                                                       0x02
#define INDICATOR_SET_READY_V2                                                           0x03
#define INDICATOR_SET_FAULT_V2                                                           0x04
#define INDICATOR_SET_BUSY_V2                                                            0x05
#define INDICATOR_SET_ENTER_ID_V2                                                        0x06
#define INDICATOR_SET_ENTER_PIN_V2                                                       0x07
#define INDICATOR_SET_OK_V2                                                              0x08
#define INDICATOR_SET_NOT_OK_V2                                                          0x09
#define INDICATOR_SET_ZONE1_ARMED_V2                                                     0x20
#define INDICATOR_SET_ZONE2_ARMED_V2                                                     0x21
#define INDICATOR_SET_ZONE3_ARMED_V2                                                     0x22
#define INDICATOR_SET_ZONE4_ARMED_V2                                                     0x23
#define INDICATOR_SET_ZONE5_ARMED_V2                                                     0x24
#define INDICATOR_SET_ZONE6_ARMED_V2                                                     0x25
#define INDICATOR_SET_LCD_BACKLIGHT_V2                                                   0x30
#define INDICATOR_SET_BUTTON_BACKLIGHT_LETTERS_V2                                        0x40
#define INDICATOR_SET_BUTTON_BACKLIGHT_DIGITS_V2                                         0x41
#define INDICATOR_SET_BUTTON_BACKLIGHT_COMMAND_V2                                        0x42
#define INDICATOR_SET_BUTTON1_INDICATION_V2                                              0x43
#define INDICATOR_SET_BUTTON2_INDICATION_V2                                              0x44
#define INDICATOR_SET_BUTTON3_INDICATION_V2                                              0x45
#define INDICATOR_SET_BUTTON4_INDICATION_V2                                              0x46
#define INDICATOR_SET_BUTTON5_INDICATION_V2                                              0x47
#define INDICATOR_SET_BUTTON6_INDICATION_V2                                              0x48
#define INDICATOR_SET_BUTTON7_INDICATION_V2                                              0x49
#define INDICATOR_SET_BUTTON8_INDICATION_V2                                              0x4A
#define INDICATOR_SET_BUTTON9_INDICATION_V2                                              0x4B
#define INDICATOR_SET_BUTTON10_INDICATION_V2                                             0x4C
#define INDICATOR_SET_BUTTON11_INDICATION_V2                                             0x4D
#define INDICATOR_SET_BUTTON12_INDICATION_V2                                             0x4E
#define INDICATOR_SET_BUZZER_V2                                                          0xF0
#define INDICATOR_SET_MULTILEVEL_V2                                                      0x01
#define INDICATOR_SET_BINARY_V2                                                          0x02
#define INDICATOR_SET_ON_OFF_PERIOD_V2                                                   0x03
#define INDICATOR_SET_ON_OFF_CYCLES_V2                                                   0x04
#define INDICATOR_SET_LOW_POWER_V2                                                       0x10
/* Values used for Indicator Supported Get command */
#define INDICATOR_SUPPORTED_GET_NA_V2                                                    0x00
#define INDICATOR_SUPPORTED_GET_ARMED_V2                                                 0x01
#define INDICATOR_SUPPORTED_GET_NOT_ARMED_V2                                             0x02
#define INDICATOR_SUPPORTED_GET_READY_V2                                                 0x03
#define INDICATOR_SUPPORTED_GET_FAULT_V2                                                 0x04
#define INDICATOR_SUPPORTED_GET_BUSY_V2                                                  0x05
#define INDICATOR_SUPPORTED_GET_ENTER_ID_V2                                              0x06
#define INDICATOR_SUPPORTED_GET_ENTER_PIN_V2                                             0x07
#define INDICATOR_SUPPORTED_GET_OK_V2                                                    0x08
#define INDICATOR_SUPPORTED_GET_NOT_OK_V2                                                0x09
#define INDICATOR_SUPPORTED_GET_ZONE1_ARMED_V2                                           0x20
#define INDICATOR_SUPPORTED_GET_ZONE2_ARMED_V2                                           0x21
#define INDICATOR_SUPPORTED_GET_ZONE3_ARMED_V2                                           0x22
#define INDICATOR_SUPPORTED_GET_ZONE4_ARMED_V2                                           0x23
#define INDICATOR_SUPPORTED_GET_ZONE5_ARMED_V2                                           0x24
#define INDICATOR_SUPPORTED_GET_ZONE6_ARMED_V2                                           0x25
#define INDICATOR_SUPPORTED_GET_LCD_BACKLIGHT_V2                                         0x30
#define INDICATOR_SUPPORTED_GET_BUTTON_BACKLIGHT_LETTERS_V2                              0x40
#define INDICATOR_SUPPORTED_GET_BUTTON_BACKLIGHT_DIGITS_V2                               0x41
#define INDICATOR_SUPPORTED_GET_BUTTON_BACKLIGHT_COMMAND_V2                              0x42
#define INDICATOR_SUPPORTED_GET_BUTTON1_INDICATION_V2                                    0x43
#define INDICATOR_SUPPORTED_GET_BUTTON2_INDICATION_V2                                    0x44
#define INDICATOR_SUPPORTED_GET_BUTTON3_INDICATION_V2                                    0x45
#define INDICATOR_SUPPORTED_GET_BUTTON4_INDICATION_V2                                    0x46
#define INDICATOR_SUPPORTED_GET_BUTTON5_INDICATION_V2                                    0x47
#define INDICATOR_SUPPORTED_GET_BUTTON6_INDICATION_V2                                    0x48
#define INDICATOR_SUPPORTED_GET_BUTTON7_INDICATION_V2                                    0x49
#define INDICATOR_SUPPORTED_GET_BUTTON8_INDICATION_V2                                    0x4A
#define INDICATOR_SUPPORTED_GET_BUTTON9_INDICATION_V2                                    0x4B
#define INDICATOR_SUPPORTED_GET_BUTTON10_INDICATION_V2                                   0x4C
#define INDICATOR_SUPPORTED_GET_BUTTON11_INDICATION_V2                                   0x4D
#define INDICATOR_SUPPORTED_GET_BUTTON12_INDICATION_V2                                   0x4E
#define INDICATOR_SUPPORTED_GET_BUZZER_V2                                                0xF0
/* Values used for Indicator Supported Report command */
#define INDICATOR_SUPPORTED_REPORT_NA_V2                                                 0x00
#define INDICATOR_SUPPORTED_REPORT_ARMED_V2                                              0x01
#define INDICATOR_SUPPORTED_REPORT_NOT_ARMED_V2                                          0x02
#define INDICATOR_SUPPORTED_REPORT_READY_V2                                              0x03
#define INDICATOR_SUPPORTED_REPORT_FAULT_V2                                              0x04
#define INDICATOR_SUPPORTED_REPORT_BUSY_V2                                               0x05
#define INDICATOR_SUPPORTED_REPORT_ENTER_ID_V2                                           0x06
#define INDICATOR_SUPPORTED_REPORT_ENTER_PIN_V2                                          0x07
#define INDICATOR_SUPPORTED_REPORT_OK_V2                                                 0x08
#define INDICATOR_SUPPORTED_REPORT_NOT_OK_V2                                             0x09
#define INDICATOR_SUPPORTED_REPORT_ZONE1_ARMED_V2                                        0x20
#define INDICATOR_SUPPORTED_REPORT_ZONE2_ARMED_V2                                        0x21
#define INDICATOR_SUPPORTED_REPORT_ZONE3_ARMED_V2                                        0x22
#define INDICATOR_SUPPORTED_REPORT_ZONE4_ARMED_V2                                        0x23
#define INDICATOR_SUPPORTED_REPORT_ZONE5_ARMED_V2                                        0x24
#define INDICATOR_SUPPORTED_REPORT_ZONE6_ARMED_V2                                        0x25
#define INDICATOR_SUPPORTED_REPORT_LCD_BACKLIGHT_V2                                      0x30
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_LETTERS_V2                           0x40
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_DIGITS_V2                            0x41
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_COMMAND_V2                           0x42
#define INDICATOR_SUPPORTED_REPORT_BUTTON1_INDICATION_V2                                 0x43
#define INDICATOR_SUPPORTED_REPORT_BUTTON2_INDICATION_V2                                 0x44
#define INDICATOR_SUPPORTED_REPORT_BUTTON3_INDICATION_V2                                 0x45
#define INDICATOR_SUPPORTED_REPORT_BUTTON4_INDICATION_V2                                 0x46
#define INDICATOR_SUPPORTED_REPORT_BUTTON5_INDICATION_V2                                 0x47
#define INDICATOR_SUPPORTED_REPORT_BUTTON6_INDICATION_V2                                 0x48
#define INDICATOR_SUPPORTED_REPORT_BUTTON7_INDICATION_V2                                 0x49
#define INDICATOR_SUPPORTED_REPORT_BUTTON8_INDICATION_V2                                 0x4A
#define INDICATOR_SUPPORTED_REPORT_BUTTON9_INDICATION_V2                                 0x4B
#define INDICATOR_SUPPORTED_REPORT_BUTTON10_INDICATION_V2                                0x4C
#define INDICATOR_SUPPORTED_REPORT_BUTTON11_INDICATION_V2                                0x4D
#define INDICATOR_SUPPORTED_REPORT_BUTTON12_INDICATION_V2                                0x4E
#define INDICATOR_SUPPORTED_REPORT_BUZZER_V2                                             0xF0
#define INDICATOR_SUPPORTED_REPORT_NA_V2                                                 0x00
#define INDICATOR_SUPPORTED_REPORT_ARMED_V2                                              0x01
#define INDICATOR_SUPPORTED_REPORT_NOT_ARMED_V2                                          0x02
#define INDICATOR_SUPPORTED_REPORT_READY_V2                                              0x03
#define INDICATOR_SUPPORTED_REPORT_FAULT_V2                                              0x04
#define INDICATOR_SUPPORTED_REPORT_BUSY_V2                                               0x05
#define INDICATOR_SUPPORTED_REPORT_ENTER_ID_V2                                           0x06
#define INDICATOR_SUPPORTED_REPORT_ENTER_PIN_V2                                          0x07
#define INDICATOR_SUPPORTED_REPORT_OK_V2                                                 0x08
#define INDICATOR_SUPPORTED_REPORT_NOT_OK_V2                                             0x09
#define INDICATOR_SUPPORTED_REPORT_ZONE1_ARMED_V2                                        0x20
#define INDICATOR_SUPPORTED_REPORT_ZONE2_ARMED_V2                                        0x21
#define INDICATOR_SUPPORTED_REPORT_ZONE3_ARMED_V2                                        0x22
#define INDICATOR_SUPPORTED_REPORT_ZONE4_ARMED_V2                                        0x23
#define INDICATOR_SUPPORTED_REPORT_ZONE5_ARMED_V2                                        0x24
#define INDICATOR_SUPPORTED_REPORT_ZONE6_ARMED_V2                                        0x25
#define INDICATOR_SUPPORTED_REPORT_LCD_BACKLIGHT_V2                                      0x30
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_LETTERS_V2                           0x40
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_DIGITS_V2                            0x41
#define INDICATOR_SUPPORTED_REPORT_BUTTON_BACKLIGHT_COMMAND_V2                           0x42
#define INDICATOR_SUPPORTED_REPORT_BUTTON1_INDICATION_V2                                 0x43
#define INDICATOR_SUPPORTED_REPORT_BUTTON2_INDICATION_V2                                 0x44
#define INDICATOR_SUPPORTED_REPORT_BUTTON3_INDICATION_V2                                 0x45
#define INDICATOR_SUPPORTED_REPORT_BUTTON4_INDICATION_V2                                 0x46
#define INDICATOR_SUPPORTED_REPORT_BUTTON5_INDICATION_V2                                 0x47
#define INDICATOR_SUPPORTED_REPORT_BUTTON6_INDICATION_V2                                 0x48
#define INDICATOR_SUPPORTED_REPORT_BUTTON7_INDICATION_V2                                 0x49
#define INDICATOR_SUPPORTED_REPORT_BUTTON8_INDICATION_V2                                 0x4A
#define INDICATOR_SUPPORTED_REPORT_BUTTON9_INDICATION_V2                                 0x4B
#define INDICATOR_SUPPORTED_REPORT_BUTTON10_INDICATION_V2                                0x4C
#define INDICATOR_SUPPORTED_REPORT_BUTTON11_INDICATION_V2                                0x4D
#define INDICATOR_SUPPORTED_REPORT_BUTTON12_INDICATION_V2                                0x4E
#define INDICATOR_SUPPORTED_REPORT_BUZZER_V2                                             0xF0
#define INDICATOR_SUPPORTED_REPORT_PROPERTIES1_PROPERTY_SUPPORTED_BIT_MASK_LENGTH_MASK_V2 0x1F
#define INDICATOR_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V2                          0xE0
#define INDICATOR_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                         0x05

/* Ip Configuration command class commands */
#define IP_CONFIGURATION_VERSION                                                         0x01
#define IP_CONFIGURATION_GET                                                             0x02
#define IP_CONFIGURATION_RELEASE                                                         0x04
#define IP_CONFIGURATION_RENEW                                                           0x05
#define IP_CONFIGURATION_REPORT                                                          0x03
#define IP_CONFIGURATION_SET                                                             0x01
/* Values used for Ip Configuration Report command */
#define IP_CONFIGURATION_REPORT_PROPERTIES1_AUTO_DNS_BIT_MASK                            0x01
#define IP_CONFIGURATION_REPORT_PROPERTIES1_AUTO_IP_BIT_MASK                             0x02
#define IP_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_MASK                                0xFC
#define IP_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_SHIFT                               0x02
/* Values used for Ip Configuration Set command */
#define IP_CONFIGURATION_SET_PROPERTIES1_AUTO_DNS_BIT_MASK                               0x01
#define IP_CONFIGURATION_SET_PROPERTIES1_AUTO_IP_BIT_MASK                                0x02
#define IP_CONFIGURATION_SET_PROPERTIES1_RESERVED_MASK                                   0xFC
#define IP_CONFIGURATION_SET_PROPERTIES1_RESERVED_SHIFT                                  0x02

/* Language command class commands */
#define LANGUAGE_VERSION                                                                 0x01
#define LANGUAGE_GET                                                                     0x02
#define LANGUAGE_REPORT                                                                  0x03
#define LANGUAGE_SET                                                                     0x01

/* Lock command class commands */
#define LOCK_VERSION                                                                     0x01
#define LOCK_GET                                                                         0x02
#define LOCK_REPORT                                                                      0x03
#define LOCK_SET                                                                         0x01

/* Manufacturer Proprietary command class commands */
#define MANUFACTURER_PROPRIETARY_VERSION                                                 0x01

/* Manufacturer Specific command class commands */
#define MANUFACTURER_SPECIFIC_VERSION                                                    0x01
#define MANUFACTURER_SPECIFIC_GET                                                        0x04
#define MANUFACTURER_SPECIFIC_REPORT                                                     0x05

/* Manufacturer Specific command class commands */
#define MANUFACTURER_SPECIFIC_VERSION_V2                                                 0x02
#define MANUFACTURER_SPECIFIC_GET_V2                                                     0x04
#define MANUFACTURER_SPECIFIC_REPORT_V2                                                  0x05
#define DEVICE_SPECIFIC_GET_V2                                                           0x06
#define DEVICE_SPECIFIC_REPORT_V2                                                        0x07
/* Values used for Device Specific Get command */
#define DEVICE_SPECIFIC_GET_PROPERTIES1_DEVICE_ID_TYPE_MASK_V2                           0x07
#define DEVICE_SPECIFIC_GET_DEVICE_ID_TYPE_RESERVED_V2                                   0x00
#define DEVICE_SPECIFIC_GET_DEVICE_ID_TYPE_SERIAL_NUMBER_V2                              0x01
#define DEVICE_SPECIFIC_GET_PROPERTIES1_RESERVED_MASK_V2                                 0xF8
#define DEVICE_SPECIFIC_GET_PROPERTIES1_RESERVED_SHIFT_V2                                0x03
/* Values used for Device Specific Report command */
#define DEVICE_SPECIFIC_REPORT_PROPERTIES1_DEVICE_ID_TYPE_MASK_V2                        0x07
#define DEVICE_SPECIFIC_REPORT_DEVICE_ID_TYPE_RESERVED_V2                                0x00
#define DEVICE_SPECIFIC_REPORT_DEVICE_ID_TYPE_SERIAL_NUMBER_V2                           0x01
#define DEVICE_SPECIFIC_REPORT_PROPERTIES1_RESERVED_MASK_V2                              0xF8
#define DEVICE_SPECIFIC_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                             0x03
#define DEVICE_SPECIFIC_REPORT_PROPERTIES2_DEVICE_ID_DATA_LENGTH_INDICATOR_MASK_V2       0x1F
#define DEVICE_SPECIFIC_REPORT_PROPERTIES2_DEVICE_ID_DATA_FORMAT_MASK_V2                 0xE0
#define DEVICE_SPECIFIC_REPORT_PROPERTIES2_DEVICE_ID_DATA_FORMAT_SHIFT_V2                0x05
#define DEVICE_SPECIFIC_REPORT_DEVICE_ID_DATA_FORMAT_RESERVED_V2                         0x00
#define DEVICE_SPECIFIC_REPORT_DEVICE_ID_DATA_FORMAT_BINARY_V2                           0x01

/* Mark command class commands */
#define MARK_VERSION                                                                     0x01

/* Meter Pulse command class commands */
#define METER_PULSE_VERSION                                                              0x01
#define METER_PULSE_GET                                                                  0x04
#define METER_PULSE_REPORT                                                               0x05

/* Meter Tbl Config command class commands */
#define METER_TBL_CONFIG_VERSION                                                         0x01
#define METER_TBL_TABLE_POINT_ADM_NO_SET                                                 0x01
/* Values used for Meter Tbl Table Point Adm No Set command */
#define METER_TBL_TABLE_POINT_ADM_NO_SET_PROPERTIES1_NUMBER_OF_CHARACTERS_MASK           0x1F
#define METER_TBL_TABLE_POINT_ADM_NO_SET_PROPERTIES1_RESERVED_MASK                       0xE0
#define METER_TBL_TABLE_POINT_ADM_NO_SET_PROPERTIES1_RESERVED_SHIFT                      0x05

/* Meter Tbl Monitor command class commands */
#define METER_TBL_MONITOR_VERSION                                                        0x01
#define METER_TBL_STATUS_REPORT                                                          0x0B
#define METER_TBL_STATUS_DATE_GET                                                        0x0A
#define METER_TBL_STATUS_DEPTH_GET                                                       0x09
#define METER_TBL_STATUS_SUPPORTED_GET                                                   0x07
#define METER_TBL_STATUS_SUPPORTED_REPORT                                                0x08
#define METER_TBL_CURRENT_DATA_GET                                                       0x0C
#define METER_TBL_CURRENT_DATA_REPORT                                                    0x0D
#define METER_TBL_HISTORICAL_DATA_GET                                                    0x0E
#define METER_TBL_HISTORICAL_DATA_REPORT                                                 0x0F
#define METER_TBL_REPORT                                                                 0x06
#define METER_TBL_TABLE_CAPABILITY_GET                                                   0x05
#define METER_TBL_TABLE_ID_GET                                                           0x03
#define METER_TBL_TABLE_ID_REPORT                                                        0x04
#define METER_TBL_TABLE_POINT_ADM_NO_GET                                                 0x01
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT                                              0x02
/* Values used for Meter Tbl Current Data Report command */
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RATE_TYPE_MASK                         0x03
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RESERVED_MASK                          0xFC
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RESERVED_SHIFT                         0x02
/* Values used for Meter Tbl Historical Data Report command */
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RATE_TYPE_MASK                      0x03
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RESERVED_MASK                       0xFC
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RESERVED_SHIFT                      0x02
/* Values used for Meter Tbl Report command */
#define METER_TBL_REPORT_PROPERTIES1_METER_TYPE_MASK                                     0x3F
#define METER_TBL_REPORT_PROPERTIES1_RATE_TYPE_MASK                                      0xC0
#define METER_TBL_REPORT_PROPERTIES1_RATE_TYPE_SHIFT                                     0x06
#define METER_TBL_REPORT_PROPERTIES2_PAY_METER_MASK                                      0x0F
#define METER_TBL_REPORT_PAY_METER_RESERVED                                              0x00
#define METER_TBL_REPORT_PAY_METER_CREDITMETER                                           0x01
#define METER_TBL_REPORT_PAY_METER_PREPAYMENT_METER                                      0x02
#define METER_TBL_REPORT_PAY_METER_PREPAYMENT_METER_DEBT                                 0x03
#define METER_TBL_REPORT_PROPERTIES2_RESERVED_MASK                                       0xF0
#define METER_TBL_REPORT_PROPERTIES2_RESERVED_SHIFT                                      0x04
/* Values used for Meter Tbl Table Id Report command */
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_NUMBER_OF_CHARACTERS_MASK                  0x1F
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_RESERVED_MASK                              0xE0
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_RESERVED_SHIFT                             0x05
/* Values used for Meter Tbl Table Point Adm No Report command */
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_NUMBER_OF_CHARACTERS_MASK        0x1F
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_RESERVED_MASK                    0xE0
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_RESERVED_SHIFT                   0x05

/* Meter Tbl Monitor command class commands */
#define METER_TBL_MONITOR_VERSION_V2                                                     0x02
#define METER_TBL_STATUS_REPORT_V2                                                       0x0B
#define METER_TBL_STATUS_DATE_GET_V2                                                     0x0A
#define METER_TBL_STATUS_DEPTH_GET_V2                                                    0x09
#define METER_TBL_STATUS_SUPPORTED_GET_V2                                                0x07
#define METER_TBL_STATUS_SUPPORTED_REPORT_V2                                             0x08
#define METER_TBL_CURRENT_DATA_GET_V2                                                    0x0C
#define METER_TBL_CURRENT_DATA_REPORT_V2                                                 0x0D
#define METER_TBL_HISTORICAL_DATA_GET_V2                                                 0x0E
#define METER_TBL_HISTORICAL_DATA_REPORT_V2                                              0x0F
#define METER_TBL_REPORT_V2                                                              0x06
#define METER_TBL_TABLE_CAPABILITY_GET_V2                                                0x05
#define METER_TBL_TABLE_ID_GET_V2                                                        0x03
#define METER_TBL_TABLE_ID_REPORT_V2                                                     0x04
#define METER_TBL_TABLE_POINT_ADM_NO_GET_V2                                              0x01
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_V2                                           0x02
/* Values used for Meter Tbl Current Data Report command */
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RATE_TYPE_MASK_V2                      0x03
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RESERVED_MASK_V2                       0x7C
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                      0x02
#define METER_TBL_CURRENT_DATA_REPORT_PROPERTIES1_OPERATING_STATUS_INDICATION_BIT_MASK_V2 0x80
/* Values used for Meter Tbl Historical Data Report command */
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RATE_TYPE_MASK_V2                   0x03
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RESERVED_MASK_V2                    0x7C
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                   0x02
#define METER_TBL_HISTORICAL_DATA_REPORT_PROPERTIES1_OPERATING_STATUS_INDICATION_BIT_MASK_V2 0x80
/* Values used for Meter Tbl Report command */
#define METER_TBL_REPORT_PROPERTIES1_METER_TYPE_MASK_V2                                  0x3F
#define METER_TBL_REPORT_PROPERTIES1_RATE_TYPE_MASK_V2                                   0xC0
#define METER_TBL_REPORT_PROPERTIES1_RATE_TYPE_SHIFT_V2                                  0x06
#define METER_TBL_REPORT_PROPERTIES2_PAY_METER_MASK_V2                                   0x0F
#define METER_TBL_REPORT_PAY_METER_RESERVED_V2                                           0x00
#define METER_TBL_REPORT_PAY_METER_CREDITMETER_V2                                        0x01
#define METER_TBL_REPORT_PAY_METER_PREPAYMENT_METER_V2                                   0x02
#define METER_TBL_REPORT_PAY_METER_PREPAYMENT_METER_DEBT_V2                              0x03
#define METER_TBL_REPORT_PROPERTIES2_RESERVED_MASK_V2                                    0xF0
#define METER_TBL_REPORT_PROPERTIES2_RESERVED_SHIFT_V2                                   0x04
/* Values used for Meter Tbl Table Id Report command */
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_NUMBER_OF_CHARACTERS_MASK_V2               0x1F
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_RESERVED_MASK_V2                           0xE0
#define METER_TBL_TABLE_ID_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                          0x05
/* Values used for Meter Tbl Table Point Adm No Report command */
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_NUMBER_OF_CHARACTERS_MASK_V2     0x1F
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_RESERVED_MASK_V2                 0xE0
#define METER_TBL_TABLE_POINT_ADM_NO_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                0x05

/* Meter Tbl Push command class commands */
#define METER_TBL_PUSH_VERSION                                                           0x01
#define METER_TBL_PUSH_CONFIGURATION_GET                                                 0x02
#define METER_TBL_PUSH_CONFIGURATION_REPORT                                              0x03
#define METER_TBL_PUSH_CONFIGURATION_SET                                                 0x01
/* Values used for Meter Tbl Push Configuration Report command */
#define METER_TBL_PUSH_CONFIGURATION_REPORT_PROPERTIES1_OPERATING_STATUS_PUSH_MODE_MASK  0x0F
#define METER_TBL_PUSH_CONFIGURATION_REPORT_PROPERTIES1_PS_BIT_MASK                      0x10
#define METER_TBL_PUSH_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_MASK                    0xE0
#define METER_TBL_PUSH_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_SHIFT                   0x05
/* Values used for Meter Tbl Push Configuration Set command */
#define METER_TBL_PUSH_CONFIGURATION_SET_PROPERTIES1_OPERATING_STATUS_PUSH_MODE_MASK     0x0F
#define METER_TBL_PUSH_CONFIGURATION_SET_PROPERTIES1_PS_BIT_MASK                         0x10
#define METER_TBL_PUSH_CONFIGURATION_SET_PROPERTIES1_RESERVED_MASK                       0xE0
#define METER_TBL_PUSH_CONFIGURATION_SET_PROPERTIES1_RESERVED_SHIFT                      0x05

/* Meter command class commands */
#define METER_VERSION                                                                    0x01
#define METER_GET                                                                        0x01
#define METER_REPORT                                                                     0x02
/* Values used for Meter Report command */
#define METER_REPORT_ELECTRIC_METER                                                      0x01
#define METER_REPORT_GAS_METER                                                           0x02
#define METER_REPORT_WATER_METER                                                         0x03
#define METER_REPORT_PROPERTIES1_SIZE_MASK                                               0x07
#define METER_REPORT_PROPERTIES1_SCALE_MASK                                              0x18
#define METER_REPORT_PROPERTIES1_SCALE_SHIFT                                             0x03
#define METER_REPORT_PROPERTIES1_PRECISION_MASK                                          0xE0
#define METER_REPORT_PROPERTIES1_PRECISION_SHIFT                                         0x05

/* Meter command class commands */
#define METER_VERSION_V2                                                                 0x02
#define METER_GET_V2                                                                     0x01
#define METER_REPORT_V2                                                                  0x02
#define METER_RESET_V2                                                                   0x05
#define METER_SUPPORTED_GET_V2                                                           0x03
#define METER_SUPPORTED_REPORT_V2                                                        0x04
/* Values used for Meter Get command */
#define METER_GET_PROPERTIES1_RESERVED_MASK_V2                                           0x07
#define METER_GET_PROPERTIES1_SCALE_MASK_V2                                              0x18
#define METER_GET_PROPERTIES1_SCALE_SHIFT_V2                                             0x03
#define METER_GET_PROPERTIES1_RESERVED2_MASK_V2                                          0xE0
#define METER_GET_PROPERTIES1_RESERVED2_SHIFT_V2                                         0x05
/* Values used for Meter Report command */
#define METER_REPORT_PROPERTIES1_METER_TYPE_MASK_V2                                      0x1F
#define METER_REPORT_PROPERTIES1_RATE_TYPE_MASK_V2                                       0x60
#define METER_REPORT_PROPERTIES1_RATE_TYPE_SHIFT_V2                                      0x05
#define METER_REPORT_PROPERTIES1_RESERVED_BIT_MASK_V2                                    0x80
#define METER_REPORT_PROPERTIES2_SIZE_MASK_V2                                            0x07
#define METER_REPORT_PROPERTIES2_SCALE_MASK_V2                                           0x18
#define METER_REPORT_PROPERTIES2_SCALE_SHIFT_V2                                          0x03
#define METER_REPORT_PROPERTIES2_PRECISION_MASK_V2                                       0xE0
#define METER_REPORT_PROPERTIES2_PRECISION_SHIFT_V2                                      0x05
/* Values used for Meter Supported Report command */
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_TYPE_MASK_V2                            0x1F
#define METER_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_MASK_V2                             0x60
#define METER_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_SHIFT_V2                            0x05
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_RESET_BIT_MASK_V2                       0x80
#define METER_SUPPORTED_REPORT_PROPERTIES2_SCALE_SUPPORTED_MASK_V2                       0x0F
#define METER_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_MASK_V2                             0xF0
#define METER_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_SHIFT_V2                            0x04

/* Meter command class commands */
#define METER_VERSION_V3                                                                 0x03
#define METER_GET_V3                                                                     0x01
#define METER_REPORT_V3                                                                  0x02
#define METER_RESET_V3                                                                   0x05
#define METER_SUPPORTED_GET_V3                                                           0x03
#define METER_SUPPORTED_REPORT_V3                                                        0x04
/* Values used for Meter Get command */
#define METER_GET_PROPERTIES1_RESERVED_MASK_V3                                           0x07
#define METER_GET_PROPERTIES1_SCALE_MASK_V3                                              0x38
#define METER_GET_PROPERTIES1_SCALE_SHIFT_V3                                             0x03
#define METER_GET_PROPERTIES1_RESERVED2_MASK_V3                                          0xC0
#define METER_GET_PROPERTIES1_RESERVED2_SHIFT_V3                                         0x06
/* Values used for Meter Report command */
#define METER_REPORT_PROPERTIES1_METER_TYPE_MASK_V3                                      0x1F
#define METER_REPORT_PROPERTIES1_RATE_TYPE_MASK_V3                                       0x60
#define METER_REPORT_PROPERTIES1_RATE_TYPE_SHIFT_V3                                      0x05
#define METER_REPORT_PROPERTIES1_SCALE_BIT_2_BIT_MASK_V3                                 0x80
#define METER_REPORT_PROPERTIES2_SIZE_MASK_V3                                            0x07
#define METER_REPORT_PROPERTIES2_SCALE_BITS_10_MASK_V3                                   0x18
#define METER_REPORT_PROPERTIES2_SCALE_BITS_10_SHIFT_V3                                  0x03
#define METER_REPORT_PROPERTIES2_PRECISION_MASK_V3                                       0xE0
#define METER_REPORT_PROPERTIES2_PRECISION_SHIFT_V3                                      0x05
/* Values used for Meter Supported Report command */
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_TYPE_MASK_V3                            0x1F
#define METER_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V3                              0x60
#define METER_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                             0x05
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_RESET_BIT_MASK_V3                       0x80

/* Meter command class commands */
#define METER_VERSION_V4                                                                 0x04
#define METER_GET_V4                                                                     0x01
#define METER_REPORT_V4                                                                  0x02
#define METER_RESET_V4                                                                   0x05
#define METER_SUPPORTED_GET_V4                                                           0x03
#define METER_SUPPORTED_REPORT_V4                                                        0x04
/* Values used for Meter Get command */
#define METER_GET_PROPERTIES1_RESERVED_MASK_V4                                           0x07
#define METER_GET_PROPERTIES1_SCALE_MASK_V4                                              0x38
#define METER_GET_PROPERTIES1_SCALE_SHIFT_V4                                             0x03
#define METER_GET_PROPERTIES1_RATE_TYPE_MASK_V4                                          0xC0
#define METER_GET_PROPERTIES1_RATE_TYPE_SHIFT_V4                                         0x06
#define METER_GET_RATE_TYPE_RESERVED_V4                                                  0x00
#define METER_GET_RATE_TYPE_IMPORT_V4                                                    0x01
#define METER_GET_RATE_TYPE_EXPORT_V4                                                    0x02
#define METER_GET_RATE_TYPE_NOT_TO_BE_USED_V4                                            0x03
/* Values used for Meter Report command */
#define METER_REPORT_PROPERTIES1_METER_TYPE_MASK_V4                                      0x1F
#define METER_REPORT_METER_TYPE_RESERVED_V4                                              0x00
#define METER_REPORT_METER_TYPE_ELECTRIC_METER_V4                                        0x01
#define METER_REPORT_METER_TYPE_GAS_METER_V4                                             0x02
#define METER_REPORT_METER_TYPE_WATER_METER_V4                                           0x03
#define METER_REPORT_PROPERTIES1_RATE_TYPE_MASK_V4                                       0x60
#define METER_REPORT_PROPERTIES1_RATE_TYPE_SHIFT_V4                                      0x05
#define METER_REPORT_RATE_TYPE_RESERVED_V4                                               0x00
#define METER_REPORT_RATE_TYPE_IMPORT_V4                                                 0x01
#define METER_REPORT_RATE_TYPE_EXPORT_V4                                                 0x02
#define METER_REPORT_RATE_TYPE_NOT_TO_BE_USED_V4                                         0x03
#define METER_REPORT_PROPERTIES1_SCALE_BIT_2_BIT_MASK_V4                                 0x80
#define METER_REPORT_PROPERTIES2_SIZE_MASK_V4                                            0x07
#define METER_REPORT_PROPERTIES2_SCALE_BITS_10_MASK_V4                                   0x18
#define METER_REPORT_PROPERTIES2_SCALE_BITS_10_SHIFT_V4                                  0x03
#define METER_REPORT_PROPERTIES2_PRECISION_MASK_V4                                       0xE0
#define METER_REPORT_PROPERTIES2_PRECISION_SHIFT_V4                                      0x05
/* Values used for Meter Supported Report command */
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_TYPE_MASK_V4                            0x1F
#define METER_SUPPORTED_REPORT_METER_TYPE_RESERVED_V4                                    0x00
#define METER_SUPPORTED_REPORT_METER_TYPE_ELECTRIC_METER_V4                              0x01
#define METER_SUPPORTED_REPORT_METER_TYPE_GAS_METER_V4                                   0x02
#define METER_SUPPORTED_REPORT_METER_TYPE_WATER_METER_V4                                 0x03
#define METER_SUPPORTED_REPORT_PROPERTIES1_RATE_TYPE_MASK_V4                             0x60
#define METER_SUPPORTED_REPORT_PROPERTIES1_RATE_TYPE_SHIFT_V4                            0x05
#define METER_SUPPORTED_REPORT_RATE_TYPE_RESERVED_V4                                     0x00
#define METER_SUPPORTED_REPORT_RATE_TYPE_IMPORT_ONLY_V4                                  0x01
#define METER_SUPPORTED_REPORT_RATE_TYPE_EXPORT_ONLY_V4                                  0x02
#define METER_SUPPORTED_REPORT_RATE_TYPE_IMPORT_AND_EXPORT_V4                            0x03
#define METER_SUPPORTED_REPORT_PROPERTIES1_METER_RESET_BIT_MASK_V4                       0x80
#define METER_SUPPORTED_REPORT_PROPERTIES2_SCALE_SUPPORTED_0_MASK_V4                     0x7F
#define METER_SUPPORTED_REPORT_PROPERTIES2_M_S_T_BIT_MASK_V4                             0x80

/* Mtp Window Covering command class commands */
#define MTP_WINDOW_COVERING_VERSION                                                      0x01
#define MOVE_TO_POSITION_GET                                                             0x02
#define MOVE_TO_POSITION_REPORT                                                          0x03
#define MOVE_TO_POSITION_SET                                                             0x01

/* Multi Channel Association command class commands */
#define MULTI_CHANNEL_ASSOCIATION_VERSION_V2                                             0x02
#define MULTI_CHANNEL_ASSOCIATION_GET_V2                                                 0x02
#define MULTI_CHANNEL_ASSOCIATION_GROUPINGS_GET_V2                                       0x05
#define MULTI_CHANNEL_ASSOCIATION_GROUPINGS_REPORT_V2                                    0x06
#define MULTI_CHANNEL_ASSOCIATION_REMOVE_V2                                              0x04
#define MULTI_CHANNEL_ASSOCIATION_REPORT_V2                                              0x03
#define MULTI_CHANNEL_ASSOCIATION_SET_V2                                                 0x01
/* Values used for Multi Channel Association Remove command */
#define MULTI_CHANNEL_ASSOCIATION_REMOVE_MARKER_V2                                       0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Channel Association Report command */
#define MULTI_CHANNEL_ASSOCIATION_REPORT_MARKER_V2                                       0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Channel Association Set command */
#define MULTI_CHANNEL_ASSOCIATION_SET_MARKER_V2                                          0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/

/* Multi Channel Association command class commands */
#define MULTI_CHANNEL_ASSOCIATION_VERSION_V3                                             0x03
#define MULTI_CHANNEL_ASSOCIATION_GET_V3                                                 0x02
#define MULTI_CHANNEL_ASSOCIATION_GROUPINGS_GET_V3                                       0x05
#define MULTI_CHANNEL_ASSOCIATION_GROUPINGS_REPORT_V3                                    0x06
#define MULTI_CHANNEL_ASSOCIATION_REMOVE_V3                                              0x04
#define MULTI_CHANNEL_ASSOCIATION_REPORT_V3                                              0x03
#define MULTI_CHANNEL_ASSOCIATION_SET_V3                                                 0x01
/* Values used for Multi Channel Association Remove command */
#define MULTI_CHANNEL_ASSOCIATION_REMOVE_MARKER_V3                                       0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Channel Association Report command */
#define MULTI_CHANNEL_ASSOCIATION_REPORT_MARKER_V3                                       0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Channel Association Set command */
#define MULTI_CHANNEL_ASSOCIATION_SET_MARKER_V3                                          0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/

/* Multi Channel command class commands */
#define MULTI_CHANNEL_VERSION_V2                                                         0x02
#define MULTI_CHANNEL_CAPABILITY_GET_V2                                                  0x09
#define MULTI_CHANNEL_CAPABILITY_REPORT_V2                                               0x0A
#define MULTI_CHANNEL_CMD_ENCAP_V2                                                       0x0D
#define MULTI_CHANNEL_END_POINT_FIND_V2                                                  0x0B
#define MULTI_CHANNEL_END_POINT_FIND_REPORT_V2                                           0x0C
#define MULTI_CHANNEL_END_POINT_GET_V2                                                   0x07
#define MULTI_CHANNEL_END_POINT_REPORT_V2                                                0x08
#define MULTI_INSTANCE_CMD_ENCAP_V2                                                      0x06
#define MULTI_INSTANCE_GET_V2                                                            0x04
#define MULTI_INSTANCE_REPORT_V2                                                         0x05
/* Values used for Multi Channel Capability Get command */
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_END_POINT_MASK_V2                       0x7F
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_RES_BIT_MASK_V2                         0x80
/* Values used for Multi Channel Capability Report command */
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_END_POINT_MASK_V2                    0x7F
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V2                  0x80
/* Values used for Multi Channel Cmd Encap command */
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_SOURCE_END_POINT_MASK_V2                     0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V2                              0x80
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_DESTINATION_END_POINT_MASK_V2                0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_BIT_ADDRESS_BIT_MASK_V2                      0x80
/* Values used for Multi Channel End Point Report command */
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_RES1_MASK_V2                          0x3F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_IDENTICAL_BIT_MASK_V2                 0x40
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V2                   0x80
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_END_POINTS_MASK_V2                    0x7F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_RES2_BIT_MASK_V2                      0x80
/* Values used for Multi Instance Cmd Encap command */
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_INSTANCE_MASK_V2                            0x7F
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V2                             0x80
/* Values used for Multi Instance Report command */
#define MULTI_INSTANCE_REPORT_PROPERTIES1_INSTANCES_MASK_V2                              0x7F
#define MULTI_INSTANCE_REPORT_PROPERTIES1_RES_BIT_MASK_V2                                0x80

/* Multi Channel command class commands */
#define MULTI_CHANNEL_VERSION_V3                                                         0x03
#define MULTI_CHANNEL_CAPABILITY_GET_V3                                                  0x09
#define MULTI_CHANNEL_CAPABILITY_REPORT_V3                                               0x0A
#define MULTI_CHANNEL_CMD_ENCAP_V3                                                       0x0D
#define MULTI_CHANNEL_END_POINT_FIND_V3                                                  0x0B
#define MULTI_CHANNEL_END_POINT_FIND_REPORT_V3                                           0x0C
#define MULTI_CHANNEL_END_POINT_GET_V3                                                   0x07
#define MULTI_CHANNEL_END_POINT_REPORT_V3                                                0x08
#define MULTI_INSTANCE_CMD_ENCAP_V3                                                      0x06
#define MULTI_INSTANCE_GET_V3                                                            0x04
#define MULTI_INSTANCE_REPORT_V3                                                         0x05
/* Values used for Multi Channel Capability Get command */
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_END_POINT_MASK_V3                       0x7F
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_RES_BIT_MASK_V3                         0x80
/* Values used for Multi Channel Capability Report command */
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_END_POINT_MASK_V3                    0x7F
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V3                  0x80
/* Values used for Multi Channel Cmd Encap command */
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_SOURCE_END_POINT_MASK_V3                     0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V3                              0x80
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_DESTINATION_END_POINT_MASK_V3                0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_BIT_ADDRESS_BIT_MASK_V3                      0x80
/* Values used for Multi Channel End Point Report command */
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_RES1_MASK_V3                          0x3F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_IDENTICAL_BIT_MASK_V3                 0x40
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V3                   0x80
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_END_POINTS_MASK_V3                    0x7F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_RES2_BIT_MASK_V3                      0x80
/* Values used for Multi Instance Cmd Encap command */
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_INSTANCE_MASK_V3                            0x7F
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V3                             0x80
/* Values used for Multi Instance Report command */
#define MULTI_INSTANCE_REPORT_PROPERTIES1_INSTANCES_MASK_V3                              0x7F
#define MULTI_INSTANCE_REPORT_PROPERTIES1_RES_BIT_MASK_V3                                0x80

/* Multi Channel command class commands */
#define MULTI_CHANNEL_VERSION_V4                                                         0x04
#define MULTI_CHANNEL_CAPABILITY_GET_V4                                                  0x09
#define MULTI_CHANNEL_CAPABILITY_REPORT_V4                                               0x0A
#define MULTI_CHANNEL_CMD_ENCAP_V4                                                       0x0D
#define MULTI_CHANNEL_END_POINT_FIND_V4                                                  0x0B
#define MULTI_CHANNEL_END_POINT_FIND_REPORT_V4                                           0x0C
#define MULTI_CHANNEL_END_POINT_GET_V4                                                   0x07
#define MULTI_CHANNEL_END_POINT_REPORT_V4                                                0x08
#define MULTI_INSTANCE_CMD_ENCAP_V4                                                      0x06
#define MULTI_INSTANCE_GET_V4                                                            0x04
#define MULTI_INSTANCE_REPORT_V4                                                         0x05
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_V4                                          0x0E
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_V4                                       0x0F
/* Values used for Multi Channel Capability Get command */
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_END_POINT_MASK_V4                       0x7F
#define MULTI_CHANNEL_CAPABILITY_GET_PROPERTIES1_RES_BIT_MASK_V4                         0x80
/* Values used for Multi Channel Capability Report command */
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_END_POINT_MASK_V4                    0x7F
#define MULTI_CHANNEL_CAPABILITY_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V4                  0x80
/* Values used for Multi Channel Cmd Encap command */
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_SOURCE_END_POINT_MASK_V4                     0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V4                              0x80
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_DESTINATION_END_POINT_MASK_V4                0x7F
#define MULTI_CHANNEL_CMD_ENCAP_PROPERTIES2_BIT_ADDRESS_BIT_MASK_V4                      0x80
/* Values used for Multi Channel End Point Report command */
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_RES1_MASK_V4                          0x3F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_IDENTICAL_BIT_MASK_V4                 0x40
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES1_DYNAMIC_BIT_MASK_V4                   0x80
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_INDIVIDUAL_END_POINTS_MASK_V4         0x7F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES2_RES2_BIT_MASK_V4                      0x80
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES3_AGGREGATED_END_POINTS_MASK_V4         0x7F
#define MULTI_CHANNEL_END_POINT_REPORT_PROPERTIES3_RES3_BIT_MASK_V4                      0x80
/* Values used for Multi Instance Cmd Encap command */
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_INSTANCE_MASK_V4                            0x7F
#define MULTI_INSTANCE_CMD_ENCAP_PROPERTIES1_RES_BIT_MASK_V4                             0x80
/* Values used for Multi Instance Report command */
#define MULTI_INSTANCE_REPORT_PROPERTIES1_INSTANCES_MASK_V4                              0x7F
#define MULTI_INSTANCE_REPORT_PROPERTIES1_RES_BIT_MASK_V4                                0x80
/* Values used for Multi Channel Aggregated Members Get command */
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_PROPERTIES1_AGGREGATED_END_POINT_MASK_V4    0x7F
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_GET_PROPERTIES1_RES_BIT_MASK_V4                 0x80
/* Values used for Multi Channel Aggregated Members Report command */
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_PROPERTIES1_AGGREGATED_END_POINT_MASK_V4 0x7F
#define MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_PROPERTIES1_RES_BIT_MASK_V4              0x80

/* Multi Cmd command class commands */
#define MULTI_CMD_VERSION                                                                0x01
#define MULTI_CMD_ENCAP                                                                  0x01

/* Multi Instance Association command class commands */
#define MULTI_INSTANCE_ASSOCIATION_VERSION                                               0x01
#define MULTI_INSTANCE_ASSOCIATION_GET                                                   0x02
#define MULTI_INSTANCE_ASSOCIATION_GROUPINGS_GET                                         0x05
#define MULTI_INSTANCE_ASSOCIATION_GROUPINGS_REPORT                                      0x06
#define MULTI_INSTANCE_ASSOCIATION_REMOVE                                                0x04
#define MULTI_INSTANCE_ASSOCIATION_REPORT                                                0x03
#define MULTI_INSTANCE_ASSOCIATION_SET                                                   0x01
/* Values used for Multi Instance Association Remove command */
#define MULTI_INSTANCE_ASSOCIATION_REMOVE_MARKER                                         0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Instance Association Report command */
#define MULTI_INSTANCE_ASSOCIATION_REPORT_MARKER                                         0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/
/* Values used for Multi Instance Association Set command */
#define MULTI_INSTANCE_ASSOCIATION_SET_MARKER                                            0x00 /*This marker identifier is used to separate between nodes without and with end points attached. This field can be omitted in case no Multi Channel node follows.*/

/* Multi Instance command class commands */
#define MULTI_INSTANCE_VERSION                                                           0x01
#define MULTI_INSTANCE_CMD_ENCAP                                                         0x06
#define MULTI_INSTANCE_GET                                                               0x04
#define MULTI_INSTANCE_REPORT                                                            0x05

/* Network Management Proxy command class commands */
#define NETWORK_MANAGEMENT_PROXY_VERSION                                                 0x01
#define NODE_INFO_CACHED_GET                                                             0x03
#define NODE_INFO_CACHED_REPORT                                                          0x04
#define NODE_LIST_GET                                                                    0x01
#define NODE_LIST_REPORT                                                                 0x02

/* Network Management Proxy command class version 2 commands*/
#define NM_MULTI_CHANNEL_END_POINT_GET             0x05
#define NM_MULTI_CHANNEL_END_POINT_REPORT          0x06
#define NM_MULTI_CHANNEL_CAPABILITY_GET            0x07
#define NM_MULTI_CHANNEL_CAPABILITY_REPORT         0x08
#define NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_GET    0x09
#define NM_MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT 0x0A

/* Values used for Node Info Cached Get command */
#define NODE_INFO_CACHED_GET_PROPERTIES1_MAX_AGE_MASK                                    0x0F
#define NODE_INFO_CACHED_GET_PROPERTIES1_RESERVED_MASK                                   0xF0
#define NODE_INFO_CACHED_GET_PROPERTIES1_RESERVED_SHIFT                                  0x04
/* Values used for Node Info Cached Report command */
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_AGE_MASK                                     0x0F
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_STATUS_MASK                                  0xF0
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_STATUS_SHIFT                                 0x04
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_OK                                         0x00
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_NOT_RESPONDING                             0x01
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_UNKNOWN                                    0x02
#define NODE_INFO_CACHED_REPORT_PROPERTIES2_CAPABILITY_MASK                              0x7F
#define NODE_INFO_CACHED_REPORT_PROPERTIES2_LISTENING_BIT_MASK                           0x80
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SECURITY_MASK                                0x0F
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SENSOR_MASK                                  0x70
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SENSOR_SHIFT                                 0x04
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_OPT_BIT_MASK                                 0x80
#define NODE_INFO_CACHED_REPORT_SECURITY_SCHEME_0_MARK                                   0xF100

/* Network Management Proxy command class commands */
#define NETWORK_MANAGEMENT_PROXY_VERSION_V2                                              0x02
#define NODE_INFO_CACHED_GET_V2                                                          0x03
#define NODE_INFO_CACHED_REPORT_V2                                                       0x04
#define NODE_LIST_GET_V2                                                                 0x01
#define NODE_LIST_REPORT_V2                                                              0x02
/* Values used for Node Info Cached Get command */
#define NODE_INFO_CACHED_GET_PROPERTIES1_MAX_AGE_MASK_V2                                 0x0F
#define NODE_INFO_CACHED_GET_PROPERTIES1_RESERVED_MASK_V2                                0xF0
#define NODE_INFO_CACHED_GET_PROPERTIES1_RESERVED_SHIFT_V2                               0x04
/* Values used for Node Info Cached Report command */
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_AGE_MASK_V2                                  0x0F
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_STATUS_MASK_V2                               0xF0
#define NODE_INFO_CACHED_REPORT_PROPERTIES1_STATUS_SHIFT_V2                              0x04
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_OK_V2                                      0x00
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_NOT_RESPONDING_V2                          0x01
#define NODE_INFO_CACHED_REPORT_STATUS_STATUS_UNKNOWN_V2                                 0x02
#define NODE_INFO_CACHED_REPORT_PROPERTIES2_CAPABILITY_MASK_V2                           0x7F
#define NODE_INFO_CACHED_REPORT_PROPERTIES2_LISTENING_BIT_MASK_V2                        0x80
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SECURITY_MASK_V2                             0x0F
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SENSOR_MASK_V2                               0x70
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_SENSOR_SHIFT_V2                              0x04
#define NODE_INFO_CACHED_REPORT_PROPERTIES3_OPT_BIT_MASK_V2                              0x80
#define NODE_INFO_CACHED_REPORT_SECURITY_MARK_V2                                         0xF100

/* Network Management Basic command class commands */
#define NETWORK_MANAGEMENT_BASIC_VERSION                                                 0x01
#define LEARN_MODE_SET                                                                   0x01
#define LEARN_MODE_SET_STATUS                                                            0x02
#define NODE_INFORMATION_SEND                                                            0x05
#define NETWORK_UPDATE_REQUEST                                                           0x03
#define NETWORK_UPDATE_REQUEST_STATUS                                                    0x04
#define DEFAULT_SET                                                                      0x06
#define DEFAULT_SET_COMPLETE                                                             0x07

/* Network Management Basic command class commands */
#define NETWORK_MANAGEMENT_BASIC_VERSION_V2                                              0x02
#define LEARN_MODE_SET_V2                                                                0x01
#define LEARN_MODE_SET_STATUS_V2                                                         0x02
#define NODE_INFORMATION_SEND_V2                                                         0x05
#define NETWORK_UPDATE_REQUEST_V2                                                        0x03
#define NETWORK_UPDATE_REQUEST_STATUS_V2                                                 0x04
#define DEFAULT_SET_V2                                                                   0x06
#define DEFAULT_SET_COMPLETE_V2                                                          0x07
#define DSK_GET_V2                                                                       0x08
#define DSK_REPORT_V2                                                                    0x09

/* Network Management Inclusion command class commands */
#define NETWORK_MANAGEMENT_INCLUSION_VERSION                                             0x01
#define FAILED_NODE_REMOVE                                                               0x07
#define FAILED_NODE_REMOVE_STATUS                                                        0x08
#define NODE_ADD                                                                         0x01
#define NODE_ADD_STATUS                                                                  0x02
#define NODE_REMOVE                                                                      0x03
#define NODE_REMOVE_STATUS                                                               0x04
#define FAILED_NODE_REPLACE                                                              0x09
#define FAILED_NODE_REPLACE_STATUS                                                       0x0A
#define NODE_NEIGHBOR_UPDATE_REQUEST                                                     0x0B
#define NODE_NEIGHBOR_UPDATE_STATUS                                                      0x0C
#define RETURN_ROUTE_ASSIGN                                                              0x0D
#define RETURN_ROUTE_ASSIGN_COMPLETE                                                     0x0E
#define RETURN_ROUTE_DELETE                                                              0x0F
#define RETURN_ROUTE_DELETE_COMPLETE                                                     0x10
/* Values used for Node Add Status command */
#define NODE_ADD_STATUS_PROPERTIES1_CAPABILITY_MASK                                      0x7F
#define NODE_ADD_STATUS_PROPERTIES1_LISTENING_BIT_MASK                                   0x80
#define NODE_ADD_STATUS_PROPERTIES2_SECURITY_MASK                                        0x7F
#define NODE_ADD_STATUS_PROPERTIES2_OPT_BIT_MASK                                         0x80

/* Network Management Inclusion command class commands */
#define NETWORK_MANAGEMENT_INCLUSION_VERSION_V2                                          0x02
#define FAILED_NODE_REMOVE_V2                                                            0x07
#define FAILED_NODE_REMOVE_STATUS_V2                                                     0x08
#define NODE_ADD_V2                                                                      0x01
#define NODE_ADD_STATUS_V2                                                               0x02
#define NODE_REMOVE_V2                                                                   0x03
#define NODE_REMOVE_STATUS_V2                                                            0x04
#define FAILED_NODE_REPLACE_V2                                                           0x09
#define FAILED_NODE_REPLACE_STATUS_V2                                                    0x0A
#define NODE_NEIGHBOR_UPDATE_REQUEST_V2                                                  0x0B
#define NODE_NEIGHBOR_UPDATE_STATUS_V2                                                   0x0C
#define RETURN_ROUTE_ASSIGN_V2                                                           0x0D
#define RETURN_ROUTE_ASSIGN_COMPLETE_V2                                                  0x0E
#define RETURN_ROUTE_DELETE_V2                                                           0x0F
#define RETURN_ROUTE_DELETE_COMPLETE_V2                                                  0x10
#define NODE_ADD_KEYS_REPORT_V2                                                          0x11
#define NODE_ADD_KEYS_SET_V2                                                             0x12
#define NODE_ADD_DSK_REPORT_V2                                                           0x13
#define NODE_ADD_DSK_SET_V2                                                              0x14
#define SMART_START_JOIN_STARTED_REPORT  0x15 /* FIXME: Get official cmd no. assigned */
#define SMART_START_ADV_JOIN_MODE_SET    0x16 /* FIXME: Get official cmd no. assigned */
#define SMART_START_ADV_JOIN_MODE_GET    0x17 /* FIXME: Get official cmd no. assigned */
#define SMART_START_ADV_JOIN_MODE_REPORT 0x18 /* FIXME: Get official cmd no. assigned */
#define INCLUDED_NIF_REPORT              0x19 /* FIXME: Get official cmd no. assigned */

/* Values used for Node Add Status command */
#define NODE_ADD_STATUS_PROPERTIES1_CAPABILITY_MASK_V2                                   0x7F
#define NODE_ADD_STATUS_PROPERTIES1_LISTENING_BIT_MASK_V2                                0x80
#define NODE_ADD_STATUS_PROPERTIES2_SECURITY_MASK_V2                                     0x7F
#define NODE_ADD_STATUS_PROPERTIES2_OPT_BIT_MASK_V2                                      0x80
#define NODE_ADD_STATUS_SECURITY_MARK_V2                                                 0xF100
/* Values used for Node Add Keys Report command */
#define NODE_ADD_KEYS_REPORT_PROPERTIES1_REQUEST_CLIENT_SIDE_AUTHENTICATION_BIT_MASK_V2  0x01
#define NODE_ADD_KEYS_REPORT_PROPERTIES1_RESERVED_MASK_V2                                0xFE
#define NODE_ADD_KEYS_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                               0x01
/* Values used for Node Add Keys Set command */
#define NODE_ADD_KEYS_SET_PROPERTIES1_ACCEPT_BIT_MASK_V2                                 0x01
#define NODE_ADD_KEYS_SET_PROPERTIES1_GRANT_CLIENT_SIDE_AUTHENTICATION_BIT_MASK_V2       0x02
#define NODE_ADD_KEYS_SET_PROPERTIES1_RESERVED_MASK_V2                                   0xFC
#define NODE_ADD_KEYS_SET_PROPERTIES1_RESERVED_SHIFT_V2                                  0x02
/* Values used for Node Add Dsk Report command */
#define NODE_ADD_DSK_REPORT_PROPERTIES1_INPUT_DSK_LENGTH_MASK_V2                         0x0F
#define NODE_ADD_DSK_REPORT_PROPERTIES1_RESERVED_MASK_V2                                 0xF0
#define NODE_ADD_DSK_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                                0x04
/* Values used for Node Add Dsk Set command */
#define NODE_ADD_DSK_SET_PROPERTIES1_INPUT_DSK_LENGTH_MASK_V2                            0x0F
#define NODE_ADD_DSK_SET_PROPERTIES1_RESERVED_MASK_V2                                    0x70
#define NODE_ADD_DSK_SET_PROPERTIES1_RESERVED_SHIFT_V2                                   0x04
#define NODE_ADD_DSK_SET_PROPERTIES1_ACCEPT_BIT_MASK_V2                                  0x80

/* No Operation command class commands */
#define NO_OPERATION_VERSION                                                             0x01

/* Node Naming command class commands */
#define NODE_NAMING_VERSION                                                              0x01
#define NODE_NAMING_NODE_LOCATION_REPORT                                                 0x06
#define NODE_NAMING_NODE_LOCATION_SET                                                    0x04
#define NODE_NAMING_NODE_LOCATION_GET                                                    0x05
#define NODE_NAMING_NODE_NAME_GET                                                        0x02
#define NODE_NAMING_NODE_NAME_REPORT                                                     0x03
#define NODE_NAMING_NODE_NAME_SET                                                        0x01
/* Values used for Node Naming Node Location Report command */
#define NODE_NAMING_NODE_LOCATION_REPORT_LEVEL_CHAR_PRESENTATION_MASK                    0x07
#define NODE_NAMING_NODE_LOCATION_REPORT_LEVEL_RESERVED_MASK                             0xF8
#define NODE_NAMING_NODE_LOCATION_REPORT_LEVEL_RESERVED_SHIFT                            0x03
/* Values used for Node Naming Node Location Set command */
#define NODE_NAMING_NODE_LOCATION_SET_LEVEL_CHAR_PRESENTATION_MASK                       0x07
#define NODE_NAMING_NODE_LOCATION_SET_LEVEL_RESERVED_MASK                                0xF8
#define NODE_NAMING_NODE_LOCATION_SET_LEVEL_RESERVED_SHIFT                               0x03
/* Values used for Node Naming Node Name Report command */
#define NODE_NAMING_NODE_NAME_REPORT_LEVEL_CHAR_PRESENTATION_MASK                        0x07
#define NODE_NAMING_NODE_NAME_REPORT_LEVEL_RESERVED_MASK                                 0xF8
#define NODE_NAMING_NODE_NAME_REPORT_LEVEL_RESERVED_SHIFT                                0x03
/* Values used for Node Naming Node Name Set command */
#define NODE_NAMING_NODE_NAME_SET_LEVEL_CHAR_PRESENTATION_MASK                           0x07
#define NODE_NAMING_NODE_NAME_SET_LEVEL_RESERVED_MASK                                    0xF8
#define NODE_NAMING_NODE_NAME_SET_LEVEL_RESERVED_SHIFT                                   0x03

/* Non Interoperable command class commands */
#define NON_INTEROPERABLE_VERSION                                                        0x01

/* Powerlevel command class commands */
#define POWERLEVEL_VERSION                                                               0x01
#define POWERLEVEL_GET                                                                   0x02
#define POWERLEVEL_REPORT                                                                0x03
#define POWERLEVEL_SET                                                                   0x01
#define POWERLEVEL_TEST_NODE_GET                                                         0x05
#define POWERLEVEL_TEST_NODE_REPORT                                                      0x06
#define POWERLEVEL_TEST_NODE_SET                                                         0x04
/* Values used for Powerlevel Report command */
#define POWERLEVEL_REPORT_NORMALPOWER                                                    0x00
#define POWERLEVEL_REPORT_MINUS1DBM                                                      0x01
#define POWERLEVEL_REPORT_MINUS2DBM                                                      0x02
#define POWERLEVEL_REPORT_MINUS3DBM                                                      0x03
#define POWERLEVEL_REPORT_MINUS4DBM                                                      0x04
#define POWERLEVEL_REPORT_MINUS5DBM                                                      0x05
#define POWERLEVEL_REPORT_MINUS6DBM                                                      0x06
#define POWERLEVEL_REPORT_MINUS7DBM                                                      0x07
#define POWERLEVEL_REPORT_MINUS8DBM                                                      0x08
#define POWERLEVEL_REPORT_MINUS9DBM                                                      0x09
/* Values used for Powerlevel Set command */
#define POWERLEVEL_SET_NORMALPOWER                                                       0x00
#define POWERLEVEL_SET_MINUS1DBM                                                         0x01
#define POWERLEVEL_SET_MINUS2DBM                                                         0x02
#define POWERLEVEL_SET_MINUS3DBM                                                         0x03
#define POWERLEVEL_SET_MINUS4DBM                                                         0x04
#define POWERLEVEL_SET_MINUS5DBM                                                         0x05
#define POWERLEVEL_SET_MINUS6DBM                                                         0x06
#define POWERLEVEL_SET_MINUS7DBM                                                         0x07
#define POWERLEVEL_SET_MINUS8DBM                                                         0x08
#define POWERLEVEL_SET_MINUS9DBM                                                         0x09
/* Values used for Powerlevel Test Node Report command */
#define POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_NOT_A_NODEID                                 0x00
#define POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_FAILED                                       0x00
#define POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_SUCCES                                       0x01
#define POWERLEVEL_TEST_NODE_REPORT_ZW_TEST_INPROGRESS                                   0x02
/* Values used for Powerlevel Test Node Set command */
#define POWERLEVEL_TEST_NODE_SET_NORMALPOWER                                             0x00
#define POWERLEVEL_TEST_NODE_SET_MINUS1DBM                                               0x01
#define POWERLEVEL_TEST_NODE_SET_MINUS2DBM                                               0x02
#define POWERLEVEL_TEST_NODE_SET_MINUS3DBM                                               0x03
#define POWERLEVEL_TEST_NODE_SET_MINUS4DBM                                               0x04
#define POWERLEVEL_TEST_NODE_SET_MINUS5DBM                                               0x05
#define POWERLEVEL_TEST_NODE_SET_MINUS6DBM                                               0x06
#define POWERLEVEL_TEST_NODE_SET_MINUS7DBM                                               0x07
#define POWERLEVEL_TEST_NODE_SET_MINUS8DBM                                               0x08
#define POWERLEVEL_TEST_NODE_SET_MINUS9DBM                                               0x09

/* Prepayment Encapsulation command class commands */
#define PREPAYMENT_ENCAPSULATION_VERSION                                                 0x01
#define CMD_ENCAPSULATION                                                                0x01

/* Prepayment command class commands */
#define PREPAYMENT_VERSION                                                               0x01
#define PREPAYMENT_BALANCE_GET                                                           0x01
#define PREPAYMENT_BALANCE_REPORT                                                        0x02
#define PREPAYMENT_SUPPORTED_GET                                                         0x03
#define PREPAYMENT_SUPPORTED_REPORT                                                      0x04
/* Values used for Prepayment Balance Get command */
#define PREPAYMENT_BALANCE_GET_PROPERTIES1_RESERVED_MASK                                 0x3F
#define PREPAYMENT_BALANCE_GET_PROPERTIES1_BALANCE_TYPE_MASK                             0xC0
#define PREPAYMENT_BALANCE_GET_PROPERTIES1_BALANCE_TYPE_SHIFT                            0x06
#define PREPAYMENT_BALANCE_GET_BALANCE_TYPE_UTILITY                                      0x00
#define PREPAYMENT_BALANCE_GET_BALANCE_TYPE_MONETARY                                     0x01
/* Values used for Prepayment Balance Report command */
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES1_METER_TYPE_MASK                            0x3F
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES1_BALANCE_TYPE_MASK                          0xC0
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES1_BALANCE_TYPE_SHIFT                         0x06
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES2_SCALE_MASK                                 0x1F
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES2_BALANCE_PRECISION_MASK                     0xE0
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES2_BALANCE_PRECISION_SHIFT                    0x05
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES3_RESERVED1_MASK                             0x1F
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES3_DEBT_PRECISION_MASK                        0xE0
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES3_DEBT_PRECISION_SHIFT                       0x05
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES4_RESERVED2_MASK                             0x1F
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES4_EMER_CREDIT_PRECISION_MASK                 0xE0
#define PREPAYMENT_BALANCE_REPORT_PROPERTIES4_EMER_CREDIT_PRECISION_SHIFT                0x05
/* Values used for Prepayment Supported Report command */
#define PREPAYMENT_SUPPORTED_REPORT_PROPERTIES1_TYPES_SUPPORTED_MASK                     0x0F
#define PREPAYMENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK                            0xF0
#define PREPAYMENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT                           0x04

/* Proprietary command class commands */
#define PROPRIETARY_VERSION                                                              0x01
#define PROPRIETARY_GET                                                                  0x02
#define PROPRIETARY_REPORT                                                               0x03
#define PROPRIETARY_SET                                                                  0x01

/* Protection command class commands */
#define PROTECTION_VERSION                                                               0x01
#define PROTECTION_GET                                                                   0x02
#define PROTECTION_REPORT                                                                0x03
#define PROTECTION_SET                                                                   0x01
/* Values used for Protection Report command */
#define PROTECTION_REPORT_UNPROTECTED                                                    0x00
#define PROTECTION_REPORT_PROTECTION_BY_SEQUENCE                                         0x01
#define PROTECTION_REPORT_NO_OPERATION_POSSIBLE                                          0x02
/* Values used for Protection Set command */
#define PROTECTION_SET_UNPROTECTED                                                       0x00
#define PROTECTION_SET_PROTECTION_BY_SEQUENCE                                            0x01
#define PROTECTION_SET_NO_OPERATION_POSSIBLE                                             0x02

/* Protection command class commands */
#define PROTECTION_VERSION_V2                                                            0x02
#define PROTECTION_EC_GET_V2                                                             0x07
#define PROTECTION_EC_REPORT_V2                                                          0x08
#define PROTECTION_EC_SET_V2                                                             0x06
#define PROTECTION_GET_V2                                                                0x02
#define PROTECTION_REPORT_V2                                                             0x03
#define PROTECTION_SET_V2                                                                0x01
#define PROTECTION_SUPPORTED_GET_V2                                                      0x04
#define PROTECTION_SUPPORTED_REPORT_V2                                                   0x05
#define PROTECTION_TIMEOUT_GET_V2                                                        0x0A
#define PROTECTION_TIMEOUT_REPORT_V2                                                     0x0B
#define PROTECTION_TIMEOUT_SET_V2                                                        0x09
/* Values used for Protection Report command */
#define PROTECTION_REPORT_LEVEL_LOCAL_PROTECTION_STATE_MASK_V2                           0x0F
#define PROTECTION_REPORT_LEVEL_RESERVED1_MASK_V2                                        0xF0
#define PROTECTION_REPORT_LEVEL_RESERVED1_SHIFT_V2                                       0x04
#define PROTECTION_REPORT_LEVEL2_RF_PROTECTION_STATE_MASK_V2                             0x0F
#define PROTECTION_REPORT_LEVEL2_RESERVED2_MASK_V2                                       0xF0
#define PROTECTION_REPORT_LEVEL2_RESERVED2_SHIFT_V2                                      0x04
/* Values used for Protection Set command */
#define PROTECTION_SET_LEVEL_LOCAL_PROTECTION_STATE_MASK_V2                              0x0F
#define PROTECTION_SET_LEVEL_RESERVED1_MASK_V2                                           0xF0
#define PROTECTION_SET_LEVEL_RESERVED1_SHIFT_V2                                          0x04
#define PROTECTION_SET_LEVEL2_RF_PROTECTION_STATE_MASK_V2                                0x0F
#define PROTECTION_SET_LEVEL2_RESERVED2_MASK_V2                                          0xF0
#define PROTECTION_SET_LEVEL2_RESERVED2_SHIFT_V2                                         0x04
/* Values used for Protection Supported Report command */
#define PROTECTION_SUPPORTED_REPORT_LEVEL_TIMEOUT_BIT_MASK_V2                            0x01
#define PROTECTION_SUPPORTED_REPORT_LEVEL_EXCLUSIVE_CONTROL_BIT_MASK_V2                  0x02
#define PROTECTION_SUPPORTED_REPORT_LEVEL_RESERVED_MASK_V2                               0xFC
#define PROTECTION_SUPPORTED_REPORT_LEVEL_RESERVED_SHIFT_V2                              0x02

/* Rate Tbl Config command class commands */
#define RATE_TBL_CONFIG_VERSION                                                          0x01
#define RATE_TBL_REMOVE                                                                  0x02
#define RATE_TBL_SET                                                                     0x01
/* Values used for Rate Tbl Remove command */
#define RATE_TBL_REMOVE_PROPERTIES1_RATE_PARAMETER_SET_IDS_MASK                          0x3F
#define RATE_TBL_REMOVE_PROPERTIES1_RESERVED_MASK                                        0xC0
#define RATE_TBL_REMOVE_PROPERTIES1_RESERVED_SHIFT                                       0x06
/* Values used for Rate Tbl Set command */
#define RATE_TBL_SET_PROPERTIES1_NUMBER_OF_RATE_CHAR_MASK                                0x1F
#define RATE_TBL_SET_PROPERTIES1_RATE_TYPE_MASK                                          0x60
#define RATE_TBL_SET_PROPERTIES1_RATE_TYPE_SHIFT                                         0x05
#define RATE_TBL_SET_PROPERTIES1_RESERVED_BIT_MASK                                       0x80
#define RATE_TBL_SET_PROPERTIES2_CONSUMPTION_SCALE_MASK                                  0x1F
#define RATE_TBL_SET_PROPERTIES2_CONSUMPTION_PRECISION_MASK                              0xE0
#define RATE_TBL_SET_PROPERTIES2_CONSUMPTION_PRECISION_SHIFT                             0x05
#define RATE_TBL_SET_PROPERTIES3_MAX_DEMAND_SCALE_MASK                                   0x1F
#define RATE_TBL_SET_PROPERTIES3_MAX_DEMAND_PRECISION_MASK                               0xE0
#define RATE_TBL_SET_PROPERTIES3_MAX_DEMAND_PRECISION_SHIFT                              0x05

/* Rate Tbl Monitor command class commands */
#define RATE_TBL_MONITOR_VERSION                                                         0x01
#define RATE_TBL_ACTIVE_RATE_GET                                                         0x05
#define RATE_TBL_ACTIVE_RATE_REPORT                                                      0x06
#define RATE_TBL_CURRENT_DATA_GET                                                        0x07
#define RATE_TBL_CURRENT_DATA_REPORT                                                     0x08
#define RATE_TBL_GET                                                                     0x03
#define RATE_TBL_HISTORICAL_DATA_GET                                                     0x09
#define RATE_TBL_HISTORICAL_DATA_REPORT                                                  0x0A
#define RATE_TBL_REPORT                                                                  0x04
#define RATE_TBL_SUPPORTED_GET                                                           0x01
#define RATE_TBL_SUPPORTED_REPORT                                                        0x02
/* Values used for Rate Tbl Report command */
#define RATE_TBL_REPORT_PROPERTIES1_NUMBER_OF_RATE_CHAR_MASK                             0x1F
#define RATE_TBL_REPORT_PROPERTIES1_RATE_TYPE_MASK                                       0x60
#define RATE_TBL_REPORT_PROPERTIES1_RATE_TYPE_SHIFT                                      0x05
#define RATE_TBL_REPORT_PROPERTIES1_RESERVED_BIT_MASK                                    0x80
#define RATE_TBL_REPORT_PROPERTIES2_CONSUMPTION_SCALE_MASK                               0x1F
#define RATE_TBL_REPORT_PROPERTIES2_CONSUMPTION_PRECISION_MASK                           0xE0
#define RATE_TBL_REPORT_PROPERTIES2_CONSUMPTION_PRECISION_SHIFT                          0x05
#define RATE_TBL_REPORT_PROPERTIES3_MAX_DEMAND_SCALE_MASK                                0x1F
#define RATE_TBL_REPORT_PROPERTIES3_MAX_DEMAND_PRECISION_MASK                            0xE0
#define RATE_TBL_REPORT_PROPERTIES3_MAX_DEMAND_PRECISION_SHIFT                           0x05

/* Remote Association Activate command class commands */
#define REMOTE_ASSOCIATION_ACTIVATE_VERSION                                              0x01
#define REMOTE_ASSOCIATION_ACTIVATE                                                      0x01

/* Remote Association command class commands */
#define REMOTE_ASSOCIATION_VERSION                                                       0x01
#define REMOTE_ASSOCIATION_CONFIGURATION_GET                                             0x02
#define REMOTE_ASSOCIATION_CONFIGURATION_REPORT                                          0x03
#define REMOTE_ASSOCIATION_CONFIGURATION_SET                                             0x01

/* Scene Activation command class commands */
#define SCENE_ACTIVATION_VERSION                                                         0x01
#define SCENE_ACTIVATION_SET                                                             0x01

/* Scene Actuator Conf command class commands */
#define SCENE_ACTUATOR_CONF_VERSION                                                      0x01
#define SCENE_ACTUATOR_CONF_GET                                                          0x02
#define SCENE_ACTUATOR_CONF_REPORT                                                       0x03
#define SCENE_ACTUATOR_CONF_SET                                                          0x01
/* Values used for Scene Actuator Conf Set command */
#define SCENE_ACTUATOR_CONF_SET_LEVEL2_RESERVED_MASK                                     0x7F
#define SCENE_ACTUATOR_CONF_SET_LEVEL2_OVERRIDE_BIT_MASK                                 0x80

/* Scene Controller Conf command class commands */
#define SCENE_CONTROLLER_CONF_VERSION                                                    0x01
#define SCENE_CONTROLLER_CONF_GET                                                        0x02
#define SCENE_CONTROLLER_CONF_REPORT                                                     0x03
#define SCENE_CONTROLLER_CONF_SET                                                        0x01

/* Schedule Entry Lock command class commands */
#define SCHEDULE_ENTRY_LOCK_VERSION                                                      0x01
#define SCHEDULE_ENTRY_LOCK_ENABLE_ALL_SET                                               0x02
#define SCHEDULE_ENTRY_LOCK_ENABLE_SET                                                   0x01
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_GET                                                 0x04
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_REPORT                                              0x05
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_SET                                                 0x03
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_GET                                                 0x07
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_REPORT                                              0x08
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_SET                                                 0x06
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_GET                                                0x09
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_REPORT                                             0x0A

/* Schedule Entry Lock command class commands */
#define SCHEDULE_ENTRY_LOCK_VERSION_V2                                                   0x02
#define SCHEDULE_ENTRY_LOCK_ENABLE_ALL_SET_V2                                            0x02
#define SCHEDULE_ENTRY_LOCK_ENABLE_SET_V2                                                0x01
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_GET_V2                                           0x0B
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_V2                                        0x0C
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_V2                                           0x0D
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_GET_V2                                              0x04
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_REPORT_V2                                           0x05
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_SET_V2                                              0x03
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_GET_V2                                              0x07
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_REPORT_V2                                           0x08
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_SET_V2                                              0x06
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_GET_V2                                             0x09
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_REPORT_V2                                          0x0A
/* Values used for Schedule Entry Lock Time Offset Report command */
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL_HOUR_TZO_MASK_V2                    0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL_SIGN_TZO_BIT_MASK_V2                0x80
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL2_MINUTE_OFFSET_DST_MASK_V2          0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V2        0x80
/* Values used for Schedule Entry Lock Time Offset Set command */
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL_HOUR_TZO_MASK_V2                       0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL_SIGN_TZO_BIT_MASK_V2                   0x80
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL2_MINUTE_OFFSET_DST_MASK_V2             0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V2           0x80

/* Schedule Entry Lock command class commands */
#define SCHEDULE_ENTRY_LOCK_VERSION_V3                                                   0x03
#define SCHEDULE_ENTRY_LOCK_ENABLE_ALL_SET_V3                                            0x02
#define SCHEDULE_ENTRY_LOCK_ENABLE_SET_V3                                                0x01
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_GET_V3                                           0x0B
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_V3                                        0x0C
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_V3                                           0x0D
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_GET_V3                                              0x04
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_REPORT_V3                                           0x05
#define SCHEDULE_ENTRY_LOCK_WEEK_DAY_SET_V3                                              0x03
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_GET_V3                                              0x07
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_REPORT_V3                                           0x08
#define SCHEDULE_ENTRY_LOCK_YEAR_DAY_SET_V3                                              0x06
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_GET_V3                                             0x09
#define SCHEDULE_ENTRY_TYPE_SUPPORTED_REPORT_V3                                          0x0A
#define SCHEDULE_ENTRY_LOCK_DAILY_REPEATING_GET_V3                                       0x0E
#define SCHEDULE_ENTRY_LOCK_DAILY_REPEATING_REPORT_V3                                    0x0F
#define SCHEDULE_ENTRY_LOCK_DAILY_REPEATING_SET_V3                                       0x10
/* Values used for Schedule Entry Lock Time Offset Report command */
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL_HOUR_TZO_MASK_V3                    0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL_SIGN_TZO_BIT_MASK_V3                0x80
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL2_MINUTE_OFFSET_DST_MASK_V3          0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_REPORT_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V3        0x80
/* Values used for Schedule Entry Lock Time Offset Set command */
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL_HOUR_TZO_MASK_V3                       0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL_SIGN_TZO_BIT_MASK_V3                   0x80
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL2_MINUTE_OFFSET_DST_MASK_V3             0x7F
#define SCHEDULE_ENTRY_LOCK_TIME_OFFSET_SET_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V3           0x80

/* Screen Attributes command class commands */
#define SCREEN_ATTRIBUTES_VERSION                                                        0x01
#define SCREEN_ATTRIBUTES_GET                                                            0x01
#define SCREEN_ATTRIBUTES_REPORT                                                         0x02
/* Values used for Screen Attributes Report command */
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_NUMBER_OF_LINES_MASK                        0x1F
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_RESERVED_MASK                               0xE0
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_RESERVED_SHIFT                              0x05

/* Screen Attributes command class commands */
#define SCREEN_ATTRIBUTES_VERSION_V2                                                     0x02
#define SCREEN_ATTRIBUTES_GET_V2                                                         0x01
#define SCREEN_ATTRIBUTES_REPORT_V2                                                      0x03
/* Values used for Screen Attributes Report command */
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_NUMBER_OF_LINES_MASK_V2                     0x1F
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_ESCAPE_SEQUENCE_BIT_MASK_V2                 0x20
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_RESERVED_MASK_V2                            0xC0
#define SCREEN_ATTRIBUTES_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                           0x06

/* Screen Md command class commands */
#define SCREEN_MD_VERSION                                                                0x01
#define SCREEN_MD_GET                                                                    0x01
#define SCREEN_MD_REPORT                                                                 0x02
/* Values used for Screen Md Report command */
#define SCREEN_MD_REPORT_PROPERTIES1_CHAR_PRESENTATION_MASK                              0x07
#define SCREEN_MD_REPORT_PROPERTIES1_SCREEN_SETTINGS_MASK                                0x38
#define SCREEN_MD_REPORT_PROPERTIES1_SCREEN_SETTINGS_SHIFT                               0x03
#define SCREEN_MD_REPORT_PROPERTIES1_RESERVED_BIT_MASK                                   0x40
#define SCREEN_MD_REPORT_PROPERTIES1_MORE_DATA_BIT_MASK                                  0x80

/* Screen Md command class commands */
#define SCREEN_MD_VERSION_V2                                                             0x02
#define SCREEN_MD_GET_V2                                                                 0x01
#define SCREEN_MD_REPORT_V2                                                              0x03
/* Values used for Screen Md Report command */
#define SCREEN_MD_REPORT_PROPERTIES1_CHAR_PRESENTATION_MASK_V2                           0x07
#define SCREEN_MD_REPORT_PROPERTIES1_SCREEN_SETTINGS_MASK_V2                             0x38
#define SCREEN_MD_REPORT_PROPERTIES1_SCREEN_SETTINGS_SHIFT_V2                            0x03
#define SCREEN_MD_REPORT_PROPERTIES1_RESERVED1_BIT_MASK_V2                               0x40
#define SCREEN_MD_REPORT_PROPERTIES1_MORE_DATA_BIT_MASK_V2                               0x80
#define SCREEN_MD_REPORT_PROPERTIES2_SCREEN_TIMEOUT_BIT_MASK_V2                          0x01
#define SCREEN_MD_REPORT_PROPERTIES2_RESERVED2_MASK_V2                                   0xFE
#define SCREEN_MD_REPORT_PROPERTIES2_RESERVED2_SHIFT_V2                                  0x01

/* Security Panel Mode command class commands */
#define SECURITY_PANEL_MODE_VERSION                                                      0x01
#define SECURITY_PANEL_MODE_GET                                                          0x03
#define SECURITY_PANEL_MODE_REPORT                                                       0x04
#define SECURITY_PANEL_MODE_SET                                                          0x05
#define SECURITY_PANEL_MODE_SUPPORTED_GET                                                0x01
#define SECURITY_PANEL_MODE_SUPPORTED_REPORT                                             0x02

/* Security Panel Zone Sensor command class commands */
#define SECURITY_PANEL_ZONE_SENSOR_VERSION                                               0x01
#define COMMAND_CLASS_SECURITY_PANEL_ZONE_SENSOR_INSTALLED_REPORT                        0x02
#define SECURITY_PANEL_ZONE_SENSOR_TYPE_GET                                              0x03
#define SECURITY_PANEL_ZONE_SENSOR_TYPE_REPORT                                           0x04
#define SECURITY_PANEL_ZONE_SENSOR_INSTALLED_GET                                         0x01
#define SECURITY_PANEL_ZONE_SENSOR_STATE_GET                                             0x05
#define SECURITY_PANEL_ZONE_SENSOR_STATE_REPORT                                          0x06

/* Security Panel Zone command class commands */
#define SECURITY_PANEL_ZONE_VERSION                                                      0x01
#define SECURITY_PANEL_ZONE_NUMBER_SUPPORTED_GET                                         0x01
#define SECURITY_PANEL_ZONE_STATE_GET                                                    0x05
#define SECURITY_PANEL_ZONE_STATE_REPORT                                                 0x06
#define SECURITY_PANEL_ZONE_SUPPORTED_REPORT                                             0x02
#define SECURITY_PANEL_ZONE_TYPE_GET                                                     0x03
#define SECURITY_PANEL_ZONE_TYPE_REPORT                                                  0x04
/* Values used for Security Panel Zone Supported Report command */
#define SECURITY_PANEL_ZONE_SUPPORTED_REPORT_PARAMETERS1_ZONES_SUPPORTED_MASK            0x7F
#define SECURITY_PANEL_ZONE_SUPPORTED_REPORT_PARAMETERS1_ZM_BIT_MASK                     0x80

/* Security command class commands */
#define SECURITY_VERSION                                                                 0x01
#define NETWORK_KEY_SET                                                                  0x06
#define NETWORK_KEY_VERIFY                                                               0x07
#define SECURITY_COMMANDS_SUPPORTED_GET                                                  0x02
#define SECURITY_COMMANDS_SUPPORTED_REPORT                                               0x03
#define SECURITY_MESSAGE_ENCAPSULATION                                                   0x81
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET                                         0xC1
#define SECURITY_NONCE_GET                                                               0x40
#define SECURITY_NONCE_REPORT                                                            0x80
#define SECURITY_SCHEME_GET                                                              0x04
#define SECURITY_SCHEME_INHERIT                                                          0x08
#define SECURITY_SCHEME_REPORT                                                           0x05
/* Values used for Security Commands Supported Report command */
#define SECURITY_COMMANDS_SUPPORTED_REPORT_COMMAND_CLASS_MARK                            0xEF /*The COMMAND_CLASS_MARK is used to indicate that all preceding command classes are supported and all following command classes are controlled.*/
/* Values used for Security Message Encapsulation command */
#define SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SEQUENCE_COUNTER_MASK                 0x0F
#define SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SEQUENCED_BIT_MASK                    0x10
#define SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_SECOND_FRAME_BIT_MASK                 0x20
#define SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_RESERVED_MASK                         0xC0
#define SECURITY_MESSAGE_ENCAPSULATION_PROPERTIES1_RESERVED_SHIFT                        0x06
/* Values used for Security Message Encapsulation Nonce Get command */
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET_PROPERTIES1_SEQUENCE_COUNTER_MASK       0x0F
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET_PROPERTIES1_SEQUENCED_BIT_MASK          0x10
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET_PROPERTIES1_SECOND_FRAME_BIT_MASK       0x20
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET_PROPERTIES1_RESERVED_MASK               0xC0
#define SECURITY_MESSAGE_ENCAPSULATION_NONCE_GET_PROPERTIES1_RESERVED_SHIFT              0x06

/* Sensor Alarm command class commands */
#define SENSOR_ALARM_VERSION                                                             0x01
#define SENSOR_ALARM_GET                                                                 0x01
#define SENSOR_ALARM_REPORT                                                              0x02
#define SENSOR_ALARM_SUPPORTED_GET                                                       0x03
#define SENSOR_ALARM_SUPPORTED_REPORT                                                    0x04

/* Sensor Binary command class commands */
#define SENSOR_BINARY_VERSION                                                            0x01
#define SENSOR_BINARY_GET                                                                0x02
#define SENSOR_BINARY_REPORT                                                             0x03
/* Values used for Sensor Binary Report command */
#define SENSOR_BINARY_REPORT_IDLE                                                        0x00
#define SENSOR_BINARY_REPORT_DETECTED_AN_EVENT                                           0xFF

/* Sensor Binary command class commands */
#define SENSOR_BINARY_VERSION_V2                                                         0x02
#define SENSOR_BINARY_GET_V2                                                             0x02
#define SENSOR_BINARY_REPORT_V2                                                          0x03
#define SENSOR_BINARY_SUPPORTED_GET_SENSOR_V2                                            0x01
#define SENSOR_BINARY_SUPPORTED_SENSOR_REPORT_V2                                         0x04
/* Values used for Sensor Binary Report command */
#define SENSOR_BINARY_REPORT_IDLE_V2                                                     0x00
#define SENSOR_BINARY_REPORT_DETECTED_AN_EVENT_V2                                        0xFF

/* Sensor Configuration command class commands */
#define SENSOR_CONFIGURATION_VERSION                                                     0x01
#define SENSOR_TRIGGER_LEVEL_GET                                                         0x02
#define SENSOR_TRIGGER_LEVEL_REPORT                                                      0x03
#define SENSOR_TRIGGER_LEVEL_SET                                                         0x01
/* Values used for Sensor Trigger Level Report command */
#define SENSOR_TRIGGER_LEVEL_REPORT_PROPERTIES1_SIZE_MASK                                0x07
#define SENSOR_TRIGGER_LEVEL_REPORT_PROPERTIES1_SCALE_MASK                               0x18
#define SENSOR_TRIGGER_LEVEL_REPORT_PROPERTIES1_SCALE_SHIFT                              0x03
#define SENSOR_TRIGGER_LEVEL_REPORT_PROPERTIES1_PRECISION_MASK                           0xE0
#define SENSOR_TRIGGER_LEVEL_REPORT_PROPERTIES1_PRECISION_SHIFT                          0x05
/* Values used for Sensor Trigger Level Set command */
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES1_RESERVED_MASK                               0x3F
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES1_CURRENT_BIT_MASK                            0x40
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES1_DEFAULT_BIT_MASK                            0x80
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES2_SIZE_MASK                                   0x07
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES2_SCALE_MASK                                  0x18
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES2_SCALE_SHIFT                                 0x03
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES2_PRECISION_MASK                              0xE0
#define SENSOR_TRIGGER_LEVEL_SET_PROPERTIES2_PRECISION_SHIFT                             0x05

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION                                                        0x01
#define SENSOR_MULTILEVEL_GET                                                            0x04
#define SENSOR_MULTILEVEL_REPORT                                                         0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1                                   0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1                         0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1                                     0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK                                         0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK                                        0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT                                       0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK                                    0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT                                   0x05

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V2                                                     0x02
#define SENSOR_MULTILEVEL_GET_V2                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V2                                                      0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V2                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V2                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V2                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V2                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V2                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V2                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V2                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V2                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V2                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V2                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V2                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V2                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V2                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V2                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V2                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V2                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V2                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V2                                0x05

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V3                                                     0x03
#define SENSOR_MULTILEVEL_GET_V3                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V3                                                      0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V3                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V3                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V3                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V3                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V3                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V3                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V3                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V3                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V3                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V3                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V3                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V3                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V3                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V3                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V3                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V3                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V3                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V3                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V3                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V3                                   0x14
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V3                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V3                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V3                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V3                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V3                                0x05

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V4                                                     0x04
#define SENSOR_MULTILEVEL_GET_V4                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V4                                                      0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V4                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V4                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V4                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V4                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V4                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V4                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V4                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V4                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V4                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V4                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V4                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V4                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V4                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V4                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V4                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V4                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V4                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V4                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V4                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V4                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V4                             0x15
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V4                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V4                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V4                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V4                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V4                                0x05

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V5                                                     0x05
#define SENSOR_MULTILEVEL_GET_V5                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V5                                                      0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SENSOR_V5                                        0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SENSOR_REPORT_V5                                     0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_V5                                         0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_V5                                      0x06
/* Values used for Sensor Multilevel Get command */
#define SENSOR_MULTILEVEL_GET_TEMPERATURE_VERSION_1_V5                                   0x01
#define SENSOR_MULTILEVEL_GET_GENERAL_PURPOSE_VALUE_VERSION_1_V5                         0x02
#define SENSOR_MULTILEVEL_GET_LUMINANCE_VERSION_1_V5                                     0x03
#define SENSOR_MULTILEVEL_GET_POWER_VERSION_2_V5                                         0x04
#define SENSOR_MULTILEVEL_GET_RELATIVE_HUMIDITY_VERSION_2_V5                             0x05
#define SENSOR_MULTILEVEL_GET_VELOCITY_VERSION_2_V5                                      0x06
#define SENSOR_MULTILEVEL_GET_DIRECTION_VERSION_2_V5                                     0x07
#define SENSOR_MULTILEVEL_GET_ATMOSPHERIC_PRESSURE_VERSION_2_V5                          0x08
#define SENSOR_MULTILEVEL_GET_BAROMETRIC_PRESSURE_VERSION_2_V5                           0x09
#define SENSOR_MULTILEVEL_GET_SOLAR_RADIATION_VERSION_2_V5                               0x0A
#define SENSOR_MULTILEVEL_GET_DEW_POINT_VERSION_2_V5                                     0x0B
#define SENSOR_MULTILEVEL_GET_RAIN_RATE_VERSION_2_V5                                     0x0C
#define SENSOR_MULTILEVEL_GET_TIDE_LEVEL_VERSION_2_V5                                    0x0D
#define SENSOR_MULTILEVEL_GET_WEIGHT_VERSION_3_V5                                        0x0E
#define SENSOR_MULTILEVEL_GET_VOLTAGE_VERSION_3_V5                                       0x0F
#define SENSOR_MULTILEVEL_GET_CURRENT_VERSION_3_V5                                       0x10
#define SENSOR_MULTILEVEL_GET_CO2_LEVEL_VERSION_3_V5                                     0x11
#define SENSOR_MULTILEVEL_GET_AIR_FLOW_VERSION_3_V5                                      0x12
#define SENSOR_MULTILEVEL_GET_TANK_CAPACITY_VERSION_3_V5                                 0x13
#define SENSOR_MULTILEVEL_GET_DISTANCE_VERSION_3_V5                                      0x14
#define SENSOR_MULTILEVEL_GET_ANGLE_POSITION_VERSION_4_V5                                0x15
#define SENSOR_MULTILEVEL_GET_ROTATION_V5_V5                                             0x16
#define SENSOR_MULTILEVEL_GET_WATER_TEMPERATURE_V5_V5                                    0x17
#define SENSOR_MULTILEVEL_GET_SOIL_TEMPERATURE_V5_V5                                     0x18
#define SENSOR_MULTILEVEL_GET_SEISMIC_INTENSITY_V5_V5                                    0x19
#define SENSOR_MULTILEVEL_GET_SEISMIC_MAGNITUDE_V5_V5                                    0x1A
#define SENSOR_MULTILEVEL_GET_ULTRAVIOLET_V5_V5                                          0x1B
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_RESISTIVITY_V5_V5                               0x1C
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_CONDUCTIVITY_V5_V5                              0x1D
#define SENSOR_MULTILEVEL_GET_LOUDNESS_V5_V5                                             0x1E
#define SENSOR_MULTILEVEL_GET_MOISTURE_V5_V5                                             0x1F
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED1_MASK_V5                              0x07
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_MASK_V5                                  0x18
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_SHIFT_V5                                 0x03
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_MASK_V5                              0xE0
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_SHIFT_V5                             0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V5                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V5                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V5                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V5                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V5                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V5                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V5                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V5                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V5                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V5                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V5                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V5                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V5                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V5                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V5                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V5                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V5                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V5                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V5                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V5                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V5                             0x15
#define SENSOR_MULTILEVEL_REPORT_ROTATION_V5_V5                                          0x16
#define SENSOR_MULTILEVEL_REPORT_WATER_TEMPERATURE_V5_V5                                 0x17
#define SENSOR_MULTILEVEL_REPORT_SOIL_TEMPERATURE_V5_V5                                  0x18
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_INTENSITY_V5_V5                                 0x19
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_MAGNITUDE_V5_V5                                 0x1A
#define SENSOR_MULTILEVEL_REPORT_ULTRAVIOLET_V5_V5                                       0x1B
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_RESISTIVITY_V5_V5                            0x1C
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V5                           0x1D
#define SENSOR_MULTILEVEL_REPORT_LOUDNESS_V5_V5                                          0x1E
#define SENSOR_MULTILEVEL_REPORT_MOISTURE_V5_V5                                          0x1F
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V5                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V5                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V5                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V5                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V5                                0x05
/* Values used for Sensor Multilevel Supported Get Scale command */
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TEMPERATURE_VERSION_1_V5                   0x01
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_GENERAL_PURPOSE_VALUE_VERSION_1_V5         0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LUMINANCE_VERSION_1_V5                     0x03
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_POWER_VERSION_2_V5                         0x04
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RELATIVE_HUMIDITY_VERSION_2_V5             0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VELOCITY_VERSION_2_V5                      0x06
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DIRECTION_VERSION_2_V5                     0x07
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ATMOSPHERIC_PRESSURE_VERSION_2_V5          0x08
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BAROMETRIC_PRESSURE_VERSION_2_V5           0x09
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOLAR_RADIATION_VERSION_2_V5               0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DEW_POINT_VERSION_2_V5                     0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RAIN_RATE_VERSION_2_V5                     0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIDE_LEVEL_VERSION_2_V5                    0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WEIGHT_VERSION_3_V5                        0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLTAGE_VERSION_3_V5                       0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CURRENT_VERSION_3_V5                       0x10
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CO2_LEVEL_VERSION_3_V5                     0x11
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_AIR_FLOW_VERSION_3_V5                      0x12
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TANK_CAPACITY_VERSION_3_V5                 0x13
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DISTANCE_VERSION_3_V5                      0x14
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ANGLE_POSITION_VERSION_4_V5                0x15
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ROTATION_V5_V5                             0x16
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_TEMPERATURE_V5_V5                    0x17
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_TEMPERATURE_V5_V5                     0x18
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_INTENSITY_V5_V5                    0x19
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_MAGNITUDE_V5_V5                    0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ULTRAVIOLET_V5_V5                          0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_RESISTIVITY_V5_V5               0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_CONDUCTIVITY_V5_V5              0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LOUDNESS_V5_V5                             0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MOISTURE_V5_V5                             0x1F
/* Values used for Sensor Multilevel Supported Scale Report command */
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TEMPERATURE_VERSION_1_V5                0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V5      0x02
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LUMINANCE_VERSION_1_V5                  0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_POWER_VERSION_2_V5                      0x04
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RELATIVE_HUMIDITY_VERSION_2_V5          0x05
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VELOCITY_VERSION_2_V5                   0x06
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DIRECTION_VERSION_2_V5                  0x07
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V5       0x08
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V5        0x09
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOLAR_RADIATION_VERSION_2_V5            0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DEW_POINT_VERSION_2_V5                  0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RAIN_RATE_VERSION_2_V5                  0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIDE_LEVEL_VERSION_2_V5                 0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WEIGHT_VERSION_3_V5                     0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLTAGE_VERSION_3_V5                    0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CURRENT_VERSION_3_V5                    0x10
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CO2_LEVEL_VERSION_3_V5                  0x11
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_AIR_FLOW_VERSION_3_V5                   0x12
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TANK_CAPACITY_VERSION_3_V5              0x13
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DISTANCE_VERSION_3_V5                   0x14
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ANGLE_POSITION_VERSION_4_V5             0x15
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ROTATION_V5_V5                          0x16
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_TEMPERATURE_V5_V5                 0x17
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_TEMPERATURE_V5_V5                  0x18
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_INTENSITY_V5_V5                 0x19
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_MAGNITUDE_V5_V5                 0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ULTRAVIOLET_V5_V5                       0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_RESISTIVITY_V5_V5            0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V5           0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LOUDNESS_V5_V5                          0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MOISTURE_V5_V5                          0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK_V5      0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_MASK_V5            0xF0
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_SHIFT_V5           0x04

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V6                                                     0x06
#define SENSOR_MULTILEVEL_GET_V6                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V6                                                      0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SENSOR_V6                                        0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SENSOR_REPORT_V6                                     0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_V6                                         0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_V6                                      0x06
/* Values used for Sensor Multilevel Get command */
#define SENSOR_MULTILEVEL_GET_TEMPERATURE_VERSION_1_V6                                   0x01
#define SENSOR_MULTILEVEL_GET_GENERAL_PURPOSE_VALUE_VERSION_1_V6                         0x02
#define SENSOR_MULTILEVEL_GET_LUMINANCE_VERSION_1_V6                                     0x03
#define SENSOR_MULTILEVEL_GET_POWER_VERSION_2_V6                                         0x04
#define SENSOR_MULTILEVEL_GET_RELATIVE_HUMIDITY_VERSION_2_V6                             0x05
#define SENSOR_MULTILEVEL_GET_VELOCITY_VERSION_2_V6                                      0x06
#define SENSOR_MULTILEVEL_GET_DIRECTION_VERSION_2_V6                                     0x07
#define SENSOR_MULTILEVEL_GET_ATMOSPHERIC_PRESSURE_VERSION_2_V6                          0x08
#define SENSOR_MULTILEVEL_GET_BAROMETRIC_PRESSURE_VERSION_2_V6                           0x09
#define SENSOR_MULTILEVEL_GET_SOLAR_RADIATION_VERSION_2_V6                               0x0A
#define SENSOR_MULTILEVEL_GET_DEW_POINT_VERSION_2_V6                                     0x0B
#define SENSOR_MULTILEVEL_GET_RAIN_RATE_VERSION_2_V6                                     0x0C
#define SENSOR_MULTILEVEL_GET_TIDE_LEVEL_VERSION_2_V6                                    0x0D
#define SENSOR_MULTILEVEL_GET_WEIGHT_VERSION_3_V6                                        0x0E
#define SENSOR_MULTILEVEL_GET_VOLTAGE_VERSION_3_V6                                       0x0F
#define SENSOR_MULTILEVEL_GET_CURRENT_VERSION_3_V6                                       0x10
#define SENSOR_MULTILEVEL_GET_CO2_LEVEL_VERSION_3_V6                                     0x11
#define SENSOR_MULTILEVEL_GET_AIR_FLOW_VERSION_3_V6                                      0x12
#define SENSOR_MULTILEVEL_GET_TANK_CAPACITY_VERSION_3_V6                                 0x13
#define SENSOR_MULTILEVEL_GET_DISTANCE_VERSION_3_V6                                      0x14
#define SENSOR_MULTILEVEL_GET_ANGLE_POSITION_VERSION_4_V6                                0x15
#define SENSOR_MULTILEVEL_GET_ROTATION_V5_V6                                             0x16
#define SENSOR_MULTILEVEL_GET_WATER_TEMPERATURE_V5_V6                                    0x17
#define SENSOR_MULTILEVEL_GET_SOIL_TEMPERATURE_V5_V6                                     0x18
#define SENSOR_MULTILEVEL_GET_SEISMIC_INTENSITY_V5_V6                                    0x19
#define SENSOR_MULTILEVEL_GET_SEISMIC_MAGNITUDE_V5_V6                                    0x1A
#define SENSOR_MULTILEVEL_GET_ULTRAVIOLET_V5_V6                                          0x1B
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_RESISTIVITY_V5_V6                               0x1C
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_CONDUCTIVITY_V5_V6                              0x1D
#define SENSOR_MULTILEVEL_GET_LOUDNESS_V5_V6                                             0x1E
#define SENSOR_MULTILEVEL_GET_MOISTURE_V5_V6                                             0x1F
#define SENSOR_MULTILEVEL_GET_FREQUENCY_V6_V6                                            0x20
#define SENSOR_MULTILEVEL_GET_TIME_V6_V6                                                 0x21
#define SENSOR_MULTILEVEL_GET_TARGET_TEMPERATURE_V6_V6                                   0x22
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED1_MASK_V6                              0x07
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_MASK_V6                                  0x18
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_SHIFT_V6                                 0x03
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_MASK_V6                              0xE0
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_SHIFT_V6                             0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V6                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V6                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V6                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V6                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V6                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V6                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V6                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V6                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V6                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V6                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V6                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V6                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V6                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V6                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V6                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V6                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V6                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V6                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V6                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V6                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V6                             0x15
#define SENSOR_MULTILEVEL_REPORT_ROTATION_V5_V6                                          0x16
#define SENSOR_MULTILEVEL_REPORT_WATER_TEMPERATURE_V5_V6                                 0x17
#define SENSOR_MULTILEVEL_REPORT_SOIL_TEMPERATURE_V5_V6                                  0x18
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_INTENSITY_V5_V6                                 0x19
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_MAGNITUDE_V5_V6                                 0x1A
#define SENSOR_MULTILEVEL_REPORT_ULTRAVIOLET_V5_V6                                       0x1B
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_RESISTIVITY_V5_V6                            0x1C
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V6                           0x1D
#define SENSOR_MULTILEVEL_REPORT_LOUDNESS_V5_V6                                          0x1E
#define SENSOR_MULTILEVEL_REPORT_MOISTURE_V5_V6                                          0x1F
#define SENSOR_MULTILEVEL_REPORT_FREQUENCY_V6_V6                                         0x20
#define SENSOR_MULTILEVEL_REPORT_TIME_V6_V6                                              0x21
#define SENSOR_MULTILEVEL_REPORT_TARGET_TEMPERATURE_V6_V6                                0x22
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V6                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V6                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V6                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V6                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V6                                0x05
/* Values used for Sensor Multilevel Supported Get Scale command */
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TEMPERATURE_VERSION_1_V6                   0x01
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_GENERAL_PURPOSE_VALUE_VERSION_1_V6         0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LUMINANCE_VERSION_1_V6                     0x03
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_POWER_VERSION_2_V6                         0x04
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RELATIVE_HUMIDITY_VERSION_2_V6             0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VELOCITY_VERSION_2_V6                      0x06
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DIRECTION_VERSION_2_V6                     0x07
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ATMOSPHERIC_PRESSURE_VERSION_2_V6          0x08
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BAROMETRIC_PRESSURE_VERSION_2_V6           0x09
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOLAR_RADIATION_VERSION_2_V6               0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DEW_POINT_VERSION_2_V6                     0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RAIN_RATE_VERSION_2_V6                     0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIDE_LEVEL_VERSION_2_V6                    0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WEIGHT_VERSION_3_V6                        0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLTAGE_VERSION_3_V6                       0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CURRENT_VERSION_3_V6                       0x10
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CO2_LEVEL_VERSION_3_V6                     0x11
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_AIR_FLOW_VERSION_3_V6                      0x12
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TANK_CAPACITY_VERSION_3_V6                 0x13
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DISTANCE_VERSION_3_V6                      0x14
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ANGLE_POSITION_VERSION_4_V6                0x15
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ROTATION_V5_V6                             0x16
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_TEMPERATURE_V5_V6                    0x17
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_TEMPERATURE_V5_V6                     0x18
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_INTENSITY_V5_V6                    0x19
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_MAGNITUDE_V5_V6                    0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ULTRAVIOLET_V5_V6                          0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_RESISTIVITY_V5_V6               0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_CONDUCTIVITY_V5_V6              0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LOUDNESS_V5_V6                             0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MOISTURE_V5_V6                             0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FREQUENCY_V6_V6                            0x20
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIME_V6_V6                                 0x21
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TARGET_TEMPERATURE_V6_V6                   0x22
/* Values used for Sensor Multilevel Supported Scale Report command */
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TEMPERATURE_VERSION_1_V6                0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V6      0x02
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LUMINANCE_VERSION_1_V6                  0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_POWER_VERSION_2_V6                      0x04
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RELATIVE_HUMIDITY_VERSION_2_V6          0x05
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VELOCITY_VERSION_2_V6                   0x06
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DIRECTION_VERSION_2_V6                  0x07
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V6       0x08
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V6        0x09
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOLAR_RADIATION_VERSION_2_V6            0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DEW_POINT_VERSION_2_V6                  0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RAIN_RATE_VERSION_2_V6                  0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIDE_LEVEL_VERSION_2_V6                 0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WEIGHT_VERSION_3_V6                     0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLTAGE_VERSION_3_V6                    0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CURRENT_VERSION_3_V6                    0x10
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CO2_LEVEL_VERSION_3_V6                  0x11
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_AIR_FLOW_VERSION_3_V6                   0x12
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TANK_CAPACITY_VERSION_3_V6              0x13
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DISTANCE_VERSION_3_V6                   0x14
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ANGLE_POSITION_VERSION_4_V6             0x15
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ROTATION_V5_V6                          0x16
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_TEMPERATURE_V5_V6                 0x17
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_TEMPERATURE_V5_V6                  0x18
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_INTENSITY_V5_V6                 0x19
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_MAGNITUDE_V5_V6                 0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ULTRAVIOLET_V5_V6                       0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_RESISTIVITY_V5_V6            0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V6           0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LOUDNESS_V5_V6                          0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MOISTURE_V5_V6                          0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FREQUENCY_V6_V6                         0x20
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIME_V6_V6                              0x21
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TARGET_TEMPERATURE_V6_V6                0x22
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK_V6      0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_MASK_V6            0xF0
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_SHIFT_V6           0x04

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V7                                                     0x07
#define SENSOR_MULTILEVEL_GET_V7                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V7                                                      0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SENSOR_V7                                        0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SENSOR_REPORT_V7                                     0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_V7                                         0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_V7                                      0x06
/* Values used for Sensor Multilevel Get command */
#define SENSOR_MULTILEVEL_GET_TEMPERATURE_VERSION_1_V7                                   0x01
#define SENSOR_MULTILEVEL_GET_GENERAL_PURPOSE_VALUE_VERSION_1_V7                         0x02
#define SENSOR_MULTILEVEL_GET_LUMINANCE_VERSION_1_V7                                     0x03
#define SENSOR_MULTILEVEL_GET_POWER_VERSION_2_V7                                         0x04
#define SENSOR_MULTILEVEL_GET_RELATIVE_HUMIDITY_VERSION_2_V7                             0x05
#define SENSOR_MULTILEVEL_GET_VELOCITY_VERSION_2_V7                                      0x06
#define SENSOR_MULTILEVEL_GET_DIRECTION_VERSION_2_V7                                     0x07
#define SENSOR_MULTILEVEL_GET_ATMOSPHERIC_PRESSURE_VERSION_2_V7                          0x08
#define SENSOR_MULTILEVEL_GET_BAROMETRIC_PRESSURE_VERSION_2_V7                           0x09
#define SENSOR_MULTILEVEL_GET_SOLAR_RADIATION_VERSION_2_V7                               0x0A
#define SENSOR_MULTILEVEL_GET_DEW_POINT_VERSION_2_V7                                     0x0B
#define SENSOR_MULTILEVEL_GET_RAIN_RATE_VERSION_2_V7                                     0x0C
#define SENSOR_MULTILEVEL_GET_TIDE_LEVEL_VERSION_2_V7                                    0x0D
#define SENSOR_MULTILEVEL_GET_WEIGHT_VERSION_3_V7                                        0x0E
#define SENSOR_MULTILEVEL_GET_VOLTAGE_VERSION_3_V7                                       0x0F
#define SENSOR_MULTILEVEL_GET_CURRENT_VERSION_3_V7                                       0x10
#define SENSOR_MULTILEVEL_GET_CO2_LEVEL_VERSION_3_V7                                     0x11
#define SENSOR_MULTILEVEL_GET_AIR_FLOW_VERSION_3_V7                                      0x12
#define SENSOR_MULTILEVEL_GET_TANK_CAPACITY_VERSION_3_V7                                 0x13
#define SENSOR_MULTILEVEL_GET_DISTANCE_VERSION_3_V7                                      0x14
#define SENSOR_MULTILEVEL_GET_ANGLE_POSITION_VERSION_4_V7                                0x15
#define SENSOR_MULTILEVEL_GET_ROTATION_V5_V7                                             0x16
#define SENSOR_MULTILEVEL_GET_WATER_TEMPERATURE_V5_V7                                    0x17
#define SENSOR_MULTILEVEL_GET_SOIL_TEMPERATURE_V5_V7                                     0x18
#define SENSOR_MULTILEVEL_GET_SEISMIC_INTENSITY_V5_V7                                    0x19
#define SENSOR_MULTILEVEL_GET_SEISMIC_MAGNITUDE_V5_V7                                    0x1A
#define SENSOR_MULTILEVEL_GET_ULTRAVIOLET_V5_V7                                          0x1B
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_RESISTIVITY_V5_V7                               0x1C
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_CONDUCTIVITY_V5_V7                              0x1D
#define SENSOR_MULTILEVEL_GET_LOUDNESS_V5_V7                                             0x1E
#define SENSOR_MULTILEVEL_GET_MOISTURE_V5_V7                                             0x1F
#define SENSOR_MULTILEVEL_GET_FREQUENCY_V6_V7                                            0x20
#define SENSOR_MULTILEVEL_GET_TIME_V6_V7                                                 0x21
#define SENSOR_MULTILEVEL_GET_TARGET_TEMPERATURE_V6_V7                                   0x22
#define SENSOR_MULTILEVEL_GET_PARTICULATE_MATTER_2_5_V7_V7                               0x23
#define SENSOR_MULTILEVEL_GET_FORMALDEHYDE_CH2O_LEVEL_V7_V7                              0x24
#define SENSOR_MULTILEVEL_GET_RADON_CONCENTRATION_V7_V7                                  0x25
#define SENSOR_MULTILEVEL_GET_METHANE_DENSITY_CH4_V7_V7                                  0x26
#define SENSOR_MULTILEVEL_GET_VOLATILE_ORGANIC_COMPOUND_V7_V7                            0x27
#define SENSOR_MULTILEVEL_GET_CARBON_MONOXIDE_CO_LEVEL_V7_V7                             0x28
#define SENSOR_MULTILEVEL_GET_SOIL_HUMIDITY_V7_V7                                        0x29
#define SENSOR_MULTILEVEL_GET_SOIL_REACTIVITY_V7_V7                                      0x2A
#define SENSOR_MULTILEVEL_GET_SOIL_SALINITY_V7_V7                                        0x2B
#define SENSOR_MULTILEVEL_GET_HEART_RATE_V7_V7                                           0x2C
#define SENSOR_MULTILEVEL_GET_BLOOD_PRESSURE_V7_V7                                       0x2D
#define SENSOR_MULTILEVEL_GET_MUSCLE_MASS_V7_V7                                          0x2E
#define SENSOR_MULTILEVEL_GET_FAT_MASS_V7_V7                                             0x2F
#define SENSOR_MULTILEVEL_GET_BONE_MASS_V7_V7                                            0x30
#define SENSOR_MULTILEVEL_GET_TOTAL_BODY_WATER_TBW_V7_V7                                 0x31
#define SENSOR_MULTILEVEL_GET_BASIC_METABOLIC_RATE_BMR_V7_V7                             0x32
#define SENSOR_MULTILEVEL_GET_BODY_MASS_INDEX_BMI_V7_V7                                  0x33
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED1_MASK_V7                              0x07
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_MASK_V7                                  0x18
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_SHIFT_V7                                 0x03
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_MASK_V7                              0xE0
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_SHIFT_V7                             0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V7                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V7                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V7                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V7                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V7                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V7                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V7                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V7                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V7                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V7                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V7                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V7                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V7                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V7                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V7                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V7                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V7                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V7                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V7                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V7                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V7                             0x15
#define SENSOR_MULTILEVEL_REPORT_ROTATION_V5_V7                                          0x16
#define SENSOR_MULTILEVEL_REPORT_WATER_TEMPERATURE_V5_V7                                 0x17
#define SENSOR_MULTILEVEL_REPORT_SOIL_TEMPERATURE_V5_V7                                  0x18
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_INTENSITY_V5_V7                                 0x19
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_MAGNITUDE_V5_V7                                 0x1A
#define SENSOR_MULTILEVEL_REPORT_ULTRAVIOLET_V5_V7                                       0x1B
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_RESISTIVITY_V5_V7                            0x1C
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V7                           0x1D
#define SENSOR_MULTILEVEL_REPORT_LOUDNESS_V5_V7                                          0x1E
#define SENSOR_MULTILEVEL_REPORT_MOISTURE_V5_V7                                          0x1F
#define SENSOR_MULTILEVEL_REPORT_FREQUENCY_V6_V7                                         0x20
#define SENSOR_MULTILEVEL_REPORT_TIME_V6_V7                                              0x21
#define SENSOR_MULTILEVEL_REPORT_TARGET_TEMPERATURE_V6_V7                                0x22
#define SENSOR_MULTILEVEL_REPORT_PARTICULATE_MATTER_2_5_V7_V7                            0x23
#define SENSOR_MULTILEVEL_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V7                           0x24
#define SENSOR_MULTILEVEL_REPORT_RADON_CONCENTRATION_V7_V7                               0x25
#define SENSOR_MULTILEVEL_REPORT_METHANE_DENSITY_CH4_V7_V7                               0x26
#define SENSOR_MULTILEVEL_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V7                         0x27
#define SENSOR_MULTILEVEL_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V7                          0x28
#define SENSOR_MULTILEVEL_REPORT_SOIL_HUMIDITY_V7_V7                                     0x29
#define SENSOR_MULTILEVEL_REPORT_SOIL_REACTIVITY_V7_V7                                   0x2A
#define SENSOR_MULTILEVEL_REPORT_SOIL_SALINITY_V7_V7                                     0x2B
#define SENSOR_MULTILEVEL_REPORT_HEART_RATE_V7_V7                                        0x2C
#define SENSOR_MULTILEVEL_REPORT_BLOOD_PRESSURE_V7_V7                                    0x2D
#define SENSOR_MULTILEVEL_REPORT_MUSCLE_MASS_V7_V7                                       0x2E
#define SENSOR_MULTILEVEL_REPORT_FAT_MASS_V7_V7                                          0x2F
#define SENSOR_MULTILEVEL_REPORT_BONE_MASS_V7_V7                                         0x30
#define SENSOR_MULTILEVEL_REPORT_TOTAL_BODY_WATER_TBW_V7_V7                              0x31
#define SENSOR_MULTILEVEL_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V7                          0x32
#define SENSOR_MULTILEVEL_REPORT_BODY_MASS_INDEX_BMI_V7_V7                               0x33
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V7                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V7                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V7                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V7                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V7                                0x05
/* Values used for Sensor Multilevel Supported Get Scale command */
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TEMPERATURE_VERSION_1_V7                   0x01
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_GENERAL_PURPOSE_VALUE_VERSION_1_V7         0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LUMINANCE_VERSION_1_V7                     0x03
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_POWER_VERSION_2_V7                         0x04
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RELATIVE_HUMIDITY_VERSION_2_V7             0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VELOCITY_VERSION_2_V7                      0x06
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DIRECTION_VERSION_2_V7                     0x07
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ATMOSPHERIC_PRESSURE_VERSION_2_V7          0x08
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BAROMETRIC_PRESSURE_VERSION_2_V7           0x09
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOLAR_RADIATION_VERSION_2_V7               0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DEW_POINT_VERSION_2_V7                     0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RAIN_RATE_VERSION_2_V7                     0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIDE_LEVEL_VERSION_2_V7                    0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WEIGHT_VERSION_3_V7                        0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLTAGE_VERSION_3_V7                       0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CURRENT_VERSION_3_V7                       0x10
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CO2_LEVEL_VERSION_3_V7                     0x11
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_AIR_FLOW_VERSION_3_V7                      0x12
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TANK_CAPACITY_VERSION_3_V7                 0x13
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DISTANCE_VERSION_3_V7                      0x14
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ANGLE_POSITION_VERSION_4_V7                0x15
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ROTATION_V5_V7                             0x16
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_TEMPERATURE_V5_V7                    0x17
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_TEMPERATURE_V5_V7                     0x18
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_INTENSITY_V5_V7                    0x19
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_MAGNITUDE_V5_V7                    0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ULTRAVIOLET_V5_V7                          0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_RESISTIVITY_V5_V7               0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_CONDUCTIVITY_V5_V7              0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LOUDNESS_V5_V7                             0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MOISTURE_V5_V7                             0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FREQUENCY_V6_V7                            0x20
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIME_V6_V7                                 0x21
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TARGET_TEMPERATURE_V6_V7                   0x22
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_PARTICULATE_MATTER_2_5_V7_V7               0x23
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FORMALDEHYDE_CH2O_LEVEL_V7_V7              0x24
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RADON_CONCENTRATION_V7_V7                  0x25
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_METHANE_DENSITY_CH4_V7_V7                  0x26
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLATILE_ORGANIC_COMPOUND_V7_V7            0x27
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CARBON_MONOXIDE_CO_LEVEL_V7_V7             0x28
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_HUMIDITY_V7_V7                        0x29
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_REACTIVITY_V7_V7                      0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_SALINITY_V7_V7                        0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_HEART_RATE_V7_V7                           0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BLOOD_PRESSURE_V7_V7                       0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MUSCLE_MASS_V7_V7                          0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FAT_MASS_V7_V7                             0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BONE_MASS_V7_V7                            0x30
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TOTAL_BODY_WATER_TBW_V7_V7                 0x31
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BASIC_METABOLIC_RATE_BMR_V7_V7             0x32
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BODY_MASS_INDEX_BMI_V7_V7                  0x33
/* Values used for Sensor Multilevel Supported Scale Report command */
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TEMPERATURE_VERSION_1_V7                0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V7      0x02
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LUMINANCE_VERSION_1_V7                  0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_POWER_VERSION_2_V7                      0x04
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RELATIVE_HUMIDITY_VERSION_2_V7          0x05
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VELOCITY_VERSION_2_V7                   0x06
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DIRECTION_VERSION_2_V7                  0x07
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V7       0x08
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V7        0x09
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOLAR_RADIATION_VERSION_2_V7            0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DEW_POINT_VERSION_2_V7                  0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RAIN_RATE_VERSION_2_V7                  0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIDE_LEVEL_VERSION_2_V7                 0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WEIGHT_VERSION_3_V7                     0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLTAGE_VERSION_3_V7                    0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CURRENT_VERSION_3_V7                    0x10
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CO2_LEVEL_VERSION_3_V7                  0x11
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_AIR_FLOW_VERSION_3_V7                   0x12
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TANK_CAPACITY_VERSION_3_V7              0x13
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DISTANCE_VERSION_3_V7                   0x14
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ANGLE_POSITION_VERSION_4_V7             0x15
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ROTATION_V5_V7                          0x16
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_TEMPERATURE_V5_V7                 0x17
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_TEMPERATURE_V5_V7                  0x18
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_INTENSITY_V5_V7                 0x19
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_MAGNITUDE_V5_V7                 0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ULTRAVIOLET_V5_V7                       0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_RESISTIVITY_V5_V7            0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V7           0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LOUDNESS_V5_V7                          0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MOISTURE_V5_V7                          0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FREQUENCY_V6_V7                         0x20
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIME_V6_V7                              0x21
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TARGET_TEMPERATURE_V6_V7                0x22
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PARTICULATE_MATTER_2_5_V7_V7            0x23
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V7           0x24
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RADON_CONCENTRATION_V7_V7               0x25
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_METHANE_DENSITY_CH4_V7_V7               0x26
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V7         0x27
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V7          0x28
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_HUMIDITY_V7_V7                     0x29
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_REACTIVITY_V7_V7                   0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_SALINITY_V7_V7                     0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_HEART_RATE_V7_V7                        0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BLOOD_PRESSURE_V7_V7                    0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MUSCLE_MASS_V7_V7                       0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FAT_MASS_V7_V7                          0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BONE_MASS_V7_V7                         0x30
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TOTAL_BODY_WATER_TBW_V7_V7              0x31
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V7          0x32
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BODY_MASS_INDEX_BMI_V7_V7               0x33
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK_V7      0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_MASK_V7            0xF0
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_SHIFT_V7           0x04

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V8                                                     0x08
#define SENSOR_MULTILEVEL_GET_V8                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V8                                                      0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SENSOR_V8                                        0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SENSOR_REPORT_V8                                     0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_V8                                         0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_V8                                      0x06
/* Values used for Sensor Multilevel Get command */
#define SENSOR_MULTILEVEL_GET_TEMPERATURE_VERSION_1_V8                                   0x01
#define SENSOR_MULTILEVEL_GET_GENERAL_PURPOSE_VALUE_VERSION_1_V8                         0x02
#define SENSOR_MULTILEVEL_GET_LUMINANCE_VERSION_1_V8                                     0x03
#define SENSOR_MULTILEVEL_GET_POWER_VERSION_2_V8                                         0x04
#define SENSOR_MULTILEVEL_GET_RELATIVE_HUMIDITY_VERSION_2_V8                             0x05
#define SENSOR_MULTILEVEL_GET_VELOCITY_VERSION_2_V8                                      0x06
#define SENSOR_MULTILEVEL_GET_DIRECTION_VERSION_2_V8                                     0x07
#define SENSOR_MULTILEVEL_GET_ATMOSPHERIC_PRESSURE_VERSION_2_V8                          0x08
#define SENSOR_MULTILEVEL_GET_BAROMETRIC_PRESSURE_VERSION_2_V8                           0x09
#define SENSOR_MULTILEVEL_GET_SOLAR_RADIATION_VERSION_2_V8                               0x0A
#define SENSOR_MULTILEVEL_GET_DEW_POINT_VERSION_2_V8                                     0x0B
#define SENSOR_MULTILEVEL_GET_RAIN_RATE_VERSION_2_V8                                     0x0C
#define SENSOR_MULTILEVEL_GET_TIDE_LEVEL_VERSION_2_V8                                    0x0D
#define SENSOR_MULTILEVEL_GET_WEIGHT_VERSION_3_V8                                        0x0E
#define SENSOR_MULTILEVEL_GET_VOLTAGE_VERSION_3_V8                                       0x0F
#define SENSOR_MULTILEVEL_GET_CURRENT_VERSION_3_V8                                       0x10
#define SENSOR_MULTILEVEL_GET_CO2_LEVEL_VERSION_3_V8                                     0x11
#define SENSOR_MULTILEVEL_GET_AIR_FLOW_VERSION_3_V8                                      0x12
#define SENSOR_MULTILEVEL_GET_TANK_CAPACITY_VERSION_3_V8                                 0x13
#define SENSOR_MULTILEVEL_GET_DISTANCE_VERSION_3_V8                                      0x14
#define SENSOR_MULTILEVEL_GET_ANGLE_POSITION_VERSION_4_V8                                0x15
#define SENSOR_MULTILEVEL_GET_ROTATION_V5_V8                                             0x16
#define SENSOR_MULTILEVEL_GET_WATER_TEMPERATURE_V5_V8                                    0x17
#define SENSOR_MULTILEVEL_GET_SOIL_TEMPERATURE_V5_V8                                     0x18
#define SENSOR_MULTILEVEL_GET_SEISMIC_INTENSITY_V5_V8                                    0x19
#define SENSOR_MULTILEVEL_GET_SEISMIC_MAGNITUDE_V5_V8                                    0x1A
#define SENSOR_MULTILEVEL_GET_ULTRAVIOLET_V5_V8                                          0x1B
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_RESISTIVITY_V5_V8                               0x1C
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_CONDUCTIVITY_V5_V8                              0x1D
#define SENSOR_MULTILEVEL_GET_LOUDNESS_V5_V8                                             0x1E
#define SENSOR_MULTILEVEL_GET_MOISTURE_V5_V8                                             0x1F
#define SENSOR_MULTILEVEL_GET_FREQUENCY_V6_V8                                            0x20
#define SENSOR_MULTILEVEL_GET_TIME_V6_V8                                                 0x21
#define SENSOR_MULTILEVEL_GET_TARGET_TEMPERATURE_V6_V8                                   0x22
#define SENSOR_MULTILEVEL_GET_PARTICULATE_MATTER_2_5_V7_V8                               0x23
#define SENSOR_MULTILEVEL_GET_FORMALDEHYDE_CH2O_LEVEL_V7_V8                              0x24
#define SENSOR_MULTILEVEL_GET_RADON_CONCENTRATION_V7_V8                                  0x25
#define SENSOR_MULTILEVEL_GET_METHANE_DENSITY_CH4_V7_V8                                  0x26
#define SENSOR_MULTILEVEL_GET_VOLATILE_ORGANIC_COMPOUND_V7_V8                            0x27
#define SENSOR_MULTILEVEL_GET_CARBON_MONOXIDE_CO_LEVEL_V7_V8                             0x28
#define SENSOR_MULTILEVEL_GET_SOIL_HUMIDITY_V7_V8                                        0x29
#define SENSOR_MULTILEVEL_GET_SOIL_REACTIVITY_V7_V8                                      0x2A
#define SENSOR_MULTILEVEL_GET_SOIL_SALINITY_V7_V8                                        0x2B
#define SENSOR_MULTILEVEL_GET_HEART_RATE_V7_V8                                           0x2C
#define SENSOR_MULTILEVEL_GET_BLOOD_PRESSURE_V7_V8                                       0x2D
#define SENSOR_MULTILEVEL_GET_MUSCLE_MASS_V7_V8                                          0x2E
#define SENSOR_MULTILEVEL_GET_FAT_MASS_V7_V8                                             0x2F
#define SENSOR_MULTILEVEL_GET_BONE_MASS_V7_V8                                            0x30
#define SENSOR_MULTILEVEL_GET_TOTAL_BODY_WATER_TBW_V7_V8                                 0x31
#define SENSOR_MULTILEVEL_GET_BASIC_METABOLIC_RATE_BMR_V7_V8                             0x32
#define SENSOR_MULTILEVEL_GET_BODY_MASS_INDEX_BMI_V7_V8                                  0x33
#define SENSOR_MULTILEVEL_GET_ACCELERATION_X_AXIS_V8_V8                                  0x34
#define SENSOR_MULTILEVEL_GET_ACCELERATION_Y_AXIS_V8_V8                                  0x35
#define SENSOR_MULTILEVEL_GET_ACCELERATION_Z_AXIS_V8_V8                                  0x36
#define SENSOR_MULTILEVEL_GET_SMOKE_DENSITY_V8_V8                                        0x37
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED1_MASK_V8                              0x07
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_MASK_V8                                  0x18
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_SHIFT_V8                                 0x03
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_MASK_V8                              0xE0
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_SHIFT_V8                             0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V8                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V8                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V8                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V8                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V8                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V8                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V8                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V8                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V8                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V8                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V8                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V8                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V8                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V8                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V8                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V8                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V8                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V8                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V8                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V8                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V8                             0x15
#define SENSOR_MULTILEVEL_REPORT_ROTATION_V5_V8                                          0x16
#define SENSOR_MULTILEVEL_REPORT_WATER_TEMPERATURE_V5_V8                                 0x17
#define SENSOR_MULTILEVEL_REPORT_SOIL_TEMPERATURE_V5_V8                                  0x18
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_INTENSITY_V5_V8                                 0x19
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_MAGNITUDE_V5_V8                                 0x1A
#define SENSOR_MULTILEVEL_REPORT_ULTRAVIOLET_V5_V8                                       0x1B
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_RESISTIVITY_V5_V8                            0x1C
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V8                           0x1D
#define SENSOR_MULTILEVEL_REPORT_LOUDNESS_V5_V8                                          0x1E
#define SENSOR_MULTILEVEL_REPORT_MOISTURE_V5_V8                                          0x1F
#define SENSOR_MULTILEVEL_REPORT_FREQUENCY_V6_V8                                         0x20
#define SENSOR_MULTILEVEL_REPORT_TIME_V6_V8                                              0x21
#define SENSOR_MULTILEVEL_REPORT_TARGET_TEMPERATURE_V6_V8                                0x22
#define SENSOR_MULTILEVEL_REPORT_PARTICULATE_MATTER_2_5_V7_V8                            0x23
#define SENSOR_MULTILEVEL_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V8                           0x24
#define SENSOR_MULTILEVEL_REPORT_RADON_CONCENTRATION_V7_V8                               0x25
#define SENSOR_MULTILEVEL_REPORT_METHANE_DENSITY_CH4_V7_V8                               0x26
#define SENSOR_MULTILEVEL_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V8                         0x27
#define SENSOR_MULTILEVEL_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V8                          0x28
#define SENSOR_MULTILEVEL_REPORT_SOIL_HUMIDITY_V7_V8                                     0x29
#define SENSOR_MULTILEVEL_REPORT_SOIL_REACTIVITY_V7_V8                                   0x2A
#define SENSOR_MULTILEVEL_REPORT_SOIL_SALINITY_V7_V8                                     0x2B
#define SENSOR_MULTILEVEL_REPORT_HEART_RATE_V7_V8                                        0x2C
#define SENSOR_MULTILEVEL_REPORT_BLOOD_PRESSURE_V7_V8                                    0x2D
#define SENSOR_MULTILEVEL_REPORT_MUSCLE_MASS_V7_V8                                       0x2E
#define SENSOR_MULTILEVEL_REPORT_FAT_MASS_V7_V8                                          0x2F
#define SENSOR_MULTILEVEL_REPORT_BONE_MASS_V7_V8                                         0x30
#define SENSOR_MULTILEVEL_REPORT_TOTAL_BODY_WATER_TBW_V7_V8                              0x31
#define SENSOR_MULTILEVEL_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V8                          0x32
#define SENSOR_MULTILEVEL_REPORT_BODY_MASS_INDEX_BMI_V7_V8                               0x33
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_X_AXIS_V8_V8                               0x34
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_Y_AXIS_V8_V8                               0x35
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_Z_AXIS_V8_V8                               0x36
#define SENSOR_MULTILEVEL_REPORT_SMOKE_DENSITY_V8_V8                                     0x37
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V8                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V8                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V8                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V8                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V8                                0x05
/* Values used for Sensor Multilevel Supported Get Scale command */
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TEMPERATURE_VERSION_1_V8                   0x01
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_GENERAL_PURPOSE_VALUE_VERSION_1_V8         0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LUMINANCE_VERSION_1_V8                     0x03
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_POWER_VERSION_2_V8                         0x04
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RELATIVE_HUMIDITY_VERSION_2_V8             0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VELOCITY_VERSION_2_V8                      0x06
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DIRECTION_VERSION_2_V8                     0x07
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ATMOSPHERIC_PRESSURE_VERSION_2_V8          0x08
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BAROMETRIC_PRESSURE_VERSION_2_V8           0x09
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOLAR_RADIATION_VERSION_2_V8               0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DEW_POINT_VERSION_2_V8                     0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RAIN_RATE_VERSION_2_V8                     0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIDE_LEVEL_VERSION_2_V8                    0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WEIGHT_VERSION_3_V8                        0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLTAGE_VERSION_3_V8                       0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CURRENT_VERSION_3_V8                       0x10
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CO2_LEVEL_VERSION_3_V8                     0x11
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_AIR_FLOW_VERSION_3_V8                      0x12
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TANK_CAPACITY_VERSION_3_V8                 0x13
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DISTANCE_VERSION_3_V8                      0x14
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ANGLE_POSITION_VERSION_4_V8                0x15
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ROTATION_V5_V8                             0x16
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_TEMPERATURE_V5_V8                    0x17
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_TEMPERATURE_V5_V8                     0x18
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_INTENSITY_V5_V8                    0x19
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_MAGNITUDE_V5_V8                    0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ULTRAVIOLET_V5_V8                          0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_RESISTIVITY_V5_V8               0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_CONDUCTIVITY_V5_V8              0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LOUDNESS_V5_V8                             0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MOISTURE_V5_V8                             0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FREQUENCY_V6_V8                            0x20
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIME_V6_V8                                 0x21
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TARGET_TEMPERATURE_V6_V8                   0x22
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_PARTICULATE_MATTER_2_5_V7_V8               0x23
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FORMALDEHYDE_CH2O_LEVEL_V7_V8              0x24
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RADON_CONCENTRATION_V7_V8                  0x25
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_METHANE_DENSITY_CH4_V7_V8                  0x26
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLATILE_ORGANIC_COMPOUND_V7_V8            0x27
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CARBON_MONOXIDE_CO_LEVEL_V7_V8             0x28
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_HUMIDITY_V7_V8                        0x29
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_REACTIVITY_V7_V8                      0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_SALINITY_V7_V8                        0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_HEART_RATE_V7_V8                           0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BLOOD_PRESSURE_V7_V8                       0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MUSCLE_MASS_V7_V8                          0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FAT_MASS_V7_V8                             0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BONE_MASS_V7_V8                            0x30
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TOTAL_BODY_WATER_TBW_V7_V8                 0x31
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BASIC_METABOLIC_RATE_BMR_V7_V8             0x32
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BODY_MASS_INDEX_BMI_V7_V8                  0x33
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_X_AXIS_V8_V8                  0x34
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_Y_AXIS_V8_V8                  0x35
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_Z_AXIS_V8_V8                  0x36
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SMOKE_DENSITY_V8_V8                        0x37
/* Values used for Sensor Multilevel Supported Scale Report command */
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TEMPERATURE_VERSION_1_V8                0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V8      0x02
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LUMINANCE_VERSION_1_V8                  0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_POWER_VERSION_2_V8                      0x04
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RELATIVE_HUMIDITY_VERSION_2_V8          0x05
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VELOCITY_VERSION_2_V8                   0x06
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DIRECTION_VERSION_2_V8                  0x07
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V8       0x08
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V8        0x09
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOLAR_RADIATION_VERSION_2_V8            0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DEW_POINT_VERSION_2_V8                  0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RAIN_RATE_VERSION_2_V8                  0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIDE_LEVEL_VERSION_2_V8                 0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WEIGHT_VERSION_3_V8                     0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLTAGE_VERSION_3_V8                    0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CURRENT_VERSION_3_V8                    0x10
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CO2_LEVEL_VERSION_3_V8                  0x11
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_AIR_FLOW_VERSION_3_V8                   0x12
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TANK_CAPACITY_VERSION_3_V8              0x13
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DISTANCE_VERSION_3_V8                   0x14
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ANGLE_POSITION_VERSION_4_V8             0x15
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ROTATION_V5_V8                          0x16
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_TEMPERATURE_V5_V8                 0x17
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_TEMPERATURE_V5_V8                  0x18
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_INTENSITY_V5_V8                 0x19
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_MAGNITUDE_V5_V8                 0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ULTRAVIOLET_V5_V8                       0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_RESISTIVITY_V5_V8            0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V8           0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LOUDNESS_V5_V8                          0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MOISTURE_V5_V8                          0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FREQUENCY_V6_V8                         0x20
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIME_V6_V8                              0x21
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TARGET_TEMPERATURE_V6_V8                0x22
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PARTICULATE_MATTER_2_5_V7_V8            0x23
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V8           0x24
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RADON_CONCENTRATION_V7_V8               0x25
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_METHANE_DENSITY_CH4_V7_V8               0x26
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V8         0x27
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V8          0x28
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_HUMIDITY_V7_V8                     0x29
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_REACTIVITY_V7_V8                   0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_SALINITY_V7_V8                     0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_HEART_RATE_V7_V8                        0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BLOOD_PRESSURE_V7_V8                    0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MUSCLE_MASS_V7_V8                       0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FAT_MASS_V7_V8                          0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BONE_MASS_V7_V8                         0x30
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TOTAL_BODY_WATER_TBW_V7_V8              0x31
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V8          0x32
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BODY_MASS_INDEX_BMI_V7_V8               0x33
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_X_AXIS_V8_V8               0x34
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_Y_AXIS_V8_V8               0x35
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_Z_AXIS_V8_V8               0x36
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SMOKE_DENSITY_V8_V8                     0x37
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK_V8      0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_MASK_V8            0xF0
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_SHIFT_V8           0x04

/* Sensor Multilevel command class commands */
#define SENSOR_MULTILEVEL_VERSION_V9                                                     0x09
#define SENSOR_MULTILEVEL_GET_V9                                                         0x04
#define SENSOR_MULTILEVEL_REPORT_V9                                                      0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SENSOR_V9                                        0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SENSOR_REPORT_V9                                     0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_V9                                         0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_V9                                      0x06
/* Values used for Sensor Multilevel Get command */
#define SENSOR_MULTILEVEL_GET_TEMPERATURE_VERSION_1_V9                                   0x01
#define SENSOR_MULTILEVEL_GET_GENERAL_PURPOSE_VALUE_VERSION_1_V9                         0x02
#define SENSOR_MULTILEVEL_GET_LUMINANCE_VERSION_1_V9                                     0x03
#define SENSOR_MULTILEVEL_GET_POWER_VERSION_2_V9                                         0x04
#define SENSOR_MULTILEVEL_GET_RELATIVE_HUMIDITY_VERSION_2_V9                             0x05
#define SENSOR_MULTILEVEL_GET_VELOCITY_VERSION_2_V9                                      0x06
#define SENSOR_MULTILEVEL_GET_DIRECTION_VERSION_2_V9                                     0x07
#define SENSOR_MULTILEVEL_GET_ATMOSPHERIC_PRESSURE_VERSION_2_V9                          0x08
#define SENSOR_MULTILEVEL_GET_BAROMETRIC_PRESSURE_VERSION_2_V9                           0x09
#define SENSOR_MULTILEVEL_GET_SOLAR_RADIATION_VERSION_2_V9                               0x0A
#define SENSOR_MULTILEVEL_GET_DEW_POINT_VERSION_2_V9                                     0x0B
#define SENSOR_MULTILEVEL_GET_RAIN_RATE_VERSION_2_V9                                     0x0C
#define SENSOR_MULTILEVEL_GET_TIDE_LEVEL_VERSION_2_V9                                    0x0D
#define SENSOR_MULTILEVEL_GET_WEIGHT_VERSION_3_V9                                        0x0E
#define SENSOR_MULTILEVEL_GET_VOLTAGE_VERSION_3_V9                                       0x0F
#define SENSOR_MULTILEVEL_GET_CURRENT_VERSION_3_V9                                       0x10
#define SENSOR_MULTILEVEL_GET_CO2_LEVEL_VERSION_3_V9                                     0x11
#define SENSOR_MULTILEVEL_GET_AIR_FLOW_VERSION_3_V9                                      0x12
#define SENSOR_MULTILEVEL_GET_TANK_CAPACITY_VERSION_3_V9                                 0x13
#define SENSOR_MULTILEVEL_GET_DISTANCE_VERSION_3_V9                                      0x14
#define SENSOR_MULTILEVEL_GET_ANGLE_POSITION_VERSION_4_V9                                0x15
#define SENSOR_MULTILEVEL_GET_ROTATION_V5_V9                                             0x16
#define SENSOR_MULTILEVEL_GET_WATER_TEMPERATURE_V5_V9                                    0x17
#define SENSOR_MULTILEVEL_GET_SOIL_TEMPERATURE_V5_V9                                     0x18
#define SENSOR_MULTILEVEL_GET_SEISMIC_INTENSITY_V5_V9                                    0x19
#define SENSOR_MULTILEVEL_GET_SEISMIC_MAGNITUDE_V5_V9                                    0x1A
#define SENSOR_MULTILEVEL_GET_ULTRAVIOLET_V5_V9                                          0x1B
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_RESISTIVITY_V5_V9                               0x1C
#define SENSOR_MULTILEVEL_GET_ELECTRICAL_CONDUCTIVITY_V5_V9                              0x1D
#define SENSOR_MULTILEVEL_GET_LOUDNESS_V5_V9                                             0x1E
#define SENSOR_MULTILEVEL_GET_MOISTURE_V5_V9                                             0x1F
#define SENSOR_MULTILEVEL_GET_FREQUENCY_V6_V9                                            0x20
#define SENSOR_MULTILEVEL_GET_TIME_V6_V9                                                 0x21
#define SENSOR_MULTILEVEL_GET_TARGET_TEMPERATURE_V6_V9                                   0x22
#define SENSOR_MULTILEVEL_GET_PARTICULATE_MATTER_2_5_V7_V9                               0x23
#define SENSOR_MULTILEVEL_GET_FORMALDEHYDE_CH2O_LEVEL_V7_V9                              0x24
#define SENSOR_MULTILEVEL_GET_RADON_CONCENTRATION_V7_V9                                  0x25
#define SENSOR_MULTILEVEL_GET_METHANE_DENSITY_CH4_V7_V9                                  0x26
#define SENSOR_MULTILEVEL_GET_VOLATILE_ORGANIC_COMPOUND_V7_V9                            0x27
#define SENSOR_MULTILEVEL_GET_CARBON_MONOXIDE_CO_LEVEL_V7_V9                             0x28
#define SENSOR_MULTILEVEL_GET_SOIL_HUMIDITY_V7_V9                                        0x29
#define SENSOR_MULTILEVEL_GET_SOIL_REACTIVITY_V7_V9                                      0x2A
#define SENSOR_MULTILEVEL_GET_SOIL_SALINITY_V7_V9                                        0x2B
#define SENSOR_MULTILEVEL_GET_HEART_RATE_V7_V9                                           0x2C
#define SENSOR_MULTILEVEL_GET_BLOOD_PRESSURE_V7_V9                                       0x2D
#define SENSOR_MULTILEVEL_GET_MUSCLE_MASS_V7_V9                                          0x2E
#define SENSOR_MULTILEVEL_GET_FAT_MASS_V7_V9                                             0x2F
#define SENSOR_MULTILEVEL_GET_BONE_MASS_V7_V9                                            0x30
#define SENSOR_MULTILEVEL_GET_TOTAL_BODY_WATER_TBW_V7_V9                                 0x31
#define SENSOR_MULTILEVEL_GET_BASIC_METABOLIC_RATE_BMR_V7_V9                             0x32
#define SENSOR_MULTILEVEL_GET_BODY_MASS_INDEX_BMI_V7_V9                                  0x33
#define SENSOR_MULTILEVEL_GET_ACCELERATION_X_AXIS_V8_V9                                  0x34
#define SENSOR_MULTILEVEL_GET_ACCELERATION_Y_AXIS_V8_V9                                  0x35
#define SENSOR_MULTILEVEL_GET_ACCELERATION_Z_AXIS_V8_V9                                  0x36
#define SENSOR_MULTILEVEL_GET_SMOKE_DENSITY_V8_V9                                        0x37
#define SENSOR_MULTILEVEL_GET_WATER_FLOW_V9_V9                                           0x38
#define SENSOR_MULTILEVEL_GET_WATER_PRESSURE_V9_V9                                       0x39
#define SENSOR_MULTILEVEL_GET_RF_SIGNAL_STRENGTH_V9_V9                                   0x3A
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED1_MASK_V9                              0x07
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_MASK_V9                                  0x18
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_SCALE_SHIFT_V9                                 0x03
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_MASK_V9                              0xE0
#define SENSOR_MULTILEVEL_GET_PROPERTIES1_RESERVED2_SHIFT_V9                             0x05
/* Values used for Sensor Multilevel Report command */
#define SENSOR_MULTILEVEL_REPORT_TEMPERATURE_VERSION_1_V9                                0x01
#define SENSOR_MULTILEVEL_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V9                      0x02
#define SENSOR_MULTILEVEL_REPORT_LUMINANCE_VERSION_1_V9                                  0x03
#define SENSOR_MULTILEVEL_REPORT_POWER_VERSION_2_V9                                      0x04
#define SENSOR_MULTILEVEL_REPORT_RELATIVE_HUMIDITY_VERSION_2_V9                          0x05
#define SENSOR_MULTILEVEL_REPORT_VELOCITY_VERSION_2_V9                                   0x06
#define SENSOR_MULTILEVEL_REPORT_DIRECTION_VERSION_2_V9                                  0x07
#define SENSOR_MULTILEVEL_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V9                       0x08
#define SENSOR_MULTILEVEL_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V9                        0x09
#define SENSOR_MULTILEVEL_REPORT_SOLAR_RADIATION_VERSION_2_V9                            0x0A
#define SENSOR_MULTILEVEL_REPORT_DEW_POINT_VERSION_2_V9                                  0x0B
#define SENSOR_MULTILEVEL_REPORT_RAIN_RATE_VERSION_2_V9                                  0x0C
#define SENSOR_MULTILEVEL_REPORT_TIDE_LEVEL_VERSION_2_V9                                 0x0D
#define SENSOR_MULTILEVEL_REPORT_WEIGHT_VERSION_3_V9                                     0x0E
#define SENSOR_MULTILEVEL_REPORT_VOLTAGE_VERSION_3_V9                                    0x0F
#define SENSOR_MULTILEVEL_REPORT_CURRENT_VERSION_3_V9                                    0x10
#define SENSOR_MULTILEVEL_REPORT_CO2_LEVEL_VERSION_3_V9                                  0x11
#define SENSOR_MULTILEVEL_REPORT_AIR_FLOW_VERSION_3_V9                                   0x12
#define SENSOR_MULTILEVEL_REPORT_TANK_CAPACITY_VERSION_3_V9                              0x13
#define SENSOR_MULTILEVEL_REPORT_DISTANCE_VERSION_3_V9                                   0x14
#define SENSOR_MULTILEVEL_REPORT_ANGLE_POSITION_VERSION_4_V9                             0x15
#define SENSOR_MULTILEVEL_REPORT_ROTATION_V5_V9                                          0x16
#define SENSOR_MULTILEVEL_REPORT_WATER_TEMPERATURE_V5_V9                                 0x17
#define SENSOR_MULTILEVEL_REPORT_SOIL_TEMPERATURE_V5_V9                                  0x18
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_INTENSITY_V5_V9                                 0x19
#define SENSOR_MULTILEVEL_REPORT_SEISMIC_MAGNITUDE_V5_V9                                 0x1A
#define SENSOR_MULTILEVEL_REPORT_ULTRAVIOLET_V5_V9                                       0x1B
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_RESISTIVITY_V5_V9                            0x1C
#define SENSOR_MULTILEVEL_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V9                           0x1D
#define SENSOR_MULTILEVEL_REPORT_LOUDNESS_V5_V9                                          0x1E
#define SENSOR_MULTILEVEL_REPORT_MOISTURE_V5_V9                                          0x1F
#define SENSOR_MULTILEVEL_REPORT_FREQUENCY_V6_V9                                         0x20
#define SENSOR_MULTILEVEL_REPORT_TIME_V6_V9                                              0x21
#define SENSOR_MULTILEVEL_REPORT_TARGET_TEMPERATURE_V6_V9                                0x22
#define SENSOR_MULTILEVEL_REPORT_PARTICULATE_MATTER_2_5_V7_V9                            0x23
#define SENSOR_MULTILEVEL_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V9                           0x24
#define SENSOR_MULTILEVEL_REPORT_RADON_CONCENTRATION_V7_V9                               0x25
#define SENSOR_MULTILEVEL_REPORT_METHANE_DENSITY_CH4_V7_V9                               0x26
#define SENSOR_MULTILEVEL_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V9                         0x27
#define SENSOR_MULTILEVEL_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V9                          0x28
#define SENSOR_MULTILEVEL_REPORT_SOIL_HUMIDITY_V7_V9                                     0x29
#define SENSOR_MULTILEVEL_REPORT_SOIL_REACTIVITY_V7_V9                                   0x2A
#define SENSOR_MULTILEVEL_REPORT_SOIL_SALINITY_V7_V9                                     0x2B
#define SENSOR_MULTILEVEL_REPORT_HEART_RATE_V7_V9                                        0x2C
#define SENSOR_MULTILEVEL_REPORT_BLOOD_PRESSURE_V7_V9                                    0x2D
#define SENSOR_MULTILEVEL_REPORT_MUSCLE_MASS_V7_V9                                       0x2E
#define SENSOR_MULTILEVEL_REPORT_FAT_MASS_V7_V9                                          0x2F
#define SENSOR_MULTILEVEL_REPORT_BONE_MASS_V7_V9                                         0x30
#define SENSOR_MULTILEVEL_REPORT_TOTAL_BODY_WATER_TBW_V7_V9                              0x31
#define SENSOR_MULTILEVEL_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V9                          0x32
#define SENSOR_MULTILEVEL_REPORT_BODY_MASS_INDEX_BMI_V7_V9                               0x33
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_X_AXIS_V8_V9                               0x34
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_Y_AXIS_V8_V9                               0x35
#define SENSOR_MULTILEVEL_REPORT_ACCELERATION_Z_AXIS_V8_V9                               0x36
#define SENSOR_MULTILEVEL_REPORT_SMOKE_DENSITY_V8_V9                                     0x37
#define SENSOR_MULTILEVEL_REPORT_WATER_FLOW_V9_V9                                        0x38
#define SENSOR_MULTILEVEL_REPORT_WATER_PRESSURE_V9_V9                                    0x39
#define SENSOR_MULTILEVEL_REPORT_RF_SIGNAL_STRENGTH_V9_V9                                0x3A
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SIZE_MASK_V9                                      0x07
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_MASK_V9                                     0x18
#define SENSOR_MULTILEVEL_REPORT_LEVEL_SCALE_SHIFT_V9                                    0x03
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_MASK_V9                                 0xE0
#define SENSOR_MULTILEVEL_REPORT_LEVEL_PRECISION_SHIFT_V9                                0x05
/* Values used for Sensor Multilevel Supported Get Scale command */
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TEMPERATURE_VERSION_1_V9                   0x01
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_GENERAL_PURPOSE_VALUE_VERSION_1_V9         0x02
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LUMINANCE_VERSION_1_V9                     0x03
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_POWER_VERSION_2_V9                         0x04
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RELATIVE_HUMIDITY_VERSION_2_V9             0x05
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VELOCITY_VERSION_2_V9                      0x06
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DIRECTION_VERSION_2_V9                     0x07
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ATMOSPHERIC_PRESSURE_VERSION_2_V9          0x08
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BAROMETRIC_PRESSURE_VERSION_2_V9           0x09
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOLAR_RADIATION_VERSION_2_V9               0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DEW_POINT_VERSION_2_V9                     0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RAIN_RATE_VERSION_2_V9                     0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIDE_LEVEL_VERSION_2_V9                    0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WEIGHT_VERSION_3_V9                        0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLTAGE_VERSION_3_V9                       0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CURRENT_VERSION_3_V9                       0x10
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CO2_LEVEL_VERSION_3_V9                     0x11
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_AIR_FLOW_VERSION_3_V9                      0x12
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TANK_CAPACITY_VERSION_3_V9                 0x13
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_DISTANCE_VERSION_3_V9                      0x14
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ANGLE_POSITION_VERSION_4_V9                0x15
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ROTATION_V5_V9                             0x16
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_TEMPERATURE_V5_V9                    0x17
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_TEMPERATURE_V5_V9                     0x18
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_INTENSITY_V5_V9                    0x19
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SEISMIC_MAGNITUDE_V5_V9                    0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ULTRAVIOLET_V5_V9                          0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_RESISTIVITY_V5_V9               0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ELECTRICAL_CONDUCTIVITY_V5_V9              0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_LOUDNESS_V5_V9                             0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MOISTURE_V5_V9                             0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FREQUENCY_V6_V9                            0x20
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TIME_V6_V9                                 0x21
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TARGET_TEMPERATURE_V6_V9                   0x22
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_PARTICULATE_MATTER_2_5_V7_V9               0x23
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FORMALDEHYDE_CH2O_LEVEL_V7_V9              0x24
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RADON_CONCENTRATION_V7_V9                  0x25
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_METHANE_DENSITY_CH4_V7_V9                  0x26
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_VOLATILE_ORGANIC_COMPOUND_V7_V9            0x27
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_CARBON_MONOXIDE_CO_LEVEL_V7_V9             0x28
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_HUMIDITY_V7_V9                        0x29
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_REACTIVITY_V7_V9                      0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SOIL_SALINITY_V7_V9                        0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_HEART_RATE_V7_V9                           0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BLOOD_PRESSURE_V7_V9                       0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_MUSCLE_MASS_V7_V9                          0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_FAT_MASS_V7_V9                             0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BONE_MASS_V7_V9                            0x30
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_TOTAL_BODY_WATER_TBW_V7_V9                 0x31
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BASIC_METABOLIC_RATE_BMR_V7_V9             0x32
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_BODY_MASS_INDEX_BMI_V7_V9                  0x33
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_X_AXIS_V8_V9                  0x34
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_Y_AXIS_V8_V9                  0x35
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_ACCELERATION_Z_AXIS_V8_V9                  0x36
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_SMOKE_DENSITY_V8_V9                        0x37
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_FLOW_V9_V9                           0x38
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_WATER_PRESSURE_V9_V9                       0x39
#define SENSOR_MULTILEVEL_SUPPORTED_GET_SCALE_RF_SIGNAL_STRENGTH_V9_V9                   0x3A
/* Values used for Sensor Multilevel Supported Scale Report command */
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TEMPERATURE_VERSION_1_V9                0x01
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_GENERAL_PURPOSE_VALUE_VERSION_1_V9      0x02
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LUMINANCE_VERSION_1_V9                  0x03
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_POWER_VERSION_2_V9                      0x04
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RELATIVE_HUMIDITY_VERSION_2_V9          0x05
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VELOCITY_VERSION_2_V9                   0x06
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DIRECTION_VERSION_2_V9                  0x07
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ATMOSPHERIC_PRESSURE_VERSION_2_V9       0x08
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BAROMETRIC_PRESSURE_VERSION_2_V9        0x09
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOLAR_RADIATION_VERSION_2_V9            0x0A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DEW_POINT_VERSION_2_V9                  0x0B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RAIN_RATE_VERSION_2_V9                  0x0C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIDE_LEVEL_VERSION_2_V9                 0x0D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WEIGHT_VERSION_3_V9                     0x0E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLTAGE_VERSION_3_V9                    0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CURRENT_VERSION_3_V9                    0x10
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CO2_LEVEL_VERSION_3_V9                  0x11
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_AIR_FLOW_VERSION_3_V9                   0x12
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TANK_CAPACITY_VERSION_3_V9              0x13
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_DISTANCE_VERSION_3_V9                   0x14
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ANGLE_POSITION_VERSION_4_V9             0x15
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ROTATION_V5_V9                          0x16
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_TEMPERATURE_V5_V9                 0x17
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_TEMPERATURE_V5_V9                  0x18
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_INTENSITY_V5_V9                 0x19
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SEISMIC_MAGNITUDE_V5_V9                 0x1A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ULTRAVIOLET_V5_V9                       0x1B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_RESISTIVITY_V5_V9            0x1C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ELECTRICAL_CONDUCTIVITY_V5_V9           0x1D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_LOUDNESS_V5_V9                          0x1E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MOISTURE_V5_V9                          0x1F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FREQUENCY_V6_V9                         0x20
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TIME_V6_V9                              0x21
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TARGET_TEMPERATURE_V6_V9                0x22
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PARTICULATE_MATTER_2_5_V7_V9            0x23
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FORMALDEHYDE_CH2O_LEVEL_V7_V9           0x24
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RADON_CONCENTRATION_V7_V9               0x25
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_METHANE_DENSITY_CH4_V7_V9               0x26
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_VOLATILE_ORGANIC_COMPOUND_V7_V9         0x27
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_CARBON_MONOXIDE_CO_LEVEL_V7_V9          0x28
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_HUMIDITY_V7_V9                     0x29
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_REACTIVITY_V7_V9                   0x2A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SOIL_SALINITY_V7_V9                     0x2B
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_HEART_RATE_V7_V9                        0x2C
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BLOOD_PRESSURE_V7_V9                    0x2D
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_MUSCLE_MASS_V7_V9                       0x2E
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_FAT_MASS_V7_V9                          0x2F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BONE_MASS_V7_V9                         0x30
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_TOTAL_BODY_WATER_TBW_V7_V9              0x31
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BASIC_METABOLIC_RATE_BMR_V7_V9          0x32
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_BODY_MASS_INDEX_BMI_V7_V9               0x33
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_X_AXIS_V8_V9               0x34
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_Y_AXIS_V8_V9               0x35
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_ACCELERATION_Z_AXIS_V8_V9               0x36
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_SMOKE_DENSITY_V8_V9                     0x37
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_FLOW_V9_V9                        0x38
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_WATER_PRESSURE_V9_V9                    0x39
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_RF_SIGNAL_STRENGTH_V9_V9                0x3A
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK_V9      0x0F
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_MASK_V9            0xF0
#define SENSOR_MULTILEVEL_SUPPORTED_SCALE_REPORT_PROPERTIES1_RESERVED_SHIFT_V9           0x04

/* Silence Alarm command class commands */
#define SILENCE_ALARM_VERSION                                                            0x01
#define SENSOR_ALARM_SET                                                                 0x01

/* Simple Av Control command class commands */
#define SIMPLE_AV_CONTROL_VERSION                                                        0x01
#define SIMPLE_AV_CONTROL_GET                                                            0x02
#define SIMPLE_AV_CONTROL_REPORT                                                         0x03
#define SIMPLE_AV_CONTROL_SET                                                            0x01
#define SIMPLE_AV_CONTROL_SUPPORTED_GET                                                  0x04
#define SIMPLE_AV_CONTROL_SUPPORTED_REPORT                                               0x05
/* Values used for Simple Av Control Set command */
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_KEY_ATTRIBUTES_MASK                            0x07
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_RESERVED_MASK                                  0xF8
#define SIMPLE_AV_CONTROL_SET_PROPERTIES1_RESERVED_SHIFT                                 0x03

/* Switch All command class commands */
#define SWITCH_ALL_VERSION                                                               0x01
#define SWITCH_ALL_GET                                                                   0x02
#define SWITCH_ALL_OFF                                                                   0x05
#define SWITCH_ALL_ON                                                                    0x04
#define SWITCH_ALL_REPORT                                                                0x03
#define SWITCH_ALL_SET                                                                   0x01
/* Values used for Switch All Report command */
#define SWITCH_ALL_REPORT_EXCLUDED_FROM_THE_ALL_ON_ALL_OFF_FUNCTIONALITY                 0x00
#define SWITCH_ALL_REPORT_EXCLUDED_FROM_THE_ALL_ON_FUNCTIONALITY_BUT_NOT_ALL_OFF         0x01
#define SWITCH_ALL_REPORT_EXCLUDED_FROM_THE_ALL_OFF_FUNCTIONALITY_BUT_NOT_ALL_ON         0x02
#define SWITCH_ALL_REPORT_INCLUDED_IN_THE_ALL_ON_ALL_OFF_FUNCTIONALITY                   0xFF
/* Values used for Switch All Set command */
#define SWITCH_ALL_SET_EXCLUDED_FROM_THE_ALL_ON_ALL_OFF_FUNCTIONALITY                    0x00
#define SWITCH_ALL_SET_EXCLUDED_FROM_THE_ALL_ON_FUNCTIONALITY_BUT_NOT_ALL_OFF            0x01
#define SWITCH_ALL_SET_EXCLUDED_FROM_THE_ALL_OFF_FUNCTIONALITY_BUT_NOT_ALL_ON            0x02
#define SWITCH_ALL_SET_INCLUDED_IN_THE_ALL_ON_ALL_OFF_FUNCTIONALITY                      0xFF

/* Switch Binary command class commands */
#define SWITCH_BINARY_VERSION                                                            0x01
#define SWITCH_BINARY_GET                                                                0x02
#define SWITCH_BINARY_REPORT                                                             0x03
#define SWITCH_BINARY_SET                                                                0x01

/* Switch Binary command class commands */
#define SWITCH_BINARY_VERSION_V2                                                         0x02
#define SWITCH_BINARY_GET_V2                                                             0x02
#define SWITCH_BINARY_REPORT_V2                                                          0x03
#define SWITCH_BINARY_SET_V2                                                             0x01
/* Values used for Switch Binary Report command */
#define SWITCH_BINARY_REPORT_OFF_DISABLE_V2                                              0x00
#define SWITCH_BINARY_REPORT_ON_ENABLE_V2                                                0xFF
#define SWITCH_BINARY_REPORT_OFF_DISABLE_V2                                              0x00
#define SWITCH_BINARY_REPORT_ON_ENABLE_V2                                                0xFF
#define SWITCH_BINARY_REPORT_ALREADY_AT_THE_TARGET_VALUE_V2                              0x00
#define SWITCH_BINARY_REPORT_UNKNOWN_DURATION_V2                                         0xFE
#define SWITCH_BINARY_REPORT_RESERVED_V2                                                 0xFF
/* Values used for Switch Binary Set command */
#define SWITCH_BINARY_SET_OFF_DISABLE_V2                                                 0x00
#define SWITCH_BINARY_SET_ON_ENABLE_V2                                                   0xFF
#define SWITCH_BINARY_SET_INSTANTLY_V2                                                   0x00
#define SWITCH_BINARY_SET_DEFAULT_V2                                                     0xFF

/* Switch Multilevel command class commands */
#define SWITCH_MULTILEVEL_VERSION                                                        0x01
#define SWITCH_MULTILEVEL_GET                                                            0x02
#define SWITCH_MULTILEVEL_REPORT                                                         0x03
#define SWITCH_MULTILEVEL_SET                                                            0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE                                             0x04
#define SWITCH_MULTILEVEL_STOP_LEVEL_CHANGE                                              0x05
/* Values used for Switch Multilevel Start Level Change command */
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_RESERVED1_MASK                        0x1F
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_IGNORE_START_LEVEL_BIT_MASK           0x20
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_UP_DOWN_BIT_MASK                      0x40
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_RESERVED2_BIT_MASK                    0x80

/* Switch Multilevel command class commands */
#define SWITCH_MULTILEVEL_VERSION_V2                                                     0x02
#define SWITCH_MULTILEVEL_GET_V2                                                         0x02
#define SWITCH_MULTILEVEL_REPORT_V2                                                      0x03
#define SWITCH_MULTILEVEL_SET_V2                                                         0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_V2                                          0x04
#define SWITCH_MULTILEVEL_STOP_LEVEL_CHANGE_V2                                           0x05
/* Values used for Switch Multilevel Start Level Change command */
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_RESERVED1_MASK_V2               0x1F
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_LEVEL_BIT_MASK_V2  0x20
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_BIT_MASK_V2             0x40
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_RESERVED2_BIT_MASK_V2           0x80

/* Switch Multilevel command class commands */
#define SWITCH_MULTILEVEL_VERSION_V3                                                     0x03
#define SWITCH_MULTILEVEL_GET_V3                                                         0x02
#define SWITCH_MULTILEVEL_REPORT_V3                                                      0x03
#define SWITCH_MULTILEVEL_SET_V3                                                         0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_V3                                          0x04
#define SWITCH_MULTILEVEL_STOP_LEVEL_CHANGE_V3                                           0x05
#define SWITCH_MULTILEVEL_SUPPORTED_GET_V3                                               0x06
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_V3                                            0x07
/* Values used for Switch Multilevel Start Level Change command */
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_RESERVED_MASK_V3                0x07
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_INC_DEC_MASK_V3                 0x18
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_INC_DEC_SHIFT_V3                0x03
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_INCREMENT_V3                        0x00
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_DECREMENT_V3                        0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_RESERVED_V3                         0x02
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_NONE_V3                             0x03
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_LEVEL_BIT_MASK_V3  0x20
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_MASK_V3                 0xC0
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_SHIFT_V3                0x06
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_UP_V3                               0x00
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_DOWN_V3                             0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_RESERVED_V3                         0x02
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_NONE_V3                             0x03
/* Values used for Switch Multilevel Supported Report command */
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_PRIMARY_SWITCH_TYPE_MASK_V3       0x1F
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_MASK_V3                 0xE0
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_SHIFT_V3                0x05
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_SECONDARY_SWITCH_TYPE_MASK_V3     0x1F
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_MASK_V3                 0xE0
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_SHIFT_V3                0x05

/* Switch Multilevel command class commands */
#define SWITCH_MULTILEVEL_VERSION_V4                                                     0x04
#define SWITCH_MULTILEVEL_GET_V4                                                         0x02
#define SWITCH_MULTILEVEL_REPORT_V4                                                      0x03
#define SWITCH_MULTILEVEL_SET_V4                                                         0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_V4                                          0x04
#define SWITCH_MULTILEVEL_STOP_LEVEL_CHANGE_V4                                           0x05
#define SWITCH_MULTILEVEL_SUPPORTED_GET_V4                                               0x06
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_V4                                            0x07
/* Values used for Switch Multilevel Report command */
#define SWITCH_MULTILEVEL_REPORT_ALREADY_AT_THE_TARGET_VALUE_V4                          0x00
#define SWITCH_MULTILEVEL_REPORT_UNKNOWN_DURATION_V4                                     0xFE
#define SWITCH_MULTILEVEL_REPORT_RESERVED_V4                                             0xFF
/* Values used for Switch Multilevel Set command */
#define SWITCH_MULTILEVEL_SET_INSTANTLY_V4                                               0x00
#define SWITCH_MULTILEVEL_SET_DEFAULT_V4                                                 0xFF
/* Values used for Switch Multilevel Start Level Change command */
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_RESERVED_MASK_V4                0x07
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_INC_DEC_MASK_V4                 0x18
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_INC_DEC_SHIFT_V4                0x03
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_INCREMENT_V4                        0x00
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_DECREMENT_V4                        0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_RESERVED_V4                         0x02
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_INC_DEC_NONE_V4                             0x03
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_LEVEL_BIT_MASK_V4  0x20
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_MASK_V4                 0xC0
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_SHIFT_V4                0x06
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_UP_V4                               0x00
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_DOWN_V4                             0x01
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_RESERVED_V4                         0x02
#define SWITCH_MULTILEVEL_START_LEVEL_CHANGE_UP_DOWN_NONE_V4                             0x03
/* Values used for Switch Multilevel Supported Report command */
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_PRIMARY_SWITCH_TYPE_MASK_V4       0x1F
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_MASK_V4                 0xE0
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_SHIFT_V4                0x05
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_SECONDARY_SWITCH_TYPE_MASK_V4     0x1F
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_MASK_V4                 0xE0
#define SWITCH_MULTILEVEL_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_SHIFT_V4                0x05

/* Switch Toggle Binary command class commands */
#define SWITCH_TOGGLE_BINARY_VERSION                                                     0x01
#define SWITCH_TOGGLE_BINARY_SET                                                         0x01 /*SWITCH_TOGGLE_BINARY_SET*/
#define SWITCH_TOGGLE_BINARY_GET                                                         0x02
#define SWITCH_TOGGLE_BINARY_REPORT                                                      0x03

/* Switch Toggle Multilevel command class commands */
#define SWITCH_TOGGLE_MULTILEVEL_VERSION                                                 0x01
#define SWITCH_TOGGLE_MULTILEVEL_SET                                                     0x01 /*SWITCH_TOGGLE_MULTILEVEL_SET*/
#define SWITCH_TOGGLE_MULTILEVEL_GET                                                     0x02
#define SWITCH_TOGGLE_MULTILEVEL_REPORT                                                  0x03
#define SWITCH_TOGGLE_MULTILEVEL_START_LEVEL_CHANGE                                      0x04
#define SWITCH_TOGGLE_MULTILEVEL_STOP_LEVEL_CHANGE                                       0x05
/* Values used for Switch Toggle Multilevel Start Level Change command */
#define SWITCH_TOGGLE_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_RESERVED1_MASK                 0x1F
#define SWITCH_TOGGLE_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_IGNORE_START_LEVEL_BIT_MASK    0x20
#define SWITCH_TOGGLE_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_RESERVED2_BIT_MASK             0x40
#define SWITCH_TOGGLE_MULTILEVEL_START_LEVEL_CHANGE_LEVEL_ROLL_OVER_BIT_MASK             0x80

/* Tariff Config command class commands */
#define TARIFF_CONFIG_VERSION                                                            0x01
#define TARIFF_TBL_REMOVE                                                                0x03
#define TARIFF_TBL_SET                                                                   0x02
#define TARIFF_TBL_SUPPLIER_SET                                                          0x01
/* Values used for Tariff Tbl Remove command */
#define TARIFF_TBL_REMOVE_PROPERTIES1_RATE_PARAMETER_SET_IDS_MASK                        0x3F
#define TARIFF_TBL_REMOVE_PROPERTIES1_RESERVED_MASK                                      0xC0
#define TARIFF_TBL_REMOVE_PROPERTIES1_RESERVED_SHIFT                                     0x06
/* Values used for Tariff Tbl Set command */
#define TARIFF_TBL_SET_PROPERTIES1_RESERVED_MASK                                         0x1F
#define TARIFF_TBL_SET_PROPERTIES1_TARIFF_PRECISION_MASK                                 0xE0
#define TARIFF_TBL_SET_PROPERTIES1_TARIFF_PRECISION_SHIFT                                0x05
/* Values used for Tariff Tbl Supplier Set command */
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES1_STANDING_CHARGE_PERIOD_MASK                  0x1F
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES1_STANDING_CHARGE_PRECISION_MASK               0xE0
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES1_STANDING_CHARGE_PRECISION_SHIFT              0x05
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES2_NUMBER_OF_SUPPLIER_CHARACTERS_MASK           0x1F
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES2_RESERVED_MASK                                0xE0
#define TARIFF_TBL_SUPPLIER_SET_PROPERTIES2_RESERVED_SHIFT                               0x05

/* Tariff Tbl Monitor command class commands */
#define TARIFF_TBL_MONITOR_VERSION                                                       0x01
#define TARIFF_TBL_COST_GET                                                              0x05
#define TARIFF_TBL_COST_REPORT                                                           0x06
#define TARIFF_TBL_GET                                                                   0x03
#define TARIFF_TBL_REPORT                                                                0x04
#define TARIFF_TBL_SUPPLIER_GET                                                          0x01
#define TARIFF_TBL_SUPPLIER_REPORT                                                       0x02
/* Values used for Tariff Tbl Cost Report command */
#define TARIFF_TBL_COST_REPORT_PROPERTIES1_RATE_TYPE_MASK                                0x03
#define TARIFF_TBL_COST_REPORT_PROPERTIES1_RESERVED1_MASK                                0xFC
#define TARIFF_TBL_COST_REPORT_PROPERTIES1_RESERVED1_SHIFT                               0x02
#define TARIFF_TBL_COST_REPORT_PROPERTIES2_RESERVED2_MASK                                0x1F
#define TARIFF_TBL_COST_REPORT_PROPERTIES2_COST_PRECISION_MASK                           0xE0
#define TARIFF_TBL_COST_REPORT_PROPERTIES2_COST_PRECISION_SHIFT                          0x05
/* Values used for Tariff Tbl Report command */
#define TARIFF_TBL_REPORT_PROPERTIES1_RESERVED_MASK                                      0x1F
#define TARIFF_TBL_REPORT_PROPERTIES1_TARIFF_PRECISION_MASK                              0xE0
#define TARIFF_TBL_REPORT_PROPERTIES1_TARIFF_PRECISION_SHIFT                             0x05
/* Values used for Tariff Tbl Supplier Report command */
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES1_STANDING_CHARGE_PERIOD_MASK               0x1F
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES1_STANDING_CHARGE_PRECISION_MASK            0xE0
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES1_STANDING_CHARGE_PRECISION_SHIFT           0x05
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES2_NUMBER_OF_SUPPLIER_CHARACTERS_MASK        0x1F
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES2_RESERVED_MASK                             0xE0
#define TARIFF_TBL_SUPPLIER_REPORT_PROPERTIES2_RESERVED_SHIFT                            0x05

/* Thermostat Fan Mode command class commands */
#define THERMOSTAT_FAN_MODE_VERSION                                                      0x01
#define THERMOSTAT_FAN_MODE_GET                                                          0x02
#define THERMOSTAT_FAN_MODE_REPORT                                                       0x03
#define THERMOSTAT_FAN_MODE_SET                                                          0x01
#define THERMOSTAT_FAN_MODE_SUPPORTED_GET                                                0x04
#define THERMOSTAT_FAN_MODE_SUPPORTED_REPORT                                             0x05
/* Values used for Thermostat Fan Mode Report command */
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_FAN_MODE_MASK                                   0x0F
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_LOW                                     0x00
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_LOW                                          0x01
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_HIGH                                    0x02
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HIGH                                         0x03
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_RESERVED_MASK                                   0xF0
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_RESERVED_SHIFT                                  0x04
/* Values used for Thermostat Fan Mode Set command */
#define THERMOSTAT_FAN_MODE_SET_LEVEL_FAN_MODE_MASK                                      0x0F
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_LOW                                        0x00
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_LOW                                             0x01
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_HIGH                                       0x02
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HIGH                                            0x03
#define THERMOSTAT_FAN_MODE_SET_LEVEL_RESERVED_MASK                                      0xF0
#define THERMOSTAT_FAN_MODE_SET_LEVEL_RESERVED_SHIFT                                     0x04

/* Thermostat Fan Mode command class commands */
#define THERMOSTAT_FAN_MODE_VERSION_V2                                                   0x02
#define THERMOSTAT_FAN_MODE_GET_V2                                                       0x02
#define THERMOSTAT_FAN_MODE_REPORT_V2                                                    0x03
#define THERMOSTAT_FAN_MODE_SET_V2                                                       0x01
#define THERMOSTAT_FAN_MODE_SUPPORTED_GET_V2                                             0x04
#define THERMOSTAT_FAN_MODE_SUPPORTED_REPORT_V2                                          0x05
/* Values used for Thermostat Fan Mode Report command */
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_FAN_MODE_MASK_V2                                0x0F
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_LOW_V2                                  0x00
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_LOW_V2                                       0x01
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_HIGH_V2                                 0x02
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HIGH_V2                                      0x03
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_MEDIUM_V2                               0x04
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_MEDIUM_V2                                    0x05
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_RESERVED_MASK_V2                                0xF0
#define THERMOSTAT_FAN_MODE_REPORT_LEVEL_RESERVED_SHIFT_V2                               0x04
/* Values used for Thermostat Fan Mode Set command */
#define THERMOSTAT_FAN_MODE_SET_LEVEL_FAN_MODE_MASK_V2                                   0x0F
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_LOW_V2                                     0x00
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_LOW_V2                                          0x01
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_HIGH_V2                                    0x02
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HIGH_V2                                         0x03
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_MEDIUM_V2                                  0x04
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_MEDIUM_V2                                       0x05
#define THERMOSTAT_FAN_MODE_SET_LEVEL_RESERVED_MASK_V2                                   0x70
#define THERMOSTAT_FAN_MODE_SET_LEVEL_RESERVED_SHIFT_V2                                  0x04
#define THERMOSTAT_FAN_MODE_SET_LEVEL_OFF_BIT_MASK_V2                                    0x80

/* Thermostat Fan Mode command class commands */
#define THERMOSTAT_FAN_MODE_VERSION_V3                                                   0x03
#define THERMOSTAT_FAN_MODE_GET_V3                                                       0x02
#define THERMOSTAT_FAN_MODE_REPORT_V3                                                    0x03
#define THERMOSTAT_FAN_MODE_SET_V3                                                       0x01
#define THERMOSTAT_FAN_MODE_SUPPORTED_GET_V3                                             0x04
#define THERMOSTAT_FAN_MODE_SUPPORTED_REPORT_V3                                          0x05
/* Values used for Thermostat Fan Mode Report command */
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_FAN_MODE_MASK_V3                          0x0F
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_LOW_V3                                  0x00
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_LOW_V3                                       0x01
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_HIGH_V3                                 0x02
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HIGH_V3                                      0x03
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_MEDIUM_V3                               0x04
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_MEDIUM_V3                                    0x05
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_CIRCULATION_V3                               0x06
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HUMIDITY_V3                                  0x07
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_RESERVED_MASK_V3                          0x70
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                         0x04
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_OFF_BIT_MASK_V3                           0x80
/* Values used for Thermostat Fan Mode Set command */
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_FAN_MODE_MASK_V3                             0x0F
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_LOW_V3                                     0x00
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_LOW_V3                                          0x01
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_HIGH_V3                                    0x02
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HIGH_V3                                         0x03
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_MEDIUM_V3                                  0x04
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_MEDIUM_V3                                       0x05
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_CIRCULATION_V3                                  0x06
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HUMIDITY_V3                                     0x07
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_RESERVED_MASK_V3                             0x70
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_RESERVED_SHIFT_V3                            0x04
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_OFF_BIT_MASK_V3                              0x80

/* Thermostat Fan Mode command class commands */
#define THERMOSTAT_FAN_MODE_VERSION_V4                                                   0x04
#define THERMOSTAT_FAN_MODE_GET_V4                                                       0x02
#define THERMOSTAT_FAN_MODE_REPORT_V4                                                    0x03
#define THERMOSTAT_FAN_MODE_SET_V4                                                       0x01
#define THERMOSTAT_FAN_MODE_SUPPORTED_GET_V4                                             0x04
#define THERMOSTAT_FAN_MODE_SUPPORTED_REPORT_V4                                          0x05
/* Values used for Thermostat Fan Mode Report command */
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_FAN_MODE_MASK_V4                          0x0F
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_LOW_V4                                  0x00
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_LOW_V4                                       0x01
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_HIGH_V4                                 0x02
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HIGH_V4                                      0x03
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_AUTO_MEDIUM_V4                               0x04
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_MEDIUM_V4                                    0x05
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_CIRCULATION_V4                               0x06
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_HUMIDITY_V4                                  0x07
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_LEFT_RIGHT_V4                                0x08
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_UP_DOWN_V4                                   0x09
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_QUIET_V4                                     0x0A
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_RESERVEDB_V4                                 0x0B
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_RESERVEDC_V4                                 0x0C
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_RESERVEDD_V4                                 0x0D
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_RESERVEDE_V4                                 0x0E
#define THERMOSTAT_FAN_MODE_REPORT_FAN_MODE_RESERVEDF_V4                                 0x0F
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_RESERVED_MASK_V4                          0x70
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_RESERVED_SHIFT_V4                         0x04
#define THERMOSTAT_FAN_MODE_REPORT_PROPERTIES1_OFF_BIT_MASK_V4                           0x80
/* Values used for Thermostat Fan Mode Set command */
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_FAN_MODE_MASK_V4                             0x0F
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_LOW_V4                                     0x00
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_LOW_V4                                          0x01
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_HIGH_V4                                    0x02
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HIGH_V4                                         0x03
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_AUTO_MEDIUM_V4                                  0x04
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_MEDIUM_V4                                       0x05
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_CIRCULATION_V4                                  0x06
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_HUMIDITY_V4                                     0x07
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_LEFT_RIGHT_V4                                   0x08
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_UP_DOWN_V4                                      0x09
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_QUIET_V4                                        0x0A
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_RESERVEDB_V4                                    0x0B
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_RESERVEDC_V4                                    0x0C
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_RESERVEDD_V4                                    0x0D
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_RESERVEDE_V4                                    0x0E
#define THERMOSTAT_FAN_MODE_SET_FAN_MODE_RESERVEDF_V4                                    0x0F
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_RESERVED_MASK_V4                             0x70
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_RESERVED_SHIFT_V4                            0x04
#define THERMOSTAT_FAN_MODE_SET_PROPERTIES1_OFF_BIT_MASK_V4                              0x80

/* Thermostat Fan State command class commands */
#define THERMOSTAT_FAN_STATE_VERSION                                                     0x01
#define THERMOSTAT_FAN_STATE_GET                                                         0x02
#define THERMOSTAT_FAN_STATE_REPORT                                                      0x03
/* Values used for Thermostat Fan State Report command */
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_FAN_OPERATING_STATE_MASK                       0x0F
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_IDLE                             0x00
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_RUNNING                          0x01
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_RESERVED_MASK                                  0xF0
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_RESERVED_SHIFT                                 0x04

/* Thermostat Fan State command class commands */
#define THERMOSTAT_FAN_STATE_VERSION_V2                                                  0x02
#define THERMOSTAT_FAN_STATE_GET_V2                                                      0x02
#define THERMOSTAT_FAN_STATE_REPORT_V2                                                   0x03
/* Values used for Thermostat Fan State Report command */
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_FAN_OPERATING_STATE_MASK_V2                    0x0F
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_IDLE_V2                          0x00
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_RUNNING_V2                       0x01
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_RUNNING_HIGH_V2                  0x02
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_RUNNING_MEDIUM_V2                0x03
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_CIRCULATION_V2                   0x04
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_HUMIDITY_CIRCULATION_V2          0x05
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_RIGHT_LEFT_CIRCULATION_V2        0x06
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_UP_DOWN_CIRCULATION_V2           0x07
#define THERMOSTAT_FAN_STATE_REPORT_FAN_OPERATING_STATE_QUIET_CIRCULATION_V2             0x08
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_RESERVED_MASK_V2                               0xF0
#define THERMOSTAT_FAN_STATE_REPORT_LEVEL_RESERVED_SHIFT_V2                              0x04

/* Thermostat Heating command class commands */
#define THERMOSTAT_HEATING_VERSION                                                       0x01
#define THERMOSTAT_HEATING_STATUS_REPORT                                                 0x0D
#define THERMOSTAT_HEATING_MODE_GET                                                      0x02
#define THERMOSTAT_HEATING_MODE_REPORT                                                   0x03
#define THERMOSTAT_HEATING_MODE_SET                                                      0x01
#define THERMOSTAT_HEATING_RELAY_STATUS_GET                                              0x09
#define THERMOSTAT_HEATING_RELAY_STATUS_REPORT                                           0x0A
#define THERMOSTAT_HEATING_SETPOINT_GET                                                  0x05
#define THERMOSTAT_HEATING_SETPOINT_REPORT                                               0x06
#define THERMOSTAT_HEATING_SETPOINT_SET                                                  0x04
#define THERMOSTAT_HEATING_STATUS_GET                                                    0x0C
#define THERMOSTAT_HEATING_STATUS_SET                                                    0x0B
#define THERMOSTAT_HEATING_TIMED_OFF_SET                                                 0x11
/* Values used for Thermostat Heating Status Report command */
#define THERMOSTAT_HEATING_STATUS_REPORT_HEATING                                         0x00
#define THERMOSTAT_HEATING_STATUS_REPORT_COOLING                                         0x01
/* Values used for Thermostat Heating Mode Report command */
#define THERMOSTAT_HEATING_MODE_REPORT_OFF                                               0x00
#define THERMOSTAT_HEATING_MODE_REPORT_OFF_TIMED                                         0x01
#define THERMOSTAT_HEATING_MODE_REPORT_OFF_3_HOURS                                       0x02
#define THERMOSTAT_HEATING_MODE_REPORT_ANTI_FREEZE                                       0x03
#define THERMOSTAT_HEATING_MODE_REPORT_MANUAL                                            0x04
#define THERMOSTAT_HEATING_MODE_REPORT_TEMPORARY_MANUAL                                  0x05
#define THERMOSTAT_HEATING_MODE_REPORT_AUTOMATIC                                         0x06
#define THERMOSTAT_HEATING_MODE_REPORT_MANUAL_TIMED                                      0x07
/* Values used for Thermostat Heating Mode Set command */
#define THERMOSTAT_HEATING_MODE_SET_OFF                                                  0x00
#define THERMOSTAT_HEATING_MODE_SET_OFF_TIMED                                            0x01
#define THERMOSTAT_HEATING_MODE_SET_OFF_3_HOURS                                          0x02
#define THERMOSTAT_HEATING_MODE_SET_ANTI_FREEZE                                          0x03
#define THERMOSTAT_HEATING_MODE_SET_MANUAL                                               0x04
#define THERMOSTAT_HEATING_MODE_SET_TEMPORARY_MANUAL                                     0x05
#define THERMOSTAT_HEATING_MODE_SET_AUTOMATIC                                            0x06
#define THERMOSTAT_HEATING_MODE_SET_MANUAL_TIMED                                         0x07
/* Values used for Thermostat Heating Relay Status Report command */
#define THERMOSTAT_HEATING_RELAY_STATUS_REPORT_OFF                                       0x00
#define THERMOSTAT_HEATING_RELAY_STATUS_REPORT_ON                                        0x01
/* Values used for Thermostat Heating Setpoint Report command */
#define THERMOSTAT_HEATING_SETPOINT_REPORT_PROPERTIES1_SIZE_MASK                         0x07
#define THERMOSTAT_HEATING_SETPOINT_REPORT_PROPERTIES1_SCALE_MASK                        0x18
#define THERMOSTAT_HEATING_SETPOINT_REPORT_PROPERTIES1_SCALE_SHIFT                       0x03
#define THERMOSTAT_HEATING_SETPOINT_REPORT_PROPERTIES1_PRECISION_MASK                    0xE0
#define THERMOSTAT_HEATING_SETPOINT_REPORT_PROPERTIES1_PRECISION_SHIFT                   0x05
/* Values used for Thermostat Heating Setpoint Set command */
#define THERMOSTAT_HEATING_SETPOINT_SET_PROPERTIES1_SIZE_MASK                            0x07
#define THERMOSTAT_HEATING_SETPOINT_SET_PROPERTIES1_SCALE_MASK                           0x18
#define THERMOSTAT_HEATING_SETPOINT_SET_PROPERTIES1_SCALE_SHIFT                          0x03
#define THERMOSTAT_HEATING_SETPOINT_SET_PROPERTIES1_PRECISION_MASK                       0xE0
#define THERMOSTAT_HEATING_SETPOINT_SET_PROPERTIES1_PRECISION_SHIFT                      0x05
/* Values used for Thermostat Heating Status Set command */
#define THERMOSTAT_HEATING_STATUS_SET_HEATING                                            0x00
#define THERMOSTAT_HEATING_STATUS_SET_COOLING                                            0x01

/* Thermostat Mode command class commands */
#define THERMOSTAT_MODE_VERSION                                                          0x01
#define THERMOSTAT_MODE_GET                                                              0x02
#define THERMOSTAT_MODE_REPORT                                                           0x03
#define THERMOSTAT_MODE_SET                                                              0x01
#define THERMOSTAT_MODE_SUPPORTED_GET                                                    0x04
#define THERMOSTAT_MODE_SUPPORTED_REPORT                                                 0x05
/* Values used for Thermostat Mode Report command */
#define THERMOSTAT_MODE_REPORT_LEVEL_MODE_MASK                                           0x1F
#define THERMOSTAT_MODE_REPORT_MODE_OFF                                                  0x00
#define THERMOSTAT_MODE_REPORT_MODE_HEAT                                                 0x01
#define THERMOSTAT_MODE_REPORT_MODE_COOL                                                 0x02
#define THERMOSTAT_MODE_REPORT_MODE_AUTO                                                 0x03
#define THERMOSTAT_MODE_REPORT_MODE_AUXILIARY_HEAT                                       0x04
#define THERMOSTAT_MODE_REPORT_MODE_RESUME                                               0x05
#define THERMOSTAT_MODE_REPORT_MODE_FAN_ONLY                                             0x06
#define THERMOSTAT_MODE_REPORT_MODE_FURNACE                                              0x07
#define THERMOSTAT_MODE_REPORT_MODE_DRY_AIR                                              0x08
#define THERMOSTAT_MODE_REPORT_MODE_MOIST_AIR                                            0x09
#define THERMOSTAT_MODE_REPORT_MODE_AUTO_CHANGEOVER                                      0x0A
#define THERMOSTAT_MODE_REPORT_LEVEL_RESERVED_MASK                                       0xE0
#define THERMOSTAT_MODE_REPORT_LEVEL_RESERVED_SHIFT                                      0x05
/* Values used for Thermostat Mode Set command */
#define THERMOSTAT_MODE_SET_LEVEL_MODE_MASK                                              0x1F
#define THERMOSTAT_MODE_SET_MODE_OFF                                                     0x00
#define THERMOSTAT_MODE_SET_MODE_HEAT                                                    0x01
#define THERMOSTAT_MODE_SET_MODE_COOL                                                    0x02
#define THERMOSTAT_MODE_SET_MODE_AUTO                                                    0x03
#define THERMOSTAT_MODE_SET_MODE_AUXILIARY_HEAT                                          0x04
#define THERMOSTAT_MODE_SET_MODE_RESUME                                                  0x05
#define THERMOSTAT_MODE_SET_MODE_FAN_ONLY                                                0x06
#define THERMOSTAT_MODE_SET_MODE_FURNACE                                                 0x07
#define THERMOSTAT_MODE_SET_MODE_DRY_AIR                                                 0x08
#define THERMOSTAT_MODE_SET_MODE_MOIST_AIR                                               0x09
#define THERMOSTAT_MODE_SET_MODE_AUTO_CHANGEOVER                                         0x0A
#define THERMOSTAT_MODE_SET_LEVEL_RESERVED_MASK                                          0xE0
#define THERMOSTAT_MODE_SET_LEVEL_RESERVED_SHIFT                                         0x05

/* Thermostat Mode command class commands */
#define THERMOSTAT_MODE_VERSION_V2                                                       0x02
#define THERMOSTAT_MODE_GET_V2                                                           0x02
#define THERMOSTAT_MODE_REPORT_V2                                                        0x03
#define THERMOSTAT_MODE_SET_V2                                                           0x01
#define THERMOSTAT_MODE_SUPPORTED_GET_V2                                                 0x04
#define THERMOSTAT_MODE_SUPPORTED_REPORT_V2                                              0x05
/* Values used for Thermostat Mode Report command */
#define THERMOSTAT_MODE_REPORT_LEVEL_MODE_MASK_V2                                        0x1F
#define THERMOSTAT_MODE_REPORT_MODE_OFF_V2                                               0x00
#define THERMOSTAT_MODE_REPORT_MODE_HEAT_V2                                              0x01
#define THERMOSTAT_MODE_REPORT_MODE_COOL_V2                                              0x02
#define THERMOSTAT_MODE_REPORT_MODE_AUTO_V2                                              0x03
#define THERMOSTAT_MODE_REPORT_MODE_AUXILIARY_HEAT_V2                                    0x04
#define THERMOSTAT_MODE_REPORT_MODE_RESUME_V2                                            0x05
#define THERMOSTAT_MODE_REPORT_MODE_FAN_ONLY_V2                                          0x06
#define THERMOSTAT_MODE_REPORT_MODE_FURNACE_V2                                           0x07
#define THERMOSTAT_MODE_REPORT_MODE_DRY_AIR_V2                                           0x08
#define THERMOSTAT_MODE_REPORT_MODE_MOIST_AIR_V2                                         0x09
#define THERMOSTAT_MODE_REPORT_MODE_AUTO_CHANGEOVER_V2                                   0x0A
#define THERMOSTAT_MODE_REPORT_MODE_ENERGY_SAVE_HEAT_V2                                  0x0B
#define THERMOSTAT_MODE_REPORT_MODE_ENERGY_SAVE_COOL_V2                                  0x0C
#define THERMOSTAT_MODE_REPORT_MODE_AWAY_V2                                              0x0D
#define THERMOSTAT_MODE_REPORT_LEVEL_RESERVED_MASK_V2                                    0xE0
#define THERMOSTAT_MODE_REPORT_LEVEL_RESERVED_SHIFT_V2                                   0x05
/* Values used for Thermostat Mode Set command */
#define THERMOSTAT_MODE_SET_LEVEL_MODE_MASK_V2                                           0x1F
#define THERMOSTAT_MODE_SET_MODE_OFF_V2                                                  0x00
#define THERMOSTAT_MODE_SET_MODE_HEAT_V2                                                 0x01
#define THERMOSTAT_MODE_SET_MODE_COOL_V2                                                 0x02
#define THERMOSTAT_MODE_SET_MODE_AUTO_V2                                                 0x03
#define THERMOSTAT_MODE_SET_MODE_AUXILIARY_HEAT_V2                                       0x04
#define THERMOSTAT_MODE_SET_MODE_RESUME_V2                                               0x05
#define THERMOSTAT_MODE_SET_MODE_FAN_ONLY_V2                                             0x06
#define THERMOSTAT_MODE_SET_MODE_FURNACE_V2                                              0x07
#define THERMOSTAT_MODE_SET_MODE_DRY_AIR_V2                                              0x08
#define THERMOSTAT_MODE_SET_MODE_MOIST_AIR_V2                                            0x09
#define THERMOSTAT_MODE_SET_MODE_AUTO_CHANGEOVER_V2                                      0x0A
#define THERMOSTAT_MODE_SET_MODE_ENERGY_SAVE_HEAT_V2                                     0x0B
#define THERMOSTAT_MODE_SET_MODE_ENERGY_SAVE_COOL_V2                                     0x0C
#define THERMOSTAT_MODE_SET_MODE_AWAY_V2                                                 0x0D
#define THERMOSTAT_MODE_SET_LEVEL_RESERVED_MASK_V2                                       0xE0
#define THERMOSTAT_MODE_SET_LEVEL_RESERVED_SHIFT_V2                                      0x05

/* Thermostat Mode command class commands */
#define THERMOSTAT_MODE_VERSION_V3                                                       0x03
#define THERMOSTAT_MODE_GET_V3                                                           0x02
#define THERMOSTAT_MODE_REPORT_V3                                                        0x03
#define THERMOSTAT_MODE_SET_V3                                                           0x01
#define THERMOSTAT_MODE_SUPPORTED_GET_V3                                                 0x04
#define THERMOSTAT_MODE_SUPPORTED_REPORT_V3                                              0x05
/* Values used for Thermostat Mode Report command */
#define THERMOSTAT_MODE_REPORT_LEVEL_MODE_MASK_V3                                        0x1F
#define THERMOSTAT_MODE_REPORT_MODE_OFF_V3                                               0x00
#define THERMOSTAT_MODE_REPORT_MODE_HEAT_V3                                              0x01
#define THERMOSTAT_MODE_REPORT_MODE_COOL_V3                                              0x02
#define THERMOSTAT_MODE_REPORT_MODE_AUTO_V3                                              0x03
#define THERMOSTAT_MODE_REPORT_MODE_AUXILIARY_HEAT_V3                                    0x04
#define THERMOSTAT_MODE_REPORT_MODE_RESUME_V3                                            0x05
#define THERMOSTAT_MODE_REPORT_MODE_FAN_ONLY_V3                                          0x06
#define THERMOSTAT_MODE_REPORT_MODE_FURNACE_V3                                           0x07
#define THERMOSTAT_MODE_REPORT_MODE_DRY_AIR_V3                                           0x08
#define THERMOSTAT_MODE_REPORT_MODE_MOIST_AIR_V3                                         0x09
#define THERMOSTAT_MODE_REPORT_MODE_AUTO_CHANGEOVER_V3                                   0x0A
#define THERMOSTAT_MODE_REPORT_MODE_ENERGY_SAVE_HEAT_V3                                  0x0B
#define THERMOSTAT_MODE_REPORT_MODE_ENERGY_SAVE_COOL_V3                                  0x0C
#define THERMOSTAT_MODE_REPORT_MODE_AWAY_V3                                              0x0D
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED_V3                                          0x0E
#define THERMOSTAT_MODE_REPORT_MODE_FULL_POWER_V3                                        0x0F
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED0_V3                                         0x10
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED1_V3                                         0x11
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED2_V3                                         0x12
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED3_V3                                         0x13
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED4_V3                                         0x14
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED5_V3                                         0x15
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED6_V3                                         0x16
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED7_V3                                         0x17
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED8_V3                                         0x18
#define THERMOSTAT_MODE_REPORT_MODE_RESERVED9_V3                                         0x19
#define THERMOSTAT_MODE_REPORT_MODE_RESERVEDA_V3                                         0x1A
#define THERMOSTAT_MODE_REPORT_MODE_RESERVEDB_V3                                         0x1B
#define THERMOSTAT_MODE_REPORT_MODE_RESERVEDC_V3                                         0x1C
#define THERMOSTAT_MODE_REPORT_MODE_RESERVEDD_V3                                         0x1D
#define THERMOSTAT_MODE_REPORT_MODE_RESERVEDE_V3                                         0x1E
#define THERMOSTAT_MODE_REPORT_MODE_MANUFACTURER_SPECIFC_V3                              0x1F
#define THERMOSTAT_MODE_REPORT_LEVEL_NO_OF_MANUFACTURER_DATA_FIELDS_MASK_V3              0xE0
#define THERMOSTAT_MODE_REPORT_LEVEL_NO_OF_MANUFACTURER_DATA_FIELDS_SHIFT_V3             0x05
/* Values used for Thermostat Mode Set command */
#define THERMOSTAT_MODE_SET_LEVEL_MODE_MASK_V3                                           0x1F
#define THERMOSTAT_MODE_SET_MODE_OFF_V3                                                  0x00
#define THERMOSTAT_MODE_SET_MODE_HEAT_V3                                                 0x01
#define THERMOSTAT_MODE_SET_MODE_COOL_V3                                                 0x02
#define THERMOSTAT_MODE_SET_MODE_AUTO_V3                                                 0x03
#define THERMOSTAT_MODE_SET_MODE_AUXILIARY_HEAT_V3                                       0x04
#define THERMOSTAT_MODE_SET_MODE_RESUME_V3                                               0x05
#define THERMOSTAT_MODE_SET_MODE_FAN_ONLY_V3                                             0x06
#define THERMOSTAT_MODE_SET_MODE_FURNACE_V3                                              0x07
#define THERMOSTAT_MODE_SET_MODE_DRY_AIR_V3                                              0x08
#define THERMOSTAT_MODE_SET_MODE_MOIST_AIR_V3                                            0x09
#define THERMOSTAT_MODE_SET_MODE_AUTO_CHANGEOVER_V3                                      0x0A
#define THERMOSTAT_MODE_SET_MODE_ENERGY_SAVE_HEAT_V3                                     0x0B
#define THERMOSTAT_MODE_SET_MODE_ENERGY_SAVE_COOL_V3                                     0x0C
#define THERMOSTAT_MODE_SET_MODE_AWAY_V3                                                 0x0D
#define THERMOSTAT_MODE_SET_MODE_RESERVED_V3                                             0x0E
#define THERMOSTAT_MODE_SET_MODE_FULL_POWER_V3                                           0x0F
#define THERMOSTAT_MODE_SET_MODE_RESERVED0_V3                                            0x10
#define THERMOSTAT_MODE_SET_MODE_RESERVED1_V3                                            0x11
#define THERMOSTAT_MODE_SET_MODE_RESERVED2_V3                                            0x12
#define THERMOSTAT_MODE_SET_MODE_RESERVED3_V3                                            0x13
#define THERMOSTAT_MODE_SET_MODE_RESERVED4_V3                                            0x14
#define THERMOSTAT_MODE_SET_MODE_RESERVED5_V3                                            0x15
#define THERMOSTAT_MODE_SET_MODE_RESERVED6_V3                                            0x16
#define THERMOSTAT_MODE_SET_MODE_RESERVED7_V3                                            0x17
#define THERMOSTAT_MODE_SET_MODE_RESERVED8_V3                                            0x18
#define THERMOSTAT_MODE_SET_MODE_RESERVED9_V3                                            0x19
#define THERMOSTAT_MODE_SET_MODE_RESERVEDA_V3                                            0x1A
#define THERMOSTAT_MODE_SET_MODE_RESERVEDB_V3                                            0x1B
#define THERMOSTAT_MODE_SET_MODE_RESERVEDC_V3                                            0x1C
#define THERMOSTAT_MODE_SET_MODE_RESERVEDD_V3                                            0x1D
#define THERMOSTAT_MODE_SET_MODE_RESERVEDE_V3                                            0x1E
#define THERMOSTAT_MODE_SET_MODE_MANUFACTURER_SPECIFC_V3                                 0x1F
#define THERMOSTAT_MODE_SET_LEVEL_NO_OF_MANUFACTURER_DATA_FIELDS_MASK_V3                 0xE0
#define THERMOSTAT_MODE_SET_LEVEL_NO_OF_MANUFACTURER_DATA_FIELDS_SHIFT_V3                0x05

/* Thermostat Operating State command class commands */
#define THERMOSTAT_OPERATING_STATE_VERSION                                               0x01
#define THERMOSTAT_OPERATING_STATE_GET                                                   0x02
#define THERMOSTAT_OPERATING_STATE_REPORT                                                0x03
/* Values used for Thermostat Operating State Report command */
#define THERMOSTAT_OPERATING_STATE_REPORT_LEVEL_OPERATING_STATE_MASK                     0x0F
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_IDLE                           0x00
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_HEATING                        0x01
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_COOLING                        0x02
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_FAN_ONLY                       0x03
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_PENDING_HEAT                   0x04
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_PENDING_COOL                   0x05
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_VENT_ECONOMIZER                0x06
#define THERMOSTAT_OPERATING_STATE_REPORT_LEVEL_RESERVED_MASK                            0xF0
#define THERMOSTAT_OPERATING_STATE_REPORT_LEVEL_RESERVED_SHIFT                           0x04

/* Thermostat Operating State command class commands */
#define THERMOSTAT_OPERATING_STATE_VERSION_V2                                            0x02
#define THERMOSTAT_OPERATING_STATE_GET_V2                                                0x02
#define THERMOSTAT_OPERATING_STATE_REPORT_V2                                             0x03
#define THERMOSTAT_OPERATING_STATE_LOGGING_SUPPORTED_GET_V2                              0x01
#define THERMOSTAT_OPERATING_LOGGING_SUPPORTED_REPORT_V2                                 0x04
#define THERMOSTAT_OPERATING_STATE_LOGGING_GET_V2                                        0x05
#define THERMOSTAT_OPERATING_STATE_LOGGING_REPORT_V2                                     0x06
/* Values used for Thermostat Operating State Report command */
#define THERMOSTAT_OPERATING_STATE_REPORT_PROPERTIES1_OPERATING_STATE_MASK_V2            0x0F
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_IDLE_V2                        0x00
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_HEATING_V2                     0x01
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_COOLING_V2                     0x02
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_FAN_ONLY_V2                    0x03
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_PENDING_HEAT_V2                0x04
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_PENDING_COOL_V2                0x05
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_VENT_ECONOMIZER_V2             0x06
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_AUX_HEATING_V2                 0x07
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_2ND_STAGE_HEATING_V2           0x08
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_2ND_STAGE_COOLING_V2           0x09
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_2ND_STAGE_AUX_HEAT_V2          0x0A
#define THERMOSTAT_OPERATING_STATE_REPORT_OPERATING_STATE_3RD_STAGE_AUX_HEAT_V2          0x0B
#define THERMOSTAT_OPERATING_STATE_REPORT_PROPERTIES1_RESERVED_MASK_V2                   0xF0
#define THERMOSTAT_OPERATING_STATE_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                  0x04

/* Thermostat Setback command class commands */
#define THERMOSTAT_SETBACK_VERSION                                                       0x01
#define THERMOSTAT_SETBACK_GET                                                           0x02
#define THERMOSTAT_SETBACK_REPORT                                                        0x03
#define THERMOSTAT_SETBACK_SET                                                           0x01
/* Values used for Thermostat Setback Report command */
#define THERMOSTAT_SETBACK_REPORT_PROPERTIES1_SETBACK_TYPE_MASK                          0x03
#define THERMOSTAT_SETBACK_REPORT_SETBACK_TYPE_NO_OVERRIDE                               0x00
#define THERMOSTAT_SETBACK_REPORT_SETBACK_TYPE_TEMPORARY_OVERRIDE                        0x01
#define THERMOSTAT_SETBACK_REPORT_SETBACK_TYPE_PERMANENT_OVERRIDE                        0x02
#define THERMOSTAT_SETBACK_REPORT_SETBACK_TYPE_RESERVED                                  0x03
#define THERMOSTAT_SETBACK_REPORT_PROPERTIES1_RESERVED_MASK                              0xFC
#define THERMOSTAT_SETBACK_REPORT_PROPERTIES1_RESERVED_SHIFT                             0x02
/* Values used for Thermostat Setback Set command */
#define THERMOSTAT_SETBACK_SET_PROPERTIES1_SETBACK_TYPE_MASK                             0x03
#define THERMOSTAT_SETBACK_SET_SETBACK_TYPE_NO_OVERRIDE                                  0x00
#define THERMOSTAT_SETBACK_SET_SETBACK_TYPE_TEMPORARY_OVERRIDE                           0x01
#define THERMOSTAT_SETBACK_SET_SETBACK_TYPE_PERMANENT_OVERRIDE                           0x02
#define THERMOSTAT_SETBACK_SET_SETBACK_TYPE_RESERVED                                     0x03
#define THERMOSTAT_SETBACK_SET_PROPERTIES1_RESERVED_MASK                                 0xFC
#define THERMOSTAT_SETBACK_SET_PROPERTIES1_RESERVED_SHIFT                                0x02

/* Thermostat Setpoint command class commands */
#define THERMOSTAT_SETPOINT_VERSION                                                      0x01
#define THERMOSTAT_SETPOINT_GET                                                          0x02
#define THERMOSTAT_SETPOINT_REPORT                                                       0x03
#define THERMOSTAT_SETPOINT_SET                                                          0x01
#define THERMOSTAT_SETPOINT_SUPPORTED_GET                                                0x04
#define THERMOSTAT_SETPOINT_SUPPORTED_REPORT                                             0x05
/* Values used for Thermostat Setpoint Get command */
#define THERMOSTAT_SETPOINT_GET_LEVEL_SETPOINT_TYPE_MASK                                 0x0F
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED                              0x00
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_HEATING_1                                  0x01
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_COOLING_1                                  0x02
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED1                             0x03
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED2                             0x04
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED3                             0x05
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED4                             0x06
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_FURNACE                                    0x07
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_DRY_AIR                                    0x08
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_MOIST_AIR                                  0x09
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AUTO_CHANGEOVER                            0x0A
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_MASK                                      0xF0
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_SHIFT                                     0x04
/* Values used for Thermostat Setpoint Report command */
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_SETPOINT_TYPE_MASK                              0x0F
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED                           0x00
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_HEATING_1                               0x01
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_COOLING_1                               0x02
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED1                          0x03
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED2                          0x04
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED3                          0x05
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED4                          0x06
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_FURNACE                                 0x07
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_DRY_AIR                                 0x08
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_MOIST_AIR                               0x09
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AUTO_CHANGEOVER                         0x0A
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_MASK                                   0xF0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_SHIFT                                  0x04
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SIZE_MASK                                      0x07
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_MASK                                     0x18
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_SHIFT                                    0x03
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_MASK                                 0xE0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_SHIFT                                0x05
/* Values used for Thermostat Setpoint Set command */
#define THERMOSTAT_SETPOINT_SET_LEVEL_SETPOINT_TYPE_MASK                                 0x0F
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED                              0x00
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_HEATING_1                                  0x01
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_COOLING_1                                  0x02
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED1                             0x03
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED2                             0x04
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED3                             0x05
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED4                             0x06
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_FURNACE                                    0x07
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_DRY_AIR                                    0x08
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_MOIST_AIR                                  0x09
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AUTO_CHANGEOVER                            0x0A
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_MASK                                      0xF0
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_SHIFT                                     0x04
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SIZE_MASK                                         0x07
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_MASK                                        0x18
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_SHIFT                                       0x03
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_MASK                                    0xE0
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_SHIFT                                   0x05

/* Thermostat Setpoint command class commands */
#define THERMOSTAT_SETPOINT_VERSION_V2                                                   0x02
#define THERMOSTAT_SETPOINT_GET_V2                                                       0x02
#define THERMOSTAT_SETPOINT_REPORT_V2                                                    0x03
#define THERMOSTAT_SETPOINT_SET_V2                                                       0x01
#define THERMOSTAT_SETPOINT_SUPPORTED_GET_V2                                             0x04
#define THERMOSTAT_SETPOINT_SUPPORTED_REPORT_V2                                          0x05
/* Values used for Thermostat Setpoint Get command */
#define THERMOSTAT_SETPOINT_GET_LEVEL_SETPOINT_TYPE_MASK_V2                              0x0F
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED_V2                           0x00
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_HEATING_1_V2                               0x01
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_COOLING_1_V2                               0x02
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED1_V2                          0x03
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED2_V2                          0x04
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED3_V2                          0x05
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED4_V2                          0x06
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_FURNACE_V2                                 0x07
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_DRY_AIR_V2                                 0x08
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_MOIST_AIR_V2                               0x09
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AUTO_CHANGEOVER_V2                         0x0A
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V2                     0x0B
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V2                     0x0C
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AWAY_HEATING_V2                            0x0D
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_MASK_V2                                   0xF0
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_SHIFT_V2                                  0x04
/* Values used for Thermostat Setpoint Report command */
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_SETPOINT_TYPE_MASK_V2                           0x0F
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED_V2                        0x00
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_HEATING_1_V2                            0x01
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_COOLING_1_V2                            0x02
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED1_V2                       0x03
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED2_V2                       0x04
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED3_V2                       0x05
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED4_V2                       0x06
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_FURNACE_V2                              0x07
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_DRY_AIR_V2                              0x08
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_MOIST_AIR_V2                            0x09
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AUTO_CHANGEOVER_V2                      0x0A
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V2                  0x0B
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V2                  0x0C
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AWAY_HEATING_V2                         0x0D
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_MASK_V2                                0xF0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_SHIFT_V2                               0x04
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SIZE_MASK_V2                                   0x07
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_MASK_V2                                  0x18
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_SHIFT_V2                                 0x03
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_MASK_V2                              0xE0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_SHIFT_V2                             0x05
/* Values used for Thermostat Setpoint Set command */
#define THERMOSTAT_SETPOINT_SET_LEVEL_SETPOINT_TYPE_MASK_V2                              0x0F
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED_V2                           0x00
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_HEATING_1_V2                               0x01
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_COOLING_1_V2                               0x02
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED1_V2                          0x03
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED2_V2                          0x04
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED3_V2                          0x05
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED4_V2                          0x06
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_FURNACE_V2                                 0x07
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_DRY_AIR_V2                                 0x08
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_MOIST_AIR_V2                               0x09
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AUTO_CHANGEOVER_V2                         0x0A
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V2                     0x0B
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V2                     0x0C
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AWAY_HEATING_V2                            0x0D
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_MASK_V2                                   0xF0
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_SHIFT_V2                                  0x04
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SIZE_MASK_V2                                      0x07
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_MASK_V2                                     0x18
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_SHIFT_V2                                    0x03
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_MASK_V2                                 0xE0
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_SHIFT_V2                                0x05

/* Thermostat Setpoint command class commands */
#define THERMOSTAT_SETPOINT_VERSION_V3                                                   0x03
#define THERMOSTAT_SETPOINT_GET_V3                                                       0x02
#define THERMOSTAT_SETPOINT_REPORT_V3                                                    0x03
#define THERMOSTAT_SETPOINT_SET_V3                                                       0x01
#define THERMOSTAT_SETPOINT_SUPPORTED_GET_V3                                             0x04
#define THERMOSTAT_SETPOINT_SUPPORTED_REPORT_V3                                          0x05
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_V3                                          0x09
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_V3                                       0x0A
/* Values used for Thermostat Setpoint Get command */
#define THERMOSTAT_SETPOINT_GET_LEVEL_SETPOINT_TYPE_MASK_V3                              0x0F
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED_V3                           0x00
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_HEATING_1_V3                               0x01
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_COOLING_1_V3                               0x02
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED1_V3                          0x03
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED2_V3                          0x04
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED3_V3                          0x05
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_NOT_SUPPORTED4_V3                          0x06
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_FURNACE_V3                                 0x07
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_DRY_AIR_V3                                 0x08
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_MOIST_AIR_V3                               0x09
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AUTO_CHANGEOVER_V3                         0x0A
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V3                     0x0B
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V3                     0x0C
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AWAY_HEATING_V3                            0x0D
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_AWAY_COOLING_V3                            0x0E
#define THERMOSTAT_SETPOINT_GET_SETPOINT_TYPE_FULL_POWER_V3                              0x0F
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_MASK_V3                                   0xF0
#define THERMOSTAT_SETPOINT_GET_LEVEL_RESERVED_SHIFT_V3                                  0x04
/* Values used for Thermostat Setpoint Report command */
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_SETPOINT_TYPE_MASK_V3                           0x0F
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED_V3                        0x00
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_HEATING_1_V3                            0x01
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_COOLING_1_V3                            0x02
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED1_V3                       0x03
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED2_V3                       0x04
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED3_V3                       0x05
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_NOT_SUPPORTED4_V3                       0x06
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_FURNACE_V3                              0x07
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_DRY_AIR_V3                              0x08
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_MOIST_AIR_V3                            0x09
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AUTO_CHANGEOVER_V3                      0x0A
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V3                  0x0B
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V3                  0x0C
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AWAY_HEATING_V3                         0x0D
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_AWAY_COOLING_V3                         0x0E
#define THERMOSTAT_SETPOINT_REPORT_SETPOINT_TYPE_FULL_POWER_V3                           0x0F
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_MASK_V3                                0xF0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL_RESERVED_SHIFT_V3                               0x04
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SIZE_MASK_V3                                   0x07
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_MASK_V3                                  0x18
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_SCALE_SHIFT_V3                                 0x03
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_MASK_V3                              0xE0
#define THERMOSTAT_SETPOINT_REPORT_LEVEL2_PRECISION_SHIFT_V3                             0x05
/* Values used for Thermostat Setpoint Set command */
#define THERMOSTAT_SETPOINT_SET_LEVEL_SETPOINT_TYPE_MASK_V3                              0x0F
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED_V3                           0x00
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_HEATING_1_V3                               0x01
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_COOLING_1_V3                               0x02
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED1_V3                          0x03
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED2_V3                          0x04
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED3_V3                          0x05
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_NOT_SUPPORTED4_V3                          0x06
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_FURNACE_V3                                 0x07
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_DRY_AIR_V3                                 0x08
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_MOIST_AIR_V3                               0x09
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AUTO_CHANGEOVER_V3                         0x0A
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V3                     0x0B
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V3                     0x0C
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AWAY_HEATING_V3                            0x0D
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_AWAY_COOLING_V3                            0x0E
#define THERMOSTAT_SETPOINT_SET_SETPOINT_TYPE_FULL_POWER_V3                              0x0F
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_MASK_V3                                   0xF0
#define THERMOSTAT_SETPOINT_SET_LEVEL_RESERVED_SHIFT_V3                                  0x04
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SIZE_MASK_V3                                      0x07
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_MASK_V3                                     0x18
#define THERMOSTAT_SETPOINT_SET_LEVEL2_SCALE_SHIFT_V3                                    0x03
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_MASK_V3                                 0xE0
#define THERMOSTAT_SETPOINT_SET_LEVEL2_PRECISION_SHIFT_V3                                0x05
/* Values used for Thermostat Setpoint Capabilities Get command */
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_PROPERTIES1_SETPOINT_TYPE_MASK_V3           0x0F
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_NOT_SUPPORTED_V3              0x00
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_HEATING_1_V3                  0x01
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_COOLING_1_V3                  0x02
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_NOT_SUPPORTED1_V3             0x03
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_NOT_SUPPORTED2_V3             0x04
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_NOT_SUPPORTED3_V3             0x05
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_NOT_SUPPORTED4_V3             0x06
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_FURNACE_V3                    0x07
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_DRY_AIR_V3                    0x08
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_MOIST_AIR_V3                  0x09
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_AUTO_CHANGEOVER_V3            0x0A
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V3        0x0B
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V3        0x0C
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_AWAY_HEATING_V3               0x0D
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_AWAY_COOLING_V3               0x0E
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_FULL_POWER_V3                 0x0F
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_PROPERTIES1_RESERVED_MASK_V3                0xF0
#define THERMOSTAT_SETPOINT_CAPABILITIES_GET_PROPERTIES1_RESERVED_SHIFT_V3               0x04
/* Values used for Thermostat Setpoint Capabilities Report command */
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_SETPOINT_TYPE_MASK_V3        0x0F
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_NOT_SUPPORTED_V3           0x00
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_HEATING_1_V3               0x01
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_COOLING_1_V3               0x02
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_NOT_SUPPORTED1_V3          0x03
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_NOT_SUPPORTED2_V3          0x04
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_NOT_SUPPORTED3_V3          0x05
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_NOT_SUPPORTED4_V3          0x06
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_FURNACE_V3                 0x07
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_DRY_AIR_V3                 0x08
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_MOIST_AIR_V3               0x09
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_AUTO_CHANGEOVER_V3         0x0A
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_ENERGY_SAVE_HEATING_V3     0x0B
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_ENERGY_SAVE_COOLING_V3     0x0C
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_AWAY_HEATING_V3            0x0D
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_AWAY_COOLING_V3            0x0E
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_FULL_POWER_V3              0x0F
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_RESERVED_MASK_V3             0xF0
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_RESERVED_SHIFT_V3            0x04
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SIZE1_MASK_V3                0x07
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SCALE1_MASK_V3               0x18
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SCALE1_SHIFT_V3              0x03
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_PRECISION1_MASK_V3           0xE0
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_PRECISION1_SHIFT_V3          0x05
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SIZE2_MASK_V3                0x07
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SCALE2_MASK_V3               0x18
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SCALE2_SHIFT_V3              0x03
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_PRECISION2_MASK_V3           0xE0
#define THERMOSTAT_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_PRECISION2_SHIFT_V3          0x05

/* Time Parameters command class commands */
#define TIME_PARAMETERS_VERSION                                                          0x01
#define TIME_PARAMETERS_GET                                                              0x02
#define TIME_PARAMETERS_REPORT                                                           0x03
#define TIME_PARAMETERS_SET                                                              0x01

/* Time command class commands */
#define TIME_VERSION                                                                     0x01
#define DATE_GET                                                                         0x03
#define DATE_REPORT                                                                      0x04
#define TIME_GET                                                                         0x01
#define TIME_REPORT                                                                      0x02
/* Values used for Time Report command */
#define TIME_REPORT_HOUR_LOCAL_TIME_HOUR_LOCAL_TIME_MASK                                 0x1F
#define TIME_REPORT_HOUR_LOCAL_TIME_RESERVED_MASK                                        0x60
#define TIME_REPORT_HOUR_LOCAL_TIME_RESERVED_SHIFT                                       0x05
#define TIME_REPORT_HOUR_LOCAL_TIME_RTC_FAILURE_BIT_MASK                                 0x80

/* Time command class commands */
#define TIME_VERSION_V2                                                                  0x02
#define DATE_GET_V2                                                                      0x03
#define DATE_REPORT_V2                                                                   0x04
#define TIME_GET_V2                                                                      0x01
#define TIME_OFFSET_GET_V2                                                               0x06
#define TIME_OFFSET_REPORT_V2                                                            0x07
#define TIME_OFFSET_SET_V2                                                               0x05
#define TIME_REPORT_V2                                                                   0x02
/* Values used for Time Offset Report command */
#define TIME_OFFSET_REPORT_LEVEL_HOUR_TZO_MASK_V2                                        0x7F
#define TIME_OFFSET_REPORT_LEVEL_SIGN_TZO_BIT_MASK_V2                                    0x80
#define TIME_OFFSET_REPORT_LEVEL2_MINUTE_OFFSET_DST_MASK_V2                              0x7F
#define TIME_OFFSET_REPORT_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V2                            0x80
/* Values used for Time Offset Set command */
#define TIME_OFFSET_SET_LEVEL_HOUR_TZO_MASK_V2                                           0x7F
#define TIME_OFFSET_SET_LEVEL_SIGN_TZO_BIT_MASK_V2                                       0x80
#define TIME_OFFSET_SET_LEVEL2_MINUTE_OFFSET_DST_MASK_V2                                 0x7F
#define TIME_OFFSET_SET_LEVEL2_SIGN_OFFSET_DST_BIT_MASK_V2                               0x80
/* Values used for Time Report command */
#define TIME_REPORT_HOUR_LOCAL_TIME_HOUR_LOCAL_TIME_MASK_V2                              0x1F
#define TIME_REPORT_HOUR_LOCAL_TIME_RESERVED_MASK_V2                                     0x60
#define TIME_REPORT_HOUR_LOCAL_TIME_RESERVED_SHIFT_V2                                    0x05
#define TIME_REPORT_HOUR_LOCAL_TIME_RTC_FAILURE_BIT_MASK_V2                              0x80

/* Transport Service command class commands */
#define TRANSPORT_SERVICE_VERSION_V2                                                     0x02
#define COMMAND_FIRST_SEGMENT_V2                                                         0xC0
#define COMMAND_SEGMENT_COMPLETE_V2                                                      0xE8
#define COMMAND_SEGMENT_REQUEST_V2                                                       0xC8
#define COMMAND_SEGMENT_WAIT_V2                                                          0xF0
#define COMMAND_SUBSEQUENT_SEGMENT_V2                                                    0xE0
/* Values used for Command First Segment command */
#define COMMAND_FIRST_SEGMENT_MASK_V2                                                    0xF8
#define COMMAND_FIRST_SEGMENT_DATAGRAM_SIZE_1_MASK_V2                                    0x07
#define COMMAND_FIRST_SEGMENT_PROPERTIES2_RESERVED_MASK_V2                               0x07
#define COMMAND_FIRST_SEGMENT_PROPERTIES2_EXT_BIT_MASK_V2                                0x08
#define COMMAND_FIRST_SEGMENT_PROPERTIES2_SESSION_ID_MASK_V2                             0xF0
#define COMMAND_FIRST_SEGMENT_PROPERTIES2_SESSION_ID_SHIFT_V2                            0x04
/* Values used for Command Segment Complete command */
#define COMMAND_SEGMENT_COMPLETE_MASK_V2                                                 0xF8
#define COMMAND_SEGMENT_COMPLETE_RESERVED_MASK_V2                                        0x07
#define COMMAND_SEGMENT_COMPLETE_PROPERTIES2_RESERVED2_MASK_V2                           0x0F
#define COMMAND_SEGMENT_COMPLETE_PROPERTIES2_SESSION_ID_MASK_V2                          0xF0
#define COMMAND_SEGMENT_COMPLETE_PROPERTIES2_SESSION_ID_SHIFT_V2                         0x04
/* Values used for Command Segment Request command */
#define COMMAND_SEGMENT_REQUEST_MASK_V2                                                  0xF8
#define COMMAND_SEGMENT_REQUEST_RESERVED_MASK_V2                                         0x07
#define COMMAND_SEGMENT_REQUEST_PROPERTIES2_DATAGRAM_OFFSET_1_MASK_V2                    0x07
#define COMMAND_SEGMENT_REQUEST_PROPERTIES2_RESERVED2_BIT_MASK_V2                        0x08
#define COMMAND_SEGMENT_REQUEST_PROPERTIES2_SESSION_ID_MASK_V2                           0xF0
#define COMMAND_SEGMENT_REQUEST_PROPERTIES2_SESSION_ID_SHIFT_V2                          0x04
/* Values used for Command Segment Wait command */
#define COMMAND_SEGMENT_WAIT_MASK_V2                                                     0xF8
#define COMMAND_SEGMENT_WAIT_RESERVED_MASK_V2                                            0x07
/* Values used for Command Subsequent Segment command */
#define COMMAND_SUBSEQUENT_SEGMENT_MASK_V2                                               0xF8
#define COMMAND_SUBSEQUENT_SEGMENT_DATAGRAM_SIZE_1_MASK_V2                               0x07
#define COMMAND_SUBSEQUENT_SEGMENT_PROPERTIES2_DATAGRAM_OFFSET_1_MASK_V2                 0x07
#define COMMAND_SUBSEQUENT_SEGMENT_PROPERTIES2_EXT_BIT_MASK_V2                           0x08
#define COMMAND_SUBSEQUENT_SEGMENT_PROPERTIES2_SESSION_ID_MASK_V2                        0xF0
#define COMMAND_SUBSEQUENT_SEGMENT_PROPERTIES2_SESSION_ID_SHIFT_V2                       0x04

/* Transport Service command class commands */
#define TRANSPORT_SERVICE_VERSION                                                        0x01
#define COMMAND_FIRST_FRAGMENT                                                           0xC0
#define COMMAND_SUBSEQUENT_FRAGMENT                                                      0xE0
/* Values used for Command First Fragment command */
#define COMMAND_FIRST_FRAGMENT_MASK                                                      0xF8
#define COMMAND_FIRST_FRAGMENT_DATAGRAM_SIZE_1_MASK                                      0x07
#define COMMAND_FIRST_FRAGMENT_PROPERTIES2_SEQUENCE_NO_MASK                              0x0F
#define COMMAND_FIRST_FRAGMENT_PROPERTIES2_RESERVED_MASK                                 0xF0
#define COMMAND_FIRST_FRAGMENT_PROPERTIES2_RESERVED_SHIFT                                0x04
/* Values used for Command Subsequent Fragment command */
#define COMMAND_SUBSEQUENT_FRAGMENT_MASK                                                 0xF8
#define COMMAND_SUBSEQUENT_FRAGMENT_DATAGRAM_SIZE_1_MASK                                 0x07
#define COMMAND_SUBSEQUENT_FRAGMENT_PROPERTIES2_DATAGRAM_OFFSET_1_MASK                   0x07
#define COMMAND_SUBSEQUENT_FRAGMENT_PROPERTIES2_SEQUENCE_NO_MASK                         0x78
#define COMMAND_SUBSEQUENT_FRAGMENT_PROPERTIES2_SEQUENCE_NO_SHIFT                        0x03
#define COMMAND_SUBSEQUENT_FRAGMENT_PROPERTIES2_RESERVED_BIT_MASK                        0x80

/* User Code command class commands */
#define USER_CODE_VERSION                                                                0x01
#define USER_CODE_GET                                                                    0x02
#define USER_CODE_REPORT                                                                 0x03
#define USER_CODE_SET                                                                    0x01
#define USERS_NUMBER_GET                                                                 0x04
#define USERS_NUMBER_REPORT                                                              0x05
/* Values used for User Code Report command */
#define USER_CODE_REPORT_AVAILABLE_NOT_SET                                               0x00
#define USER_CODE_REPORT_OCCUPIED                                                        0x01
#define USER_CODE_REPORT_RESERVED_BY_ADMINISTRATOR                                       0x02
#define USER_CODE_REPORT_STATUS_NOT_AVAILABLE                                            0xFE
/* Values used for User Code Set command */
#define USER_CODE_SET_AVAILABLE_NOT_SET                                                  0x00
#define USER_CODE_SET_OCCUPIED                                                           0x01
#define USER_CODE_SET_RESERVED_BY_ADMINISTRATOR                                          0x02
#define USER_CODE_SET_STATUS_NOT_AVAILABLE                                               0xFE

/* Version command class commands */
#define VERSION_VERSION                                                                  0x01
#define VERSION_COMMAND_CLASS_GET                                                        0x13
#define VERSION_COMMAND_CLASS_REPORT                                                     0x14
#define VERSION_GET                                                                      0x11
#define VERSION_REPORT                                                                   0x12

/* Version command class commands */
#define VERSION_VERSION_V2                                                               0x02
#define VERSION_COMMAND_CLASS_GET_V2                                                     0x13
#define VERSION_COMMAND_CLASS_REPORT_V2                                                  0x14
#define VERSION_GET_V2                                                                   0x11
#define VERSION_REPORT_V2                                                                0x12

/* Wake Up command class commands */
#define WAKE_UP_VERSION                                                                  0x01
#define WAKE_UP_INTERVAL_GET                                                             0x05
#define WAKE_UP_INTERVAL_REPORT                                                          0x06
#define WAKE_UP_INTERVAL_SET                                                             0x04
#define WAKE_UP_NO_MORE_INFORMATION                                                      0x08
#define WAKE_UP_NOTIFICATION                                                             0x07

/* Wake Up command class commands */
#define WAKE_UP_VERSION_V2                                                               0x02
#define WAKE_UP_INTERVAL_CAPABILITIES_GET_V2                                             0x09
#define WAKE_UP_INTERVAL_CAPABILITIES_REPORT_V2                                          0x0A
#define WAKE_UP_INTERVAL_GET_V2                                                          0x05
#define WAKE_UP_INTERVAL_REPORT_V2                                                       0x06
#define WAKE_UP_INTERVAL_SET_V2                                                          0x04
#define WAKE_UP_NO_MORE_INFORMATION_V2                                                   0x08
#define WAKE_UP_NOTIFICATION_V2                                                          0x07

/* Zip 6lowpan command class commands */
#define ZIP_6LOWPAN_VERSION                                                              0x01
#define LOWPAN_FIRST_FRAGMENT                                                            0xC0
#define LOWPAN_SUBSEQUENT_FRAGMENT                                                       0xE0
/* Values used for Lowpan First Fragment command */
#define LOWPAN_FIRST_FRAGMENT_MASK                                                       0xF8
#define LOWPAN_FIRST_FRAGMENT_DATAGRAM_SIZE_1_MASK                                       0x07
/* Values used for Lowpan Subsequent Fragment command */
#define LOWPAN_SUBSEQUENT_FRAGMENT_MASK                                                  0xF8
#define LOWPAN_SUBSEQUENT_FRAGMENT_DATAGRAM_SIZE_1_MASK                                  0x07

/* Zip command class commands */
#define ZIP_VERSION                                                                      0x01
#define COMMAND_ZIP_PACKET                                                               0x02
#define COMMAND_ZIP_KEEP_ALIVE                                                           0x03
/* Values used for Command Zip Packet command */
#define COMMAND_ZIP_PACKET_PROPERTIES1_RESERVED1_MASK                                    0x03
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_OPTION_ERROR_BIT_MASK                        0x04
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_QUEUE_FULL_BIT_MASK                          0x08
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_WAITING_BIT_MASK                             0x10
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_RESPONSE_BIT_MASK                            0x20
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_RESPONSE_BIT_MASK                             0x40
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_REQUEST_BIT_MASK                              0x80
#define COMMAND_ZIP_PACKET_PROPERTIES2_RESERVED2_MASK                                    0x1F
#define COMMAND_ZIP_PACKET_PROPERTIES2_MORE_INFORMATION_BIT_MASK                         0x20
#define COMMAND_ZIP_PACKET_PROPERTIES2_Z_WAVE_CMD_INCLUDED_BIT_MASK                      0x40
#define COMMAND_ZIP_PACKET_PROPERTIES2_HEADER_EXT_INCLUDED_BIT_MASK                      0x80
#define COMMAND_ZIP_PACKET_PROPERTIES3_SOURCE_END_POINT_MASK                             0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES3_RESERVED3_BIT_MASK                                0x80
#define COMMAND_ZIP_PACKET_PROPERTIES4_DESTINATION_END_POINT_MASK                        0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES4_BIT_ADDRESS_BIT_MASK                              0x80

/* Zip command class commands */
#define ZIP_VERSION_V2                                                                   0x02
#define COMMAND_ZIP_PACKET_V2                                                            0x02
/* Values used for Command Zip Packet command */
#define COMMAND_ZIP_PACKET_PROPERTIES1_RESERVED1_MASK_V2                                 0x03
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_OPTION_ERROR_BIT_MASK_V2                     0x04
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_QUEUE_FULL_BIT_MASK_V2                       0x08
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_WAITING_BIT_MASK_V2                          0x10
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_RESPONSE_BIT_MASK_V2                         0x20
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_RESPONSE_BIT_MASK_V2                          0x40
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_REQUEST_BIT_MASK_V2                           0x80
#define COMMAND_ZIP_PACKET_PROPERTIES2_RESERVED2_MASK_V2                                 0x0F
#define COMMAND_ZIP_PACKET_PROPERTIES2_SECURE_ORIGIN_BIT_MASK_V2                         0x10
#define COMMAND_ZIP_PACKET_PROPERTIES2_MORE_INFORMATION_BIT_MASK_V2                      0x20
#define COMMAND_ZIP_PACKET_PROPERTIES2_Z_WAVE_CMD_INCLUDED_BIT_MASK_V2                   0x40
#define COMMAND_ZIP_PACKET_PROPERTIES2_HEADER_EXT_INCLUDED_BIT_MASK_V2                   0x80
#define COMMAND_ZIP_PACKET_PROPERTIES3_SOURCE_END_POINT_MASK_V2                          0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES3_RESERVED3_BIT_MASK_V2                             0x80
#define COMMAND_ZIP_PACKET_PROPERTIES4_DESTINATION_END_POINT_MASK_V2                     0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES4_BIT_ADDRESS_BIT_MASK_V2                           0x80

/* Zip command class commands */
#define ZIP_VERSION_V3                                                                   0x03
#define COMMAND_ZIP_PACKET_V3                                                            0x02
/* Values used for Command Zip Packet command */
#define COMMAND_ZIP_PACKET_PROPERTIES1_RESERVED1_MASK_V3                                 0x03
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_OPTION_ERROR_BIT_MASK_V3                     0x04
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_QUEUE_FULL_BIT_MASK_V3                       0x08
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_WAITING_BIT_MASK_V3                          0x10
#define COMMAND_ZIP_PACKET_PROPERTIES1_NACK_RESPONSE_BIT_MASK_V3                         0x20
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_RESPONSE_BIT_MASK_V3                          0x40
#define COMMAND_ZIP_PACKET_PROPERTIES1_ACK_REQUEST_BIT_MASK_V3                           0x80
#define COMMAND_ZIP_PACKET_PROPERTIES2_RESERVED2_MASK_V3                                 0x0F
#define COMMAND_ZIP_PACKET_PROPERTIES2_SECURE_ORIGIN_BIT_MASK_V3                         0x10
#define COMMAND_ZIP_PACKET_PROPERTIES2_MORE_INFORMATION_BIT_MASK_V3                      0x20
#define COMMAND_ZIP_PACKET_PROPERTIES2_Z_WAVE_CMD_INCLUDED_BIT_MASK_V3                   0x40
#define COMMAND_ZIP_PACKET_PROPERTIES2_HEADER_EXT_INCLUDED_BIT_MASK_V3                   0x80
#define COMMAND_ZIP_PACKET_PROPERTIES3_SOURCE_END_POINT_MASK_V3                          0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES3_RESERVED3_BIT_MASK_V3                             0x80
#define COMMAND_ZIP_PACKET_PROPERTIES4_DESTINATION_END_POINT_MASK_V3                     0x7F
#define COMMAND_ZIP_PACKET_PROPERTIES4_BIT_ADDRESS_BIT_MASK_V3                           0x80

/* Application Capability command class commands */
#define APPLICATION_CAPABILITY_VERSION                                                   0x01
#define COMMAND_COMMAND_CLASS_NOT_SUPPORTED                                              0x01
/* Values used for Command Command Class Not Supported command */
#define COMMAND_COMMAND_CLASS_NOT_SUPPORTED_PROPERTIES1_RESERVED_MASK                    0x7F
#define COMMAND_COMMAND_CLASS_NOT_SUPPORTED_PROPERTIES1_DYNAMIC_BIT_MASK                 0x80

/* Switch Color command class commands */
#define SWITCH_COLOR_VERSION                                                             0x01
#define SWITCH_COLOR_SUPPORTED_GET                                                       0x01
#define SWITCH_COLOR_SUPPORTED_REPORT                                                    0x02
#define SWITCH_COLOR_GET                                                                 0x03
#define SWITCH_COLOR_REPORT                                                              0x04
#define SWITCH_COLOR_SET                                                                 0x05
#define SWITCH_COLOR_START_LEVEL_CHANGE                                                  0x06
#define SWITCH_COLOR_STOP_LEVEL_CHANGE                                                   0x07
/* Values used for Switch Color Set command */
#define SWITCH_COLOR_SET_PROPERTIES1_COLOR_COMPONENT_COUNT_MASK                          0x1F
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_MASK                                       0xE0
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_SHIFT                                      0x05
/* Values used for Switch Color Start Level Change command */
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES1_MASK                            0x1F
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_STATE_BIT_MASK          0x20
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_BIT_MASK                     0x40
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES2_BIT_MASK                        0x80

/* Switch Color command class commands */
#define SWITCH_COLOR_VERSION_V2                                                          0x02
#define SWITCH_COLOR_SUPPORTED_GET_V2                                                    0x01
#define SWITCH_COLOR_SUPPORTED_REPORT_V2                                                 0x02
#define SWITCH_COLOR_GET_V2                                                              0x03
#define SWITCH_COLOR_REPORT_V2                                                           0x04
#define SWITCH_COLOR_SET_V2                                                              0x05
#define SWITCH_COLOR_START_LEVEL_CHANGE_V2                                               0x06
#define SWITCH_COLOR_STOP_LEVEL_CHANGE_V2                                                0x07
/* Values used for Switch Color Set command */
#define SWITCH_COLOR_SET_PROPERTIES1_COLOR_COMPONENT_COUNT_MASK_V2                       0x1F
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_MASK_V2                                    0xE0
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_SHIFT_V2                                   0x05
/* Values used for Switch Color Start Level Change command */
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES1_MASK_V2                         0x1F
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_STATE_BIT_MASK_V2       0x20
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_BIT_MASK_V2                  0x40
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES2_BIT_MASK_V2                     0x80

/* Switch Color command class commands */
#define SWITCH_COLOR_VERSION_V3                                                          0x03
#define SWITCH_COLOR_SUPPORTED_GET_V3                                                    0x01
#define SWITCH_COLOR_SUPPORTED_REPORT_V3                                                 0x02
#define SWITCH_COLOR_GET_V3                                                              0x03
#define SWITCH_COLOR_REPORT_V3                                                           0x04
#define SWITCH_COLOR_SET_V3                                                              0x05
#define SWITCH_COLOR_START_LEVEL_CHANGE_V3                                               0x06
#define SWITCH_COLOR_STOP_LEVEL_CHANGE_V3                                                0x07
/* Values used for Switch Color Report command */
#define SWITCH_COLOR_REPORT_ALREADY_AT_THE_TARGET_VALUE_V3                               0x00
#define SWITCH_COLOR_REPORT_UNKNOWN_DURATION_V3                                          0xFE
#define SWITCH_COLOR_REPORT_RESERVED_V3                                                  0xFF
/* Values used for Switch Color Set command */
#define SWITCH_COLOR_SET_PROPERTIES1_COLOR_COMPONENT_COUNT_MASK_V3                       0x1F
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_MASK_V3                                    0xE0
#define SWITCH_COLOR_SET_PROPERTIES1_RESERVED_SHIFT_V3                                   0x05
#define SWITCH_COLOR_SET_INSTANTLY_V3                                                    0x00
#define SWITCH_COLOR_SET_DEFAULT_V3                                                      0xFF
/* Values used for Switch Color Start Level Change command */
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES1_MASK_V3                         0x1F
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_IGNORE_START_STATE_BIT_MASK_V3       0x20
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_BIT_MASK_V3                  0x40
#define SWITCH_COLOR_START_LEVEL_CHANGE_PROPERTIES1_RES2_BIT_MASK_V3                     0x80
#define SWITCH_COLOR_START_LEVEL_CHANGE_INSTANTLY_V3                                     0x00
#define SWITCH_COLOR_START_LEVEL_CHANGE_DEFAULT_V3                                       0xFF

/* Schedule command class commands */
#define SCHEDULE_VERSION                                                                 0x01
#define SCHEDULE_SUPPORTED_GET                                                           0x01
#define SCHEDULE_SUPPORTED_REPORT                                                        0x02
#define COMMAND_SCHEDULE_SET                                                             0x03
#define COMMAND_SCHEDULE_GET                                                             0x04
#define COMMAND_SCHEDULE_REPORT                                                          0x05
#define SCHEDULE_REMOVE                                                                  0x06
#define SCHEDULE_STATE_SET                                                               0x07
#define SCHEDULE_STATE_GET                                                               0x08
#define SCHEDULE_STATE_REPORT                                                            0x09
/* Values used for Schedule Supported Report command */
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_START_TIME_SUPPORT_MASK                    0x3F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_FALLBACK_SUPPORT_BIT_MASK                  0x40
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_SUPPORT_ENABLE_DISABLE_BIT_MASK            0x80
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_SUPPORTED_OVERRIDE_TYPES_MASK              0x7F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_OVERRIDE_SUPPORT_BIT_MASK                  0x80
/* Values used for Command Schedule Set command */
#define COMMAND_SCHEDULE_SET_PROPERTIES1_START_MONTH_MASK                                0x0F
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RESERVED1_MASK                                  0xF0
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RESERVED1_SHIFT                                 0x04
#define COMMAND_SCHEDULE_SET_PROPERTIES2_START_DAY_OF_MONTH_MASK                         0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RESERVED2_MASK                                  0xE0
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RESERVED2_SHIFT                                 0x05
#define COMMAND_SCHEDULE_SET_PROPERTIES3_START_WEEKDAY_MASK                              0x7F
#define COMMAND_SCHEDULE_SET_PROPERTIES3_RES_BIT_MASK                                    0x80
#define COMMAND_SCHEDULE_SET_PROPERTIES4_START_HOUR_MASK                                 0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_MASK                              0xE0
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_SHIFT                             0x05
#define COMMAND_SCHEDULE_SET_PROPERTIES5_START_MINUTE_MASK                               0x3F
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RESERVED3_MASK                                  0xC0
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RESERVED3_SHIFT                                 0x06
/* Values used for Command Schedule Report command */
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_START_MONTH_MASK                             0x0F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_ACTIVE_ID_MASK                               0xF0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_ACTIVE_ID_SHIFT                              0x04
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_START_DAY_OF_MONTH_MASK                      0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RESERVED2_MASK                               0xE0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RESERVED2_SHIFT                              0x05
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_START_WEEKDAY_MASK                           0x7F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_RES_BIT_MASK                                 0x80
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_START_HOUR_MASK                              0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_MASK                           0xE0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_SHIFT                          0x05
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_START_MINUTE_MASK                            0x3F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RESERVED3_MASK                               0xC0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RESERVED3_SHIFT                              0x06
/* Values used for Schedule State Report command */
#define SCHEDULE_STATE_REPORT_PROPERTIES1_OVERRIDE_BIT_MASK                              0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_MASK                         0xFE
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_SHIFT                        0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_1_MASK                               0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_MASK                               0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_SHIFT                              0x04
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_3_MASK                               0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_MASK                               0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_SHIFT                              0x04

/* Schedule command class commands */
#define SCHEDULE_VERSION_V2                                                              0x02
#define SCHEDULE_SUPPORTED_GET_V2                                                        0x01
#define SCHEDULE_SUPPORTED_REPORT_V2                                                     0x02
#define COMMAND_SCHEDULE_SET_V2                                                          0x03
#define COMMAND_SCHEDULE_GET_V2                                                          0x04
#define COMMAND_SCHEDULE_REPORT_V2                                                       0x05
#define SCHEDULE_REMOVE_V2                                                               0x06
#define SCHEDULE_STATE_SET_V2                                                            0x07
#define SCHEDULE_STATE_GET_V2                                                            0x08
#define SCHEDULE_STATE_REPORT_V2                                                         0x09
/* Values used for Schedule Supported Report command */
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_START_TIME_SUPPORT_MASK_V2                 0x3F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_FALLBACK_SUPPORT_BIT_MASK_V2               0x40
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_SUPPORT_ENABLE_DISABLE_BIT_MASK_V2         0x80
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_SUPPORTED_OVERRIDE_TYPES_MASK_V2           0x7F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_OVERRIDE_SUPPORT_BIT_MASK_V2               0x80
/* Values used for Command Schedule Set command */
#define COMMAND_SCHEDULE_SET_PROPERTIES1_START_MONTH_MASK_V2                             0x0F
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RESERVED0_MASK_V2                               0xF0
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RESERVED0_SHIFT_V2                              0x04
#define COMMAND_SCHEDULE_SET_PROPERTIES2_START_DAY_OF_MONTH_MASK_V2                      0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RESERVED1_MASK_V2                               0xE0
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RESERVED1_SHIFT_V2                              0x05
#define COMMAND_SCHEDULE_SET_PROPERTIES3_START_WEEKDAY_MASK_V2                           0x7F
#define COMMAND_SCHEDULE_SET_PROPERTIES3_RESERVED2_BIT_MASK_V2                           0x80
#define COMMAND_SCHEDULE_SET_PROPERTIES4_START_HOUR_MASK_V2                              0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_MASK_V2                           0xE0
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_SHIFT_V2                          0x05
#define COMMAND_SCHEDULE_SET_PROPERTIES5_START_MINUTE_MASK_V2                            0x3F
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RESERVED3_MASK_V2                               0xC0
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RESERVED3_SHIFT_V2                              0x06
/* Values used for Command Schedule Report command */
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_START_MONTH_MASK_V2                          0x0F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_ACTIVE_ID_MASK_V2                            0xF0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_ACTIVE_ID_SHIFT_V2                           0x04
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_START_DAY_OF_MONTH_MASK_V2                   0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RESERVED0_MASK_V2                            0xE0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RESERVED0_SHIFT_V2                           0x05
#define COMMAND_SCHEDULE_REPORT_RESERVED0_REPEAT_EVERY_N_HOURS_V2                        0x00
#define COMMAND_SCHEDULE_REPORT_RESERVED0_REPEAT_EVERY_N_DAYS_V2                         0x01
#define COMMAND_SCHEDULE_REPORT_RESERVED0_REPEAT_EVERY_N_WEEKS_V2                        0x02
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_START_WEEKDAY_MASK_V2                        0x7F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_RESERVED1_BIT_MASK_V2                        0x80
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_START_HOUR_MASK_V2                           0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_MASK_V2                        0xE0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_SHIFT_V2                       0x05
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_START_MINUTE_MASK_V2                         0x3F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RESERVED2_MASK_V2                            0xC0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RESERVED2_SHIFT_V2                           0x06
/* Values used for Schedule State Report command */
#define SCHEDULE_STATE_REPORT_PROPERTIES1_OVERRIDE_BIT_MASK_V2                           0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_MASK_V2                      0xFE
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_SHIFT_V2                     0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_1_MASK_V2                            0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_MASK_V2                            0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_SHIFT_V2                           0x04
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_3_MASK_V2                            0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_MASK_V2                            0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_SHIFT_V2                           0x04

/* Schedule command class commands */
#define SCHEDULE_VERSION_V3                                                              0x03
#define SCHEDULE_SUPPORTED_GET_V3                                                        0x01
#define SCHEDULE_SUPPORTED_REPORT_V3                                                     0x02
#define COMMAND_SCHEDULE_SET_V3                                                          0x03
#define COMMAND_SCHEDULE_GET_V3                                                          0x04
#define COMMAND_SCHEDULE_REPORT_V3                                                       0x05
#define SCHEDULE_REMOVE_V3                                                               0x06
#define SCHEDULE_STATE_SET_V3                                                            0x07
#define SCHEDULE_STATE_GET_V3                                                            0x08
#define SCHEDULE_STATE_REPORT_V3                                                         0x09
/* Values used for Schedule Supported Report command */
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_START_TIME_SUPPORT_MASK_V3                 0x3F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_FALLBACK_SUPPORT_BIT_MASK_V3               0x40
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES1_SUPPORT_ENABLE_DISABLE_BIT_MASK_V3         0x80
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_SUPPORTED_OVERRIDE_TYPES_MASK_V3           0x7F
#define SCHEDULE_SUPPORTED_REPORT_PROPERTIES3_OVERRIDE_SUPPORT_BIT_MASK_V3               0x80
/* Values used for Command Schedule Set command */
#define COMMAND_SCHEDULE_SET_PROPERTIES1_START_MONTH_MASK_V3                             0x0F
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RECURRENCE_OFFSET_MASK_V3                       0xF0
#define COMMAND_SCHEDULE_SET_PROPERTIES1_RECURRENCE_OFFSET_SHIFT_V3                      0x04
#define COMMAND_SCHEDULE_SET_PROPERTIES2_START_DAY_OF_MONTH_MASK_V3                      0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RECURRENCE_MODE_MASK_V3                         0x60
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RECURRENCE_MODE_SHIFT_V3                        0x05
#define COMMAND_SCHEDULE_SET_RECURRENCE_MODE_REPEAT_EVERY_N_HOURS_V3                     0x00
#define COMMAND_SCHEDULE_SET_RECURRENCE_MODE_REPEAT_EVERY_N_DAYS_V3                      0x01
#define COMMAND_SCHEDULE_SET_RECURRENCE_MODE_REPEAT_EVERY_N_WEEKS_V3                     0x02
#define COMMAND_SCHEDULE_SET_PROPERTIES2_RESERVED1_BIT_MASK_V3                           0x80
#define COMMAND_SCHEDULE_SET_PROPERTIES3_START_WEEKDAY_MASK_V3                           0x7F
#define COMMAND_SCHEDULE_SET_PROPERTIES3_RESERVED2_BIT_MASK_V3                           0x80
#define COMMAND_SCHEDULE_SET_PROPERTIES4_START_HOUR_MASK_V3                              0x1F
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_MASK_V3                           0xE0
#define COMMAND_SCHEDULE_SET_PROPERTIES4_DURATION_TYPE_SHIFT_V3                          0x05
#define COMMAND_SCHEDULE_SET_PROPERTIES5_START_MINUTE_MASK_V3                            0x3F
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RELATIVE_BIT_MASK_V3                            0x40
#define COMMAND_SCHEDULE_SET_PROPERTIES5_RESERVED3_BIT_MASK_V3                           0x80
/* Values used for Command Schedule Get command */
#define COMMAND_SCHEDULE_GET_PROPERTIES1_RESERVED_MASK_V3                                0x7F
#define COMMAND_SCHEDULE_GET_PROPERTIES1_AID_RO_CTL_BIT_MASK_V3                          0x80
/* Values used for Command Schedule Report command */
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_START_MONTH_MASK_V3                          0x0F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_AID_RO_MASK_V3                               0xF0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES1_AID_RO_SHIFT_V3                              0x04
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_START_DAY_OF_MONTH_MASK_V3                   0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RECURRENCE_MODE_MASK_V3                      0x60
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_RECURRENCE_MODE_SHIFT_V3                     0x05
#define COMMAND_SCHEDULE_REPORT_RECURRENCE_MODE_REPEAT_EVERY_N_HOURS_V3                  0x00
#define COMMAND_SCHEDULE_REPORT_RECURRENCE_MODE_REPEAT_EVERY_N_DAYS_V3                   0x01
#define COMMAND_SCHEDULE_REPORT_RECURRENCE_MODE_REPEAT_EVERY_N_WEEKS_V3                  0x02
#define COMMAND_SCHEDULE_REPORT_PROPERTIES2_AID_RO_CTL_BIT_MASK_V3                       0x80
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_START_WEEKDAY_MASK_V3                        0x7F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES3_RESERVED1_BIT_MASK_V3                        0x80
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_START_HOUR_MASK_V3                           0x1F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_MASK_V3                        0xE0
#define COMMAND_SCHEDULE_REPORT_PROPERTIES4_DURATION_TYPE_SHIFT_V3                       0x05
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_START_MINUTE_MASK_V3                         0x3F
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RELATIVE_BIT_MASK_V3                         0x40
#define COMMAND_SCHEDULE_REPORT_PROPERTIES5_RESERVED2_BIT_MASK_V3                        0x80
/* Values used for Schedule State Report command */
#define SCHEDULE_STATE_REPORT_PROPERTIES1_OVERRIDE_BIT_MASK_V3                           0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_MASK_V3                      0xFE
#define SCHEDULE_STATE_REPORT_PROPERTIES1_REPORTS_TO_FOLLOW_SHIFT_V3                     0x01
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_1_MASK_V3                            0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_MASK_V3                            0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES2_ACTIVE_ID_2_SHIFT_V3                           0x04
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_3_MASK_V3                            0x0F
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_MASK_V3                            0xF0
#define SCHEDULE_STATE_REPORT_PROPERTIES3_ACTIVE_ID_N_SHIFT_V3                           0x04

/* Network Management Primary command class commands */
#define NETWORK_MANAGEMENT_PRIMARY_VERSION                                               0x01
#define CONTROLLER_CHANGE                                                                0x01
#define CONTROLLER_CHANGE_STATUS                                                         0x02
/* Values used for Controller Change Status command */
#define CONTROLLER_CHANGE_STATUS_PROPERTIES1_CAPABILITY_MASK                             0x7F
#define CONTROLLER_CHANGE_STATUS_PROPERTIES1_LISTENING_BIT_MASK                          0x80
#define CONTROLLER_CHANGE_STATUS_PROPERTIES2_SECURITY_MASK                               0x7F
#define CONTROLLER_CHANGE_STATUS_PROPERTIES2_OPT_BIT_MASK                                0x80

/* Zip Nd command class commands */
#define ZIP_ND_VERSION                                                                   0x01
#define ZIP_NODE_SOLICITATION                                                            0x03
#define ZIP_INV_NODE_SOLICITATION                                                        0x04
#define ZIP_NODE_ADVERTISEMENT                                                           0x01
/* Values used for Zip Inv Node Solicitation command */
#define ZIP_INV_NODE_SOLICITATION_PROPERTIES1_RESERVED1_MASK                             0x03
#define ZIP_INV_NODE_SOLICITATION_PROPERTIES1_LOCAL_BIT_MASK                             0x04
#define ZIP_INV_NODE_SOLICITATION_PROPERTIES1_RESERVED2_MASK                             0xF8
#define ZIP_INV_NODE_SOLICITATION_PROPERTIES1_RESERVED2_SHIFT                            0x03
/* Values used for Zip Node Advertisement command */
#define ZIP_NODE_ADVERTISEMENT_PROPERTIES1_VALIDITY_MASK                                 0x03
#define ZIP_NODE_ADVERTISEMENT_VALIDITY_INFORMATION_OK                                   0x00
#define ZIP_NODE_ADVERTISEMENT_VALIDITY_INFORMATION_OBSOLETE                             0x01
#define ZIP_NODE_ADVERTISEMENT_VALIDITY_INFORMATION_NOT_FOUND                            0x02
#define ZIP_NODE_ADVERTISEMENT_PROPERTIES1_LOCAL_BIT_MASK                                0x04
#define ZIP_NODE_ADVERTISEMENT_PROPERTIES1_RESERVED_MASK                                 0xF8
#define ZIP_NODE_ADVERTISEMENT_PROPERTIES1_RESERVED_SHIFT                                0x03

/* Association Grp Info command class commands */
#define ASSOCIATION_GRP_INFO_VERSION                                                     0x01
#define ASSOCIATION_GROUP_NAME_GET                                                       0x01
#define ASSOCIATION_GROUP_NAME_REPORT                                                    0x02
#define ASSOCIATION_GROUP_INFO_GET                                                       0x03
#define ASSOCIATION_GROUP_INFO_REPORT                                                    0x04
#define ASSOCIATION_GROUP_COMMAND_LIST_GET                                               0x05
#define ASSOCIATION_GROUP_COMMAND_LIST_REPORT                                            0x06
/* Values used for Association Group Info Get command */
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_RESERVED_MASK                             0x3F
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_LIST_MODE_BIT_MASK                        0x40
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_REFRESH_CACHE_BIT_MASK                    0x80
/* Values used for Association Group Info Report command */
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_GROUP_COUNT_MASK                       0x3F
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_DYNAMIC_INFO_BIT_MASK                  0x40
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_LIST_MODE_BIT_MASK                     0x80
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL                                    0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL                                    0x20
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_SENSOR                                     0x31
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION                               0x71
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_NA                                 0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE                           0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY01                              0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY02                              0x02
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY03                              0x03
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY04                              0x04
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY05                              0x05
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY06                              0x06
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY07                              0x07
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY08                              0x08
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY09                              0x09
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY10                              0x0A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY11                              0x0B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY12                              0x0C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY13                              0x0D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY14                              0x0E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY15                              0x0F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY16                              0x10
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY17                              0x11
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY18                              0x12
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY19                              0x13
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY20                              0x14
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY21                              0x15
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY22                              0x16
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY23                              0x17
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY24                              0x18
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY25                              0x19
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY26                              0x1A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY27                              0x1B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY28                              0x1C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY29                              0x1D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY30                              0x1E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY31                              0x1F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY32                              0x20
/* Values used for Association Group Command List Get command */
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_RESERVED_MASK                     0x7F
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_ALLOW_CACHE_BIT_MASK              0x80

/* Association Grp Info command class commands */
#define ASSOCIATION_GRP_INFO_VERSION_V2                                                  0x02
#define ASSOCIATION_GROUP_NAME_GET_V2                                                    0x01
#define ASSOCIATION_GROUP_NAME_REPORT_V2                                                 0x02
#define ASSOCIATION_GROUP_INFO_GET_V2                                                    0x03
#define ASSOCIATION_GROUP_INFO_REPORT_V2                                                 0x04
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_V2                                            0x05
#define ASSOCIATION_GROUP_COMMAND_LIST_REPORT_V2                                         0x06
/* Values used for Association Group Info Get command */
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_RESERVED_MASK_V2                          0x3F
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_LIST_MODE_BIT_MASK_V2                     0x40
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_REFRESH_CACHE_BIT_MASK_V2                 0x80
/* Values used for Association Group Info Report command */
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_GROUP_COUNT_MASK_V2                    0x3F
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_DYNAMIC_INFO_BIT_MASK_V2               0x40
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_LIST_MODE_BIT_MASK_V2                  0x80
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_V2                                 0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_V2                                 0x20
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_SENSOR_V2                                  0x31
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_V2                            0x71
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_V2                                   0x32
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_NA_V2                              0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE_V2                        0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY01_V2                           0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY02_V2                           0x02
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY03_V2                           0x03
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY04_V2                           0x04
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY05_V2                           0x05
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY06_V2                           0x06
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY07_V2                           0x07
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY08_V2                           0x08
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY09_V2                           0x09
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY10_V2                           0x0A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY11_V2                           0x0B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY12_V2                           0x0C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY13_V2                           0x0D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY14_V2                           0x0E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY15_V2                           0x0F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY16_V2                           0x10
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY17_V2                           0x11
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY18_V2                           0x12
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY19_V2                           0x13
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY20_V2                           0x14
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY21_V2                           0x15
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY22_V2                           0x16
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY23_V2                           0x17
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY24_V2                           0x18
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY25_V2                           0x19
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY26_V2                           0x1A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY27_V2                           0x1B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY28_V2                           0x1C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY29_V2                           0x1D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY30_V2                           0x1E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY31_V2                           0x1F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY32_V2                           0x20
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_MULTILEVEL_SENSOR_TYPE_TEMPERATURE_V2      0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_MULTILEVEL_SENSOR_TYPE_HUMIDITY_V2         0x05
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_TYPE_SMOKE_V2                 0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_TYPE_CO2_V2                   0x03
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_ELECTRIC_V2                     0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_GAS_V2                          0x02
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_WATER_V2                        0x03
/* Values used for Association Group Command List Get command */
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_RESERVED_MASK_V2                  0x7F
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_ALLOW_CACHE_BIT_MASK_V2           0x80

/* Association Grp Info command class commands */
#define ASSOCIATION_GRP_INFO_VERSION_V3                                                  0x03
#define ASSOCIATION_GROUP_NAME_GET_V3                                                    0x01
#define ASSOCIATION_GROUP_NAME_REPORT_V3                                                 0x02
#define ASSOCIATION_GROUP_INFO_GET_V3                                                    0x03
#define ASSOCIATION_GROUP_INFO_REPORT_V3                                                 0x04
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_V3                                            0x05
#define ASSOCIATION_GROUP_COMMAND_LIST_REPORT_V3                                         0x06
/* Values used for Association Group Info Get command */
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_RESERVED_MASK_V3                          0x3F
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_LIST_MODE_BIT_MASK_V3                     0x40
#define ASSOCIATION_GROUP_INFO_GET_PROPERTIES1_REFRESH_CACHE_BIT_MASK_V3                 0x80
/* Values used for Association Group Info Report command */
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_GROUP_COUNT_MASK_V3                    0x3F
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_DYNAMIC_INFO_BIT_MASK_V3               0x40
#define ASSOCIATION_GROUP_INFO_REPORT_PROPERTIES1_LIST_MODE_BIT_MASK_V3                  0x80
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_V3                                 0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_V3                                 0x20
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_SENSOR_V3                                  0x31
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_V3                            0x71
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_V3                                   0x32
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_IRRIGATION_V3                              0x6B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_NA_V3                              0x00
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_GENERAL_LIFELINE_V3                        0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY01_V3                           0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY02_V3                           0x02
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY03_V3                           0x03
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY04_V3                           0x04
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY05_V3                           0x05
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY06_V3                           0x06
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY07_V3                           0x07
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY08_V3                           0x08
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY09_V3                           0x09
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY10_V3                           0x0A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY11_V3                           0x0B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY12_V3                           0x0C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY13_V3                           0x0D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY14_V3                           0x0E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY15_V3                           0x0F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY16_V3                           0x10
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY17_V3                           0x11
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY18_V3                           0x12
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY19_V3                           0x13
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY20_V3                           0x14
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY21_V3                           0x15
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY22_V3                           0x16
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY23_V3                           0x17
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY24_V3                           0x18
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY25_V3                           0x19
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY26_V3                           0x1A
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY27_V3                           0x1B
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY28_V3                           0x1C
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY29_V3                           0x1D
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY30_V3                           0x1E
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY31_V3                           0x1F
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY32_V3                           0x20
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_MULTILEVEL_SENSOR_TYPE_TEMPERATURE_V3      0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_MULTILEVEL_SENSOR_TYPE_HUMIDITY_V3         0x05
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_TYPE_SMOKE_V3                 0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_NOTIFICATION_TYPE_CO2_V3                   0x03
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_ELECTRIC_V3                     0x01
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_GAS_V3                          0x02
#define ASSOCIATION_GROUP_INFO_REPORT_PROFILE_METER_TYPE_WATER_V3                        0x03
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_01_V3                           0x01
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_02_V3                           0x02
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_03_V3                           0x03
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_04_V3                           0x04
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_05_V3                           0x05
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_06_V3                           0x06
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_07_V3                           0x07
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_08_V3                           0x08
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_09_V3                           0x09
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_10_V3                           0x0A
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_11_V3                           0x0B
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_12_V3                           0x0C
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_13_V3                           0x0D
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_14_V3                           0x0E
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_15_V3                           0x0F
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_16_V3                           0x10
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_17_V3                           0x11
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_18_V3                           0x12
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_19_V3                           0x13
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_20_V3                           0x14
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_21_V3                           0x15
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_22_V3                           0x16
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_23_V3                           0x17
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_24_V3                           0x18
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_25_V3                           0x19
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_26_V3                           0x1A
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_27_V3                           0x1B
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_28_V3                           0x1C
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_29_V3                           0x1D
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_30_V3                           0x1E
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_31_V3                           0x1F
#define ASSOCIATION_GROUP_INFO_REPORT_IRRIGATION_CHANNEL_32_V3                           0x20
/* Values used for Association Group Command List Get command */
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_RESERVED_MASK_V3                  0x7F
#define ASSOCIATION_GROUP_COMMAND_LIST_GET_PROPERTIES1_ALLOW_CACHE_BIT_MASK_V3           0x80

/* Device Reset Locally command class commands */
#define DEVICE_RESET_LOCALLY_VERSION                                                     0x01
#define DEVICE_RESET_LOCALLY_NOTIFICATION                                                0x01

/* Central Scene command class commands */
#define CENTRAL_SCENE_VERSION                                                            0x01
#define CENTRAL_SCENE_SUPPORTED_GET                                                      0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT                                                   0x02
#define CENTRAL_SCENE_NOTIFICATION                                                       0x03
/* Values used for Central Scene Notification command */
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_KEY_ATTRIBUTES_MASK                       0x07
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_MASK                             0xF8
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_SHIFT                            0x03

/* Central Scene command class commands */
#define CENTRAL_SCENE_VERSION_V2                                                         0x02
#define CENTRAL_SCENE_SUPPORTED_GET_V2                                                   0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_V2                                                0x02
#define CENTRAL_SCENE_NOTIFICATION_V2                                                    0x03
/* Values used for Central Scene Supported Report command */
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_IDENTICAL_BIT_MASK_V2                 0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASK_BYTES_MASK_V2      0x06
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASK_BYTES_SHIFT_V2     0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V2                      0xF8
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V2                     0x03
/* Values used for Central Scene Notification command */
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_KEY_ATTRIBUTES_MASK_V2                    0x07
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_1_TIME_V2                  0x00
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_RELEASED_V2                        0x01
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_HELD_DOWN_V2                       0x02
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_2_TIMES_V2                 0x03
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_3_TIMES_V2                 0x04
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_4_TIMES_V2                 0x05
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_5_TIMES_V2                 0x06
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_MASK_V2                          0xF8
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_SHIFT_V2                         0x03

/* Central Scene command class commands */
#define CENTRAL_SCENE_VERSION_V3                                                         0x03
#define CENTRAL_SCENE_SUPPORTED_GET_V3                                                   0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_V3                                                0x02
#define CENTRAL_SCENE_NOTIFICATION_V3                                                    0x03
#define CENTRAL_SCENE_CONFIGURATION_SET_V3                                               0x04
#define CENTRAL_SCENE_CONFIGURATION_GET_V3                                               0x05
#define CENTRAL_SCENE_CONFIGURATION_REPORT_V3                                            0x06
/* Values used for Central Scene Supported Report command */
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_IDENTICAL_BIT_MASK_V3                 0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASK_BYTES_MASK_V3      0x06
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_BIT_MASK_BYTES_SHIFT_V3     0x01
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK_V3                      0x78
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT_V3                     0x03
#define CENTRAL_SCENE_SUPPORTED_REPORT_PROPERTIES1_SLOW_REFRESH_SUPPORT_BIT_MASK_V3      0x80
/* Values used for Central Scene Notification command */
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_KEY_ATTRIBUTES_MASK_V3                    0x07
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_1_TIME_V3                  0x00
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_RELEASED_V3                        0x01
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_HELD_DOWN_V3                       0x02
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_2_TIMES_V3                 0x03
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_3_TIMES_V3                 0x04
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_4_TIMES_V3                 0x05
#define CENTRAL_SCENE_NOTIFICATION_KEY_ATTRIBUTES_KEY_PRESSED_5_TIMES_V3                 0x06
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_MASK_V3                          0x78
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_RESERVED_SHIFT_V3                         0x03
#define CENTRAL_SCENE_NOTIFICATION_PROPERTIES1_SLOW_REFRESH_BIT_MASK_V3                  0x80
/* Values used for Central Scene Configuration Set command */
#define CENTRAL_SCENE_CONFIGURATION_SET_PROPERTIES1_RESERVED_MASK_V3                     0x7F
#define CENTRAL_SCENE_CONFIGURATION_SET_PROPERTIES1_SLOW_REFRESH_BIT_MASK_V3             0x80
/* Values used for Central Scene Configuration Report command */
#define CENTRAL_SCENE_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_MASK_V3                  0x7F
#define CENTRAL_SCENE_CONFIGURATION_REPORT_PROPERTIES1_SLOW_REFRESH_BIT_MASK_V3          0x80

/* Ip Association command class commands */
#define IP_ASSOCIATION_VERSION                                                           0x01
#define IP_ASSOCIATION_SET                                                               0x01
#define IP_ASSOCIATION_GET                                                               0x02
#define IP_ASSOCIATION_REPORT                                                            0x03
#define IP_ASSOCIATION_REMOVE                                                            0x04
/* Values used for Ip Association Set command */
#define IP_ASSOCIATION_SET_PROPERTIES1_RESOURCE_NAME_LENGTH_MASK                         0x3F
#define IP_ASSOCIATION_SET_PROPERTIES1_RESERVED_MASK                                     0xC0
#define IP_ASSOCIATION_SET_PROPERTIES1_RESERVED_SHIFT                                    0x06
/* Values used for Ip Association Report command */
#define IP_ASSOCIATION_REPORT_PROPERTIES1_RESOURCE_NAME_LENGTH_MASK                      0x3F
#define IP_ASSOCIATION_REPORT_PROPERTIES1_RESERVED_MASK                                  0xC0
#define IP_ASSOCIATION_REPORT_PROPERTIES1_RESERVED_SHIFT                                 0x06
/* Values used for Ip Association Remove command */
#define IP_ASSOCIATION_REMOVE_PROPERTIES1_RESOURCE_NAME_LENGTH_MASK                      0x3F
#define IP_ASSOCIATION_REMOVE_PROPERTIES1_RESERVED_MASK                                  0xC0
#define IP_ASSOCIATION_REMOVE_PROPERTIES1_RESERVED_SHIFT                                 0x06

/* Antitheft command class commands */
#define ANTITHEFT_VERSION                                                                0x01
#define ANTITHEFT_SET                                                                    0x01
#define ANTITHEFT_GET                                                                    0x02
#define ANTITHEFT_REPORT                                                                 0x03
/* Values used for Antitheft Set command */
#define ANTITHEFT_SET_PROPERTIES1_NUMBER_OF_MAGIC_CODE_BYTES_MASK                        0x7F
#define ANTITHEFT_SET_PROPERTIES1_ENABLE_BIT_MASK                                        0x80

/* Antitheft command class commands */
#define ANTITHEFT_VERSION_V2                                                             0x02
#define ANTITHEFT_SET_V2                                                                 0x01
#define ANTITHEFT_GET_V2                                                                 0x02
#define ANTITHEFT_REPORT_V2                                                              0x03
/* Values used for Antitheft Set command */
#define ANTITHEFT_SET_PROPERTIES1_NUMBER_OF_MAGIC_CODE_BYTES_MASK_V2                     0x7F
#define ANTITHEFT_SET_PROPERTIES1_ENABLE_BIT_MASK_V2                                     0x80

/* Zwaveplus Info command class commands */
#define ZWAVEPLUS_INFO_VERSION                                                           0x01
#define ZWAVEPLUS_INFO_GET                                                               0x01
#define ZWAVEPLUS_INFO_REPORT                                                            0x02
/* Values used for Zwaveplus Info Report command */
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_CENTRAL_STATIC                        0x00
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_SUB_STATIC                            0x01
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_PORTABLE                              0x02
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_PORTABLE_REPORTING                    0x03
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_PORTABLE                                   0x04
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_ALWAYS_ON                                  0x05
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_SLEEPING_REPORTING                         0x06
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_SLEEPING_LISTENING                         0x07
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_NODE                                   0x00
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_FOR_IP_ROUTER                          0x01
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_FOR_IP_GATEWAY                         0x02
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_FOR_IP_CLIENT_IP_NODE                  0x03
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_FOR_IP_CLIENT_ZWAVE_NODE               0x04

/* Zwaveplus Info command class commands */
#define ZWAVEPLUS_INFO_VERSION_V2                                                        0x02
#define ZWAVEPLUS_INFO_GET_V2                                                            0x01
#define ZWAVEPLUS_INFO_REPORT_V2                                                         0x02
/* Values used for Zwaveplus Info Report command */
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_CENTRAL_STATIC_V2                     0x00
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_SUB_STATIC_V2                         0x01
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_PORTABLE_V2                           0x02
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_CONTROLLER_PORTABLE_REPORTING_V2                 0x03
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_PORTABLE_V2                                0x04
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_ALWAYS_ON_V2                               0x05
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_SLEEPING_REPORTING_V2                      0x06
#define ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_SLEEPING_LISTENING_V2                      0x07
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_NODE_V2                                0x00
#define ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_FOR_IP_GATEWAY_V2                      0x02

/* Provisioning List command class commands */
#define COMMAND_PROVISION_SET         0x01
#define COMMAND_PROVISION_DELETE      0x02
#define COMMAND_PROVISION_ITER_GET    0x03
#define COMMAND_PROVISION_ITER_REPORT 0x04
#define COMMAND_PROVISION_GET         0x05
#define COMMAND_PROVISION_REPORT      0x06

/* Zip Gateway command class commands */
#define ZIP_GATEWAY_VERSION                                                              0x01
#define GATEWAY_MODE_SET                                                                 0x01
#define GATEWAY_MODE_GET                                                                 0x02
#define GATEWAY_MODE_REPORT                                                              0x03
#define GATEWAY_PEER_SET                                                                 0x04
#define GATEWAY_PEER_GET                                                                 0x05
#define GATEWAY_PEER_REPORT                                                              0x06
#define GATEWAY_LOCK_SET                                                                 0x07
#define UNSOLICITED_DESTINATION_SET                                                      0x08
#define UNSOLICITED_DESTINATION_GET                                                      0x09
#define UNSOLICITED_DESTINATION_REPORT                                                   0x0A
#define COMMAND_APPLICATION_NODE_INFO_SET                                                0x0B
#define COMMAND_APPLICATION_NODE_INFO_GET                                                0x0C
#define COMMAND_APPLICATION_NODE_INFO_REPORT                                             0x0D
#define GATEWAY_WIFI_CONFIG_SET                                                                 0x0E
#define GATEWAY_WIFI_CONFIG_GET                                                                 0x0F
#define GATEWAY_WIFI_CONFIG_REPORT                                                              0x10
#define GATEWAY_WIFI_ENCRYPT_SUPPORTED_GET                                                      0x11
#define GATEWAY_WIFI_ENCRYPT_SUPPORTED_REPORT                                                   0x12
/* Values used for Gateway Mode Set command */
#define GATEWAY_MODE_SET_STAND_ALONE                                                     0x01
#define GATEWAY_MODE_SET_PORTAL                                                          0x02
/* Values used for Gateway Mode Report command */
#define GATEWAY_MODE_REPORT_STAND_ALONE                                                  0x01
#define GATEWAY_MODE_REPORT_PORTAL                                                       0x02
/* Values used for Gateway Peer Set command */
#define GATEWAY_PEER_SET_PROPERTIES1_PEER_NAME_LENGTH_MASK                               0x3F
#define GATEWAY_PEER_SET_PROPERTIES1_RESERVED_MASK                                       0xC0
#define GATEWAY_PEER_SET_PROPERTIES1_RESERVED_SHIFT                                      0x06
/* Values used for Gateway Peer Report command */
#define GATEWAY_PEER_REPORT_PROPERTIES1_PEER_NAME_LENGTH_MASK                            0x3F
#define GATEWAY_PEER_REPORT_PROPERTIES1_RESERVED_MASK                                    0xC0
#define GATEWAY_PEER_REPORT_PROPERTIES1_RESERVED_SHIFT                                   0x06
/* Values used for Gateway Lock Set command */
#define GATEWAY_LOCK_SET_PROPERTIES1_LOCK_BIT_MASK                                       0x01
#define GATEWAY_LOCK_SET_PROPERTIES1_SHOW_BIT_MASK                                       0x02
#define GATEWAY_LOCK_SET_PROPERTIES1_RESERVED_MASK                                       0xFC
#define GATEWAY_LOCK_SET_PROPERTIES1_RESERVED_SHIFT                                      0x02
/* Values used for Command Application Node Info Set command */
#define COMMAND_APPLICATION_NODE_INFO_SET_SECURITY_SCHEME_0_MARK                         0xF100
/* Values used for Command Application Node Info Report command */
#define COMMAND_APPLICATION_NODE_INFO_REPORT_SECURITY_SCHEME_0_MARK                      0xF100

/* Zip Portal command class commands */
#define ZIP_PORTAL_VERSION                                                               0x01
#define GATEWAY_CONFIGURATION_SET                                                        0x01
#define GATEWAY_CONFIGURATION_STATUS                                                     0x02
#define GATEWAY_CONFIGURATION_GET                                                        0x03
#define GATEWAY_CONFIGURATION_REPORT                                                     0x04
#define GATEWAY_CONFIGURATION_UNREGISTER                                                 0x05   //Testing


/* Dmx command class commands */
#define DMX_VERSION                                                                      0x01
#define DMX_ADDRESS_SET                                                                  0x01
#define DMX_ADDRESS_GET                                                                  0x02
#define DMX_ADDRESS_REPORT                                                               0x03
#define DMX_CAPABILITY_GET                                                               0x04
#define DMX_CAPABILITY_REPORT                                                            0x05
#define DMX_DATA                                                                         0x06
/* Values used for Dmx Address Set command */
#define DMX_ADDRESS_SET_PROPERTIES1_PAGE_ID_MASK                                         0x0F
#define DMX_ADDRESS_SET_PROPERTIES1_RESERVED_MASK                                        0xF0
#define DMX_ADDRESS_SET_PROPERTIES1_RESERVED_SHIFT                                       0x04
/* Values used for Dmx Address Report command */
#define DMX_ADDRESS_REPORT_PROPERTIES1_PAGE_ID_MASK                                      0x0F
#define DMX_ADDRESS_REPORT_PROPERTIES1_RESERVED_MASK                                     0xF0
#define DMX_ADDRESS_REPORT_PROPERTIES1_RESERVED_SHIFT                                    0x04
/* Values used for Dmx Data command */
#define DMX_DATA_PROPERTIES1_PAGE_MASK                                                   0x0F
#define DMX_DATA_PROPERTIES1_SEQUENCE_NO_MASK                                            0x30
#define DMX_DATA_PROPERTIES1_SEQUENCE_NO_SHIFT                                           0x04
#define DMX_DATA_PROPERTIES1_RESERVED_MASK                                               0xC0
#define DMX_DATA_PROPERTIES1_RESERVED_SHIFT                                              0x06

/* Barrier Operator command class commands */
#define BARRIER_OPERATOR_VERSION                                                         0x01
#define BARRIER_OPERATOR_SET                                                             0x01
#define BARRIER_OPERATOR_GET                                                             0x02
#define BARRIER_OPERATOR_REPORT                                                          0x03
#define BARRIER_OPERATOR_SIGNAL_SUPPORTED_GET                                            0x04
#define BARRIER_OPERATOR_SIGNAL_SUPPORTED_REPORT                                         0x05
#define BARRIER_OPERATOR_SIGNAL_SET                                                      0x06
#define BARRIER_OPERATOR_SIGNAL_GET                                                      0x07
#define BARRIER_OPERATOR_SIGNAL_REPORT                                                   0x08
/* Values used for Barrier Operator Set command */
#define BARRIER_OPERATOR_SET_CLOSE                                                       0x00
#define BARRIER_OPERATOR_SET_OPEN                                                        0xFF
/* Values used for Barrier Operator Report command */
#define BARRIER_OPERATOR_REPORT_CLOSED                                                   0x00
#define BARRIER_OPERATOR_REPORT_CLOSING                                                  0xFC
#define BARRIER_OPERATOR_REPORT_STOPPED                                                  0xFD
#define BARRIER_OPERATOR_REPORT_OPENING                                                  0xFE
#define BARRIER_OPERATOR_REPORT_OPEN                                                     0xFF
/* Values used for Barrier Operator Signal Set command */
#define BARRIER_OPERATOR_SIGNAL_SET_OFF                                                  0x00
#define BARRIER_OPERATOR_SIGNAL_SET_ON                                                   0xFF
/* Values used for Barrier Operator Signal Report command */
#define BARRIER_OPERATOR_SIGNAL_REPORT_OFF                                               0x00
#define BARRIER_OPERATOR_SIGNAL_REPORT_ON                                                0xFF

/* Network Management Installation Maintenance command class commands */
#define NETWORK_MANAGEMENT_INSTALLATION_MAINTENANCE_VERSION                              0x01
#define LAST_WORKING_ROUTE_SET                                                           0x01
#define LAST_WORKING_ROUTE_GET                                                           0x02
#define LAST_WORKING_ROUTE_REPORT                                                        0x03
#define STATISTICS_GET                                                                   0x04
#define STATISTICS_REPORT                                                                0x05
#define STATISTICS_CLEAR                                                                 0x06
#define RSSI_GET                                                                         0x07
#define RSSI_REPORT                                                                      0x08
/* Values used for Last Working Route Set command */
#define LAST_WORKING_ROUTE_SET_9_6_KBIT_SEC                                              0x01
#define LAST_WORKING_ROUTE_SET_40_KBIT_SEC                                               0x02
#define LAST_WORKING_ROUTE_SET_100_KBIT_SEC                                              0x03
/* Values used for Last Working Route Report command */
#define LAST_WORKING_ROUTE_REPORT_9_6_KBIT_SEC                                           0x01
#define LAST_WORKING_ROUTE_REPORT_40_KBIT_SEC                                            0x02
#define LAST_WORKING_ROUTE_REPORT_100_KBIT_SEC                                           0x03
/* Values used for Statistics Report command */
#define STATISTICS_REPORT_ROUTE_CHANGES_RC                                               0x00
#define STATISTICS_REPORT_TRANSMISSION_COUNT_TC                                          0x01
#define STATISTICS_REPORT_NEIGHBORS_NB                                                   0x02
#define STATISTICS_REPORT_PACKET_ERROR_COUNT_PEC                                         0x03
#define STATISTICS_REPORT_SUM_OF_TRANSMISSION_TIMES_TS                                   0x04
#define STATISTICS_REPORT_SUM_OF_TRANSMISSION_TIMES_SQURARED_TS2                         0x05

/* Zip Naming command class commands */
#define ZIP_NAMING_VERSION                                                               0x01
#define ZIP_NAMING_NAME_SET                                                              0x01
#define ZIP_NAMING_NAME_GET                                                              0x02
#define ZIP_NAMING_NAME_REPORT                                                           0x03
#define ZIP_NAMING_LOCATION_SET                                                          0x04
#define ZIP_NAMING_LOCATION_GET                                                          0x05
#define ZIP_NAMING_LOCATION_REPORT                                                       0x06

/* Mailbox command class commands */
#define MAILBOX_VERSION                                                                  0x01
#define MAILBOX_CONFIGURATION_GET                                                        0x01
#define MAILBOX_CONFIGURATION_SET                                                        0x02
#define MAILBOX_CONFIGURATION_REPORT                                                     0x03
#define MAILBOX_QUEUE                                                                    0x04
#define MAILBOX_WAKEUP_NOTIFICATION                                                      0x05
#define MAILBOX_NODE_FAILING                                                             0x06
/* Values used for Mailbox Configuration Set command */
#define MAILBOX_CONFIGURATION_SET_PROPERTIES1_MODE_MASK                                  0x07
#define MAILBOX_CONFIGURATION_SET_MODE_DISABLE                                           0x00
#define MAILBOX_CONFIGURATION_SET_MODE_ENABLE_MAILBOX_SERVICE                            0x01
#define MAILBOX_CONFIGURATION_SET_MODE_ENABLE_MAILBOX_PROXY                              0x02
#define MAILBOX_CONFIGURATION_SET_PROPERTIES1_RESERVED_MASK                              0xF8
#define MAILBOX_CONFIGURATION_SET_PROPERTIES1_RESERVED_SHIFT                             0x03
/* Values used for Mailbox Configuration Report command */
#define MAILBOX_CONFIGURATION_REPORT_PROPERTIES1_MODE_MASK                               0x07
#define MAILBOX_CONFIGURATION_REPORT_MODE_DISABLE                                        0x00
#define MAILBOX_CONFIGURATION_REPORT_MODE_ENABLE_MAILBOX_SERVICE                         0x01
#define MAILBOX_CONFIGURATION_REPORT_MODE_ENABLE_MAILBOX_PROXY                           0x02
#define MAILBOX_CONFIGURATION_REPORT_PROPERTIES1_SUPPORTED_MODES_MASK                    0x18
#define MAILBOX_CONFIGURATION_REPORT_PROPERTIES1_SUPPORTED_MODES_SHIFT                   0x03
#define MAILBOX_CONFIGURATION_REPORT_SUPPORTED_MODES_MAILBOX_SERVICE_SUPPORTED           0x00
#define MAILBOX_CONFIGURATION_REPORT_SUPPORTED_MODES_MAILBOX_PROXY_SUPPORTED             0x01
#define MAILBOX_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_MASK                           0xE0
#define MAILBOX_CONFIGURATION_REPORT_PROPERTIES1_RESERVED_SHIFT                          0x05
/* Values used for Mailbox Queue command */
#define MAILBOX_QUEUE_PROPERTIES1_MODE_MASK                                              0x03
#define MAILBOX_QUEUE_MODE_PUSH                                                          0x00
#define MAILBOX_QUEUE_MODE_POP                                                           0x01
#define MAILBOX_QUEUE_MODE_WAITING                                                       0x02
#define MAILBOX_QUEUE_MODE_PING                                                          0x03
#define MAILBOX_QUEUE_MODE_ACK                                                           0x04
#define MAILBOX_QUEUE_MODE_NACK                                                          0x05
#define MAILBOX_QUEUE_MODE_QUEUE_FULL                                                    0x06
#define MAILBOX_QUEUE_PROPERTIES1_LAST_BIT_MASK                                          0x04
#define MAILBOX_QUEUE_PROPERTIES1_RESERVED_MASK                                          0xF8
#define MAILBOX_QUEUE_PROPERTIES1_RESERVED_SHIFT                                         0x03

/* Window Covering command class commands */
#define WINDOW_COVERING_VERSION                                                          0x01
#define WINDOW_COVERING_SUPPORTED_GET                                                    0x01
#define WINDOW_COVERING_SUPPORTED_REPORT                                                 0x02
#define WINDOW_COVERING_GET                                                              0x03
#define WINDOW_COVERING_REPORT                                                           0x04
#define WINDOW_COVERING_SET                                                              0x05
#define WINDOW_COVERING_START_LEVEL_CHANGE                                               0x06
#define WINDOW_COVERING_STOP_LEVEL_CHANGE                                                0x07
/* Values used for Window Covering Supported Report command */
#define WINDOW_COVERING_SUPPORTED_REPORT_PROPERTIES1_NUMBER_OF_PARAMETER_MASK_BYTES_MASK 0x0F
#define WINDOW_COVERING_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK                       0xF0
#define WINDOW_COVERING_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT                      0x04
/* Values used for Window Covering Get command */
#define WINDOW_COVERING_GET_OUT_LEFT_1                                                   0x00
#define WINDOW_COVERING_GET_OUT_LEFT_2                                                   0x01
#define WINDOW_COVERING_GET_OUT_RIGHT_1                                                  0x02
#define WINDOW_COVERING_GET_OUT_RIGHT_2                                                  0x03
#define WINDOW_COVERING_GET_IN_LEFT_1                                                    0x04
#define WINDOW_COVERING_GET_IN_LEFT_2                                                    0x05
#define WINDOW_COVERING_GET_IN_RIGHT_1                                                   0x06
#define WINDOW_COVERING_GET_IN_RIGHT_2                                                   0x07
#define WINDOW_COVERING_GET_IN_RIGHT_LEFT_1                                              0x08
#define WINDOW_COVERING_GET_IN_RIGHT_LEFT_2                                              0x09
#define WINDOW_COVERING_GET_VERTICAL_SLATS_ANGLE_1                                       0x0A
#define WINDOW_COVERING_GET_VERTICAL_SLATS_ANGLE_2                                       0x0B
#define WINDOW_COVERING_GET_OUT_BOTTOM_1                                                 0x0C
#define WINDOW_COVERING_GET_OUT_BOTTOM_2                                                 0x0D
#define WINDOW_COVERING_GET_OUT_TOP_1                                                    0x0E
#define WINDOW_COVERING_GET_OUT_TOP_2                                                    0x0F
#define WINDOW_COVERING_GET_IN_BOTTOM_1                                                  0x10
#define WINDOW_COVERING_GET_IN_BOTTOM_2                                                  0x11
#define WINDOW_COVERING_GET_IN_TOP_1                                                     0x0B
#define WINDOW_COVERING_GET_IN_TOP_2                                                     0x12
#define WINDOW_COVERING_GET_IN_TOP_BOTTOM_1                                              0x13
#define WINDOW_COVERING_GET_IN_TOP_BOTTOM_2                                              0x14
#define WINDOW_COVERING_GET_HORIZONTAL_SLATS_ANGLE_1                                     0x15
#define WINDOW_COVERING_GET_HORIZONTAL_SLATS_ANGLE_2                                     0x16
/* Values used for Window Covering Report command */
#define WINDOW_COVERING_REPORT_OUT_LEFT_1                                                0x00
#define WINDOW_COVERING_REPORT_OUT_LEFT_2                                                0x01
#define WINDOW_COVERING_REPORT_OUT_RIGHT_1                                               0x02
#define WINDOW_COVERING_REPORT_OUT_RIGHT_2                                               0x03
#define WINDOW_COVERING_REPORT_IN_LEFT_1                                                 0x04
#define WINDOW_COVERING_REPORT_IN_LEFT_2                                                 0x05
#define WINDOW_COVERING_REPORT_IN_RIGHT_1                                                0x06
#define WINDOW_COVERING_REPORT_IN_RIGHT_2                                                0x07
#define WINDOW_COVERING_REPORT_IN_RIGHT_LEFT_1                                           0x08
#define WINDOW_COVERING_REPORT_IN_RIGHT_LEFT_2                                           0x09
#define WINDOW_COVERING_REPORT_VERTICAL_SLATS_ANGLE_1                                    0x0A
#define WINDOW_COVERING_REPORT_VERTICAL_SLATS_ANGLE_2                                    0x0B
#define WINDOW_COVERING_REPORT_OUT_BOTTOM_1                                              0x0C
#define WINDOW_COVERING_REPORT_OUT_BOTTOM_2                                              0x0D
#define WINDOW_COVERING_REPORT_OUT_TOP_1                                                 0x0E
#define WINDOW_COVERING_REPORT_OUT_TOP_2                                                 0x0F
#define WINDOW_COVERING_REPORT_IN_BOTTOM_1                                               0x10
#define WINDOW_COVERING_REPORT_IN_BOTTOM_2                                               0x11
#define WINDOW_COVERING_REPORT_IN_TOP_1                                                  0x0B
#define WINDOW_COVERING_REPORT_IN_TOP_2                                                  0x12
#define WINDOW_COVERING_REPORT_IN_TOP_BOTTOM_1                                           0x13
#define WINDOW_COVERING_REPORT_IN_TOP_BOTTOM_2                                           0x14
#define WINDOW_COVERING_REPORT_HORIZONTAL_SLATS_ANGLE_1                                  0x15
#define WINDOW_COVERING_REPORT_HORIZONTAL_SLATS_ANGLE_2                                  0x16
/* Values used for Window Covering Set command */
#define WINDOW_COVERING_SET_PROPERTIES1_PARAMETER_COUNT_MASK                             0x1F
#define WINDOW_COVERING_SET_PROPERTIES1_RESERVED_MASK                                    0xE0
#define WINDOW_COVERING_SET_PROPERTIES1_RESERVED_SHIFT                                   0x05
#define WINDOW_COVERING_SET_OUT_LEFT_1                                                   0x00
#define WINDOW_COVERING_SET_OUT_LEFT_2                                                   0x01
#define WINDOW_COVERING_SET_OUT_RIGHT_1                                                  0x02
#define WINDOW_COVERING_SET_OUT_RIGHT_2                                                  0x03
#define WINDOW_COVERING_SET_IN_LEFT_1                                                    0x04
#define WINDOW_COVERING_SET_IN_LEFT_2                                                    0x05
#define WINDOW_COVERING_SET_IN_RIGHT_1                                                   0x06
#define WINDOW_COVERING_SET_IN_RIGHT_2                                                   0x07
#define WINDOW_COVERING_SET_IN_RIGHT_LEFT_1                                              0x08
#define WINDOW_COVERING_SET_IN_RIGHT_LEFT_2                                              0x09
#define WINDOW_COVERING_SET_VERTICAL_SLATS_ANGLE_1                                       0x0A
#define WINDOW_COVERING_SET_VERTICAL_SLATS_ANGLE_2                                       0x0B
#define WINDOW_COVERING_SET_OUT_BOTTOM_1                                                 0x0C
#define WINDOW_COVERING_SET_OUT_BOTTOM_2                                                 0x0D
#define WINDOW_COVERING_SET_OUT_TOP_1                                                    0x0E
#define WINDOW_COVERING_SET_OUT_TOP_2                                                    0x0F
#define WINDOW_COVERING_SET_IN_BOTTOM_1                                                  0x10
#define WINDOW_COVERING_SET_IN_BOTTOM_2                                                  0x11
#define WINDOW_COVERING_SET_IN_TOP_1                                                     0x0B
#define WINDOW_COVERING_SET_IN_TOP_2                                                     0x12
#define WINDOW_COVERING_SET_IN_TOP_BOTTOM_1                                              0x13
#define WINDOW_COVERING_SET_IN_TOP_BOTTOM_2                                              0x14
#define WINDOW_COVERING_SET_HORIZONTAL_SLATS_ANGLE_1                                     0x15
#define WINDOW_COVERING_SET_HORIZONTAL_SLATS_ANGLE_2                                     0x16
/* Values used for Window Covering Start Level Change command */
#define WINDOW_COVERING_START_LEVEL_CHANGE_PROPERTIES1_RES1_MASK                         0x3F
#define WINDOW_COVERING_START_LEVEL_CHANGE_PROPERTIES1_UP_DOWN_BIT_MASK                  0x40
#define WINDOW_COVERING_START_LEVEL_CHANGE_PROPERTIES1_RES2_BIT_MASK                     0x80
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_LEFT_1                                    0x00
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_LEFT_2                                    0x01
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_RIGHT_1                                   0x02
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_RIGHT_2                                   0x03
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_LEFT_1                                     0x04
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_LEFT_2                                     0x05
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_RIGHT_1                                    0x06
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_RIGHT_2                                    0x07
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_RIGHT_LEFT_1                               0x08
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_RIGHT_LEFT_2                               0x09
#define WINDOW_COVERING_START_LEVEL_CHANGE_VERTICAL_SLATS_ANGLE_1                        0x0A
#define WINDOW_COVERING_START_LEVEL_CHANGE_VERTICAL_SLATS_ANGLE_2                        0x0B
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_BOTTOM_1                                  0x0C
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_BOTTOM_2                                  0x0D
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_TOP_1                                     0x0E
#define WINDOW_COVERING_START_LEVEL_CHANGE_OUT_TOP_2                                     0x0F
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_BOTTOM_1                                   0x10
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_BOTTOM_2                                   0x11
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_TOP_1                                      0x0B
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_TOP_2                                      0x12
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_TOP_BOTTOM_1                               0x13
#define WINDOW_COVERING_START_LEVEL_CHANGE_IN_TOP_BOTTOM_2                               0x14
#define WINDOW_COVERING_START_LEVEL_CHANGE_HORIZONTAL_SLATS_ANGLE_1                      0x15
#define WINDOW_COVERING_START_LEVEL_CHANGE_HORIZONTAL_SLATS_ANGLE_2                      0x16
/* Values used for Window Covering Stop Level Change command */
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_LEFT_1                                     0x00
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_LEFT_2                                     0x01
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_RIGHT_1                                    0x02
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_RIGHT_2                                    0x03
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_LEFT_1                                      0x04
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_LEFT_2                                      0x05
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_RIGHT_1                                     0x06
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_RIGHT_2                                     0x07
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_RIGHT_LEFT_1                                0x08
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_RIGHT_LEFT_2                                0x09
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_VERTICAL_SLATS_ANGLE_1                         0x0A
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_VERTICAL_SLATS_ANGLE_2                         0x0B
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_BOTTOM_1                                   0x0C
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_BOTTOM_2                                   0x0D
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_TOP_1                                      0x0E
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_OUT_TOP_2                                      0x0F
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_BOTTOM_1                                    0x10
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_BOTTOM_2                                    0x11
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_TOP_1                                       0x0B
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_TOP_2                                       0x12
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_TOP_BOTTOM_1                                0x13
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_IN_TOP_BOTTOM_2                                0x14
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_HORIZONTAL_SLATS_ANGLE_1                       0x15
#define WINDOW_COVERING_STOP_LEVEL_CHANGE_HORIZONTAL_SLATS_ANGLE_2                       0x16

/* Security 2 command class commands */
#define SECURITY_2_VERSION                                                               0x01
#define SECURITY_2_NONCE_GET                                                             0x01
#define SECURITY_2_NONCE_REPORT                                                          0x02
#define SECURITY_2_MESSAGE_ENCAPSULATION                                                 0x03
#define KEX_GET                                                                          0x04
#define KEX_REPORT                                                                       0x05
#define KEX_SET                                                                          0x06
#define KEX_FAIL                                                                         0x07
#define PUBLIC_KEY_REPORT                                                                0x08
#define SECURITY_2_NETWORK_KEY_GET                                                       0x09
#define SECURITY_2_NETWORK_KEY_REPORT                                                    0x0A
#define SECURITY_2_NETWORK_KEY_VERIFY                                                    0x0B
#define SECURITY_2_TRANSFER_END                                                          0x0C
#define SECURITY_2_COMMANDS_SUPPORTED_GET                                                0x0D
#define SECURITY_2_COMMANDS_SUPPORTED_REPORT                                             0x0E
#define SECURITY_2_CAPABILITIES_GET                                                      0x0F
#define SECURITY_2_CAPABILITIES_REPORT                                                   0x10
/* Values used for Security 2 Nonce Report command */
#define SECURITY_2_NONCE_REPORT_PROPERTIES1_SOS_BIT_MASK                                 0x01
#define SECURITY_2_NONCE_REPORT_PROPERTIES1_MOS_BIT_MASK                                 0x02
#define SECURITY_2_NONCE_REPORT_PROPERTIES1_RESERVED_MASK                                0xFC
#define SECURITY_2_NONCE_REPORT_PROPERTIES1_RESERVED_SHIFT                               0x02
/* Values used for Security 2 Message Encapsulation command */
#define SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_EXTENSION_BIT_MASK                  0x01
#define SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_ENCRYPTED_EXTENSION_BIT_MASK        0x02
#define SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_RESERVED_MASK                       0xFC
#define SECURITY_2_MESSAGE_ENCAPSULATION_PROPERTIES1_RESERVED_SHIFT                      0x02
/* Values used for Kex Report command */
#define KEX_REPORT_PROPERTIES1_ECHO_BIT_MASK                                             0x01
#define KEX_REPORT_PROPERTIES1_REQUEST_CSA_BIT_MASK                                      0x02
#define KEX_REPORT_PROPERTIES1_RESERVED_MASK                                             0xFC
#define KEX_REPORT_PROPERTIES1_RESERVED_SHIFT                                            0x02
/* Values used for Kex Set command */
#define KEX_SET_PROPERTIES1_ECHO_BIT_MASK                                                0x01
#define KEX_SET_PROPERTIES1_REQUEST_CSA_BIT_MASK                                         0x02
#define KEX_SET_PROPERTIES1_RESERVED_MASK                                                0xFC
#define KEX_SET_PROPERTIES1_RESERVED_SHIFT                                               0x02
/* Values used for Kex Fail command */
#define KEX_FAIL_KEX_KEY                                                                 0x01
#define KEX_FAIL_KEX_SCHEME                                                              0x02
#define KEX_FAIL_KEX_CURVES                                                              0x03
#define KEX_FAIL_DECRYPT                                                                 0x05
#define KEX_FAIL_CANCEL                                                                  0x06
#define KEX_FAIL_AUTH                                                                    0x07
#define KEX_FAIL_KEY_GET                                                                 0x08
#define KEX_FAIL_KEY_VERIFY                                                              0x09
#define KEX_FAIL_KEY_REPORT                                                              0x0A
/* Values used for Public Key Report command */
#define PUBLIC_KEY_REPORT_PROPERTIES1_INCLUDING_NODE_BIT_MASK                            0x01
#define PUBLIC_KEY_REPORT_PROPERTIES1_RESERVED_MASK                                      0xFE
#define PUBLIC_KEY_REPORT_PROPERTIES1_RESERVED_SHIFT                                     0x01
/* Values used for Security 2 Transfer End command */
#define SECURITY_2_TRANSFER_END_PROPERTIES1_KEY_REQUEST_COMPLETE_BIT_MASK                0x01
#define SECURITY_2_TRANSFER_END_PROPERTIES1_KEY_VERIFIED_BIT_MASK                        0x02
#define SECURITY_2_TRANSFER_END_PROPERTIES1_RESERVED_MASK                                0xFC
#define SECURITY_2_TRANSFER_END_PROPERTIES1_RESERVED_SHIFT                               0x02

/* Irrigation command class commands */
#define IRRIGATION_VERSION                                                               0x01
#define IRRIGATION_SYSTEM_INFO_GET                                                       0x01
#define IRRIGATION_SYSTEM_INFO_REPORT                                                    0x02
#define IRRIGATION_SYSTEM_STATUS_GET                                                     0x03
#define IRRIGATION_SYSTEM_STATUS_REPORT                                                  0x04
#define IRRIGATION_SYSTEM_CONFIG_SET                                                     0x05
#define IRRIGATION_SYSTEM_CONFIG_GET                                                     0x06
#define IRRIGATION_SYSTEM_CONFIG_REPORT                                                  0x07
#define IRRIGATION_VALVE_INFO_GET                                                        0x08
#define IRRIGATION_VALVE_INFO_REPORT                                                     0x09
#define IRRIGATION_VALVE_CONFIG_SET                                                      0x0A
#define IRRIGATION_VALVE_CONFIG_GET                                                      0x0B
#define IRRIGATION_VALVE_CONFIG_REPORT                                                   0x0C
#define IRRIGATION_VALVE_RUN                                                             0x0D
#define IRRIGATION_VALVE_TABLE_SET                                                       0x0E
#define IRRIGATION_VALVE_TABLE_GET                                                       0x0F
#define IRRIGATION_VALVE_TABLE_REPORT                                                    0x10
#define IRRIGATION_VALVE_TABLE_RUN                                                       0x11
#define IRRIGATION_SYSTEM_SHUTOFF                                                        0x12
/* Values used for Irrigation System Info Report command */
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_MASTER_VALVE_BIT_MASK                  0x01
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED1_MASK                         0x06
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED1_SHIFT                        0x01
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED2_MASK                         0x18
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED2_SHIFT                        0x03
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED3_MASK                         0xE0
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES1_RESERVED3_SHIFT                        0x05
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES2_VALVE_TABLE_MAX_SIZE_MASK              0x0F
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES2_RESERVED_MASK                          0xF0
#define IRRIGATION_SYSTEM_INFO_REPORT_PROPERTIES2_RESERVED_SHIFT                         0x04
/* Values used for Irrigation System Status Report command */
#define IRRIGATION_SYSTEM_STATUS_REPORT_FLOW_SENSOR_DETECTED                             0x00
#define IRRIGATION_SYSTEM_STATUS_REPORT_PRESSURE_SENSOR_DETECTED                         0x01
#define IRRIGATION_SYSTEM_STATUS_REPORT_RAIN_SENSOR_DETECTED                             0x02
#define IRRIGATION_SYSTEM_STATUS_REPORT_MOISTURE_SENSOR_DETECTED                         0x03
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES1_FLOW_SIZE_MASK                       0x07
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES1_FLOW_SCALE_MASK                      0x18
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES1_FLOW_SCALE_SHIFT                     0x03
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES1_FLOW_PRECISION_MASK                  0xE0
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES1_FLOW_PRECISION_SHIFT                 0x05
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES2_PRESSURE_SIZE_MASK                   0x07
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES2_PRESSURE_SCALE_MASK                  0x18
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES2_PRESSURE_SCALE_SHIFT                 0x03
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES2_PRESSURE_PRECISION_MASK              0xE0
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES2_PRESSURE_PRECISION_SHIFT             0x05
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES3_MASTER_VALVE_BIT_MASK                0x01
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES3_RESERVED_MASK                        0xFE
#define IRRIGATION_SYSTEM_STATUS_REPORT_PROPERTIES3_RESERVED_SHIFT                       0x01
/* Values used for Irrigation System Config Set command */
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SIZE_MASK       0x07
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SCALE_MASK      0x18
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SCALE_SHIFT     0x03
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_PRECISION_MASK  0xE0
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_PRECISION_SHIFT 0x05
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SIZE_MASK        0x07
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SCALE_MASK       0x18
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SCALE_SHIFT      0x03
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES2_LOW_PRESSURE_THRESHOLD_PRECISION_MASK   0xE0
#define IRRIGATION_SYSTEM_CONFIG_SET_PROPERTIES2_LOW_PRESSURE_THRESHOLD_PRECISION_SHIFT  0x05
/* Values used for Irrigation System Config Report command */
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SIZE_MASK    0x07
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SCALE_MASK   0x18
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_SCALE_SHIFT  0x03
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_PRECISION_MASK 0xE0
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES1_HIGH_PRESSURE_THRESHOLD_PRECISION_SHIFT 0x05
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SIZE_MASK     0x07
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SCALE_MASK    0x18
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES2_LOW_PRESSURE_THRESHOLD_SCALE_SHIFT   0x03
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES2_LOW_PRESSURE_THRESHOLD_PRECISION_MASK 0xE0
#define IRRIGATION_SYSTEM_CONFIG_REPORT_PROPERTIES2_LOW_PRESSURE_THRESHOLD_PRECISION_SHIFT 0x05
/* Values used for Irrigation Valve Info Get command */
#define IRRIGATION_VALVE_INFO_GET_PROPERTIES1_MASTER_VALVE_BIT_MASK                      0x01
#define IRRIGATION_VALVE_INFO_GET_PROPERTIES1_RESERVED_MASK                              0xFE
#define IRRIGATION_VALVE_INFO_GET_PROPERTIES1_RESERVED_SHIFT                             0x01
/* Values used for Irrigation Valve Info Report command */
#define IRRIGATION_VALVE_INFO_REPORT_PROPERTIES1_MASTER_BIT_MASK                         0x01
#define IRRIGATION_VALVE_INFO_REPORT_PROPERTIES1_CONNECTED_BIT_MASK                      0x02
#define IRRIGATION_VALVE_INFO_REPORT_PROPERTIES1_RESERVED_MASK                           0xFC
#define IRRIGATION_VALVE_INFO_REPORT_PROPERTIES1_RESERVED_SHIFT                          0x02
/* Values used for Irrigation Valve Config Set command */
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES1_MASTER_VALVE_BIT_MASK                    0x01
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES1_RESERVED_MASK                            0xFE
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES1_RESERVED_SHIFT                           0x01
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES2_MAXIMUM_FLOW_SIZE_MASK                   0x07
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES2_MAXIMUM_FLOW_SCALE_MASK                  0x18
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES2_MAXIMUM_FLOW_SCALE_SHIFT                 0x03
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES2_MAXIMUM_FLOW_PRECISION_MASK              0xE0
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES2_MAXIMUM_FLOW_PRECISION_SHIFT             0x05
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES3_FLOW_HIGH_THRESHOLD_SIZE_MASK            0x07
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES3_FLOW_HIGH_THRESHOLD_SCALE_MASK           0x18
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES3_FLOW_HIGH_THRESHOLD_SCALE_SHIFT          0x03
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES3_FLOW_HIGH_THRESHOLD_PRECISION_MASK       0xE0
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES3_FLOW_HIGH_THRESHOLD_PRECISION_SHIFT      0x05
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES4_FLOW_LOW_THRESHOLD_SIZE_MASK             0x07
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES4_FLOW_LOW_THRESHOLD_SCALE_MASK            0x18
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES4_FLOW_LOW_THRESHOLD_SCALE_SHIFT           0x03
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES4_FLOW_LOW_THRESHOLD_PRECISION_MASK        0xE0
#define IRRIGATION_VALVE_CONFIG_SET_PROPERTIES4_FLOW_LOW_THRESHOLD_PRECISION_SHIFT       0x05
/* Values used for Irrigation Valve Config Get command */
#define IRRIGATION_VALVE_CONFIG_GET_PROPERTIES1_MASTER_VALVE_BIT_MASK                    0x01
#define IRRIGATION_VALVE_CONFIG_GET_PROPERTIES1_RESERVED_MASK                            0xFE
#define IRRIGATION_VALVE_CONFIG_GET_PROPERTIES1_RESERVED_SHIFT                           0x01
/* Values used for Irrigation Valve Config Report command */
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES1_MASTER_VALVE_BIT_MASK                 0x01
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES1_RESERVED_MASK                         0xFE
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES1_RESERVED_SHIFT                        0x01
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES2_MAXIMUM_FLOW_SIZE_MASK                0x07
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES2_MAXIMUM_FLOW_SCALE_MASK               0x18
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES2_MAXIMUM_FLOW_SCALE_SHIFT              0x03
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES2_MAXIMUM_FLOW_PRECISION_MASK           0xE0
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES2_MAXIMUM_FLOW_PRECISION_SHIFT          0x05
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES3_FLOW_HIGH_THRESHOLD_SIZE_MASK         0x07
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES3_FLOW_HIGH_THRESHOLD_SCALE_MASK        0x18
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES3_FLOW_HIGH_THRESHOLD_SCALE_SHIFT       0x03
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES3_FLOW_HIGH_THRESHOLD_PRECISION_MASK    0xE0
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES3_FLOW_HIGH_THRESHOLD_PRECISION_SHIFT   0x05
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES4_FLOW_LOW_THRESHOLD_SIZE_MASK          0x07
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES4_FLOW_LOW_THRESHOLD_SCALE_MASK         0x18
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES4_FLOW_LOW_THRESHOLD_SCALE_SHIFT        0x03
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES4_FLOW_LOW_THRESHOLD_PRECISION_MASK     0xE0
#define IRRIGATION_VALVE_CONFIG_REPORT_PROPERTIES4_FLOW_LOW_THRESHOLD_PRECISION_SHIFT    0x05
/* Values used for Irrigation Valve Run command */
#define IRRIGATION_VALVE_RUN_PROPERTIES1_MASTER_VALVE_BIT_MASK                           0x01
#define IRRIGATION_VALVE_RUN_PROPERTIES1_RESERVED_MASK                                   0xFE
#define IRRIGATION_VALVE_RUN_PROPERTIES1_RESERVED_SHIFT                                  0x01

/* Supervision command class commands */
#define SUPERVISION_VERSION                                                              0x01
#define SUPERVISION_GET                                                                  0x01
#define SUPERVISION_REPORT                                                               0x02
/* Values used for Supervision Get command */
#define SUPERVISION_GET_PROPERTIES1_SESSION_ID_MASK                                      0x3F
#define SUPERVISION_GET_PROPERTIES1_RESERVED_BIT_MASK                                    0x40
#define SUPERVISION_GET_PROPERTIES1_STATUS_UPDATES_BIT_MASK                              0x80
/* Values used for Supervision Report command */
#define SUPERVISION_REPORT_PROPERTIES1_SESSION_ID_MASK                                   0x3F
#define SUPERVISION_REPORT_PROPERTIES1_RESERVED_BIT_MASK                                 0x40
#define SUPERVISION_REPORT_PROPERTIES1_MORE_STATUS_UPDATES_BIT_MASK                      0x80
#define SUPERVISION_REPORT_NO_SUPPORT                                                    0x00
#define SUPERVISION_REPORT_WORKING                                                       0x01
#define SUPERVISION_REPORT_FAIL                                                          0x02
#define SUPERVISION_REPORT_BUSY                                                          0x03
#define SUPERVISION_REPORT_SUCCESS                                                       0xFF

/* Humidity Control Setpoint command class commands */
#define HUMIDITY_CONTROL_SETPOINT_VERSION                                                0x01
#define HUMIDITY_CONTROL_SETPOINT_SET                                                    0x01
#define HUMIDITY_CONTROL_SETPOINT_GET                                                    0x02
#define HUMIDITY_CONTROL_SETPOINT_REPORT                                                 0x03
#define HUMIDITY_CONTROL_SETPOINT_SUPPORTED_GET                                          0x04
#define HUMIDITY_CONTROL_SETPOINT_SUPPORTED_REPORT                                       0x05
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET                                    0x06
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT                                 0x07
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET                                       0x08
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT                                    0x09
/* Values used for Humidity Control Setpoint Set command */
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES1_SETPOINT_TYPE_MASK                     0x0F
#define HUMIDITY_CONTROL_SETPOINT_SET_SETPOINT_TYPE_HUMIDIFIER                           0x01
#define HUMIDITY_CONTROL_SETPOINT_SET_SETPOINT_TYPE_DEHUMIDIFIER                         0x02
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES1_RESERVED_MASK                          0xF0
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES1_RESERVED_SHIFT                         0x04
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES2_SIZE_MASK                              0x07
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES2_SCALE_MASK                             0x18
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES2_SCALE_SHIFT                            0x03
#define HUMIDITY_CONTROL_SETPOINT_SET_SCALE_PERCENTAGE                                   0x00
#define HUMIDITY_CONTROL_SETPOINT_SET_SCALE_ABSOLUTE                                     0x01
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES2_PRECISION_MASK                         0xE0
#define HUMIDITY_CONTROL_SETPOINT_SET_PROPERTIES2_PRECISION_SHIFT                        0x05
/* Values used for Humidity Control Setpoint Get command */
#define HUMIDITY_CONTROL_SETPOINT_GET_PROPERTIES1_SETPOINT_TYPE_MASK                     0x0F
#define HUMIDITY_CONTROL_SETPOINT_GET_SETPOINT_TYPE_HUMIDIFIER                           0x01
#define HUMIDITY_CONTROL_SETPOINT_GET_SETPOINT_TYPE_DEHUMIDIFIER                         0x02
#define HUMIDITY_CONTROL_SETPOINT_GET_PROPERTIES1_RESERVED_MASK                          0xF0
#define HUMIDITY_CONTROL_SETPOINT_GET_PROPERTIES1_RESERVED_SHIFT                         0x04
/* Values used for Humidity Control Setpoint Report command */
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES1_SETPOINT_TYPE_MASK                  0x0F
#define HUMIDITY_CONTROL_SETPOINT_REPORT_SETPOINT_TYPE_HUMIDIFIER                        0x01
#define HUMIDITY_CONTROL_SETPOINT_REPORT_SETPOINT_TYPE_DEHUMIDIFIER                      0x02
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES1_RESERVED_MASK                       0xF0
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES1_RESERVED_SHIFT                      0x04
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES2_SIZE_MASK                           0x07
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES2_SCALE_MASK                          0x18
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES2_SCALE_SHIFT                         0x03
#define HUMIDITY_CONTROL_SETPOINT_REPORT_SCALE_PERCENTAGE                                0x00
#define HUMIDITY_CONTROL_SETPOINT_REPORT_SCALE_ABSOLUTE                                  0x01
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES2_PRECISION_MASK                      0xE0
#define HUMIDITY_CONTROL_SETPOINT_REPORT_PROPERTIES2_PRECISION_SHIFT                     0x05
/* Values used for Humidity Control Setpoint Scale Supported Get command */
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET_PROPERTIES1_SETPOINT_TYPE_MASK     0x0F
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET_SETPOINT_TYPE_HUMIDIFIER           0x01
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET_SETPOINT_TYPE_DEHUMIDIFIER         0x02
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET_PROPERTIES1_RESERVED_MASK          0xF0
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_GET_PROPERTIES1_RESERVED_SHIFT         0x04
/* Values used for Humidity Control Setpoint Scale Supported Report command */
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT_PROPERTIES1_SCALE_BIT_MASK_MASK 0x0F
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT_SCALE_BIT_MASK_PERCENTAGE       0x00
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT_SCALE_BIT_MASK_ABSOLUTE         0x01
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_MASK       0xF0
#define HUMIDITY_CONTROL_SETPOINT_SCALE_SUPPORTED_REPORT_PROPERTIES1_RESERVED_SHIFT      0x04
/* Values used for Humidity Control Setpoint Capabilities Get command */
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET_PROPERTIES1_SETPOINT_TYPE_MASK        0x0F
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_HUMIDIFIER              0x01
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET_SETPOINT_TYPE_DEHUMIDIFIER            0x02
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET_PROPERTIES1_RESERVED_MASK             0xF0
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_GET_PROPERTIES1_RESERVED_SHIFT            0x04
/* Values used for Humidity Control Setpoint Capabilities Report command */
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_SETPOINT_TYPE_MASK     0x0F
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_HUMIDIFIER           0x01
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SETPOINT_TYPE_DEHUMIDIFIER         0x02
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_RESERVED_MASK          0xF0
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES1_RESERVED_SHIFT         0x04
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SIZE1_MASK             0x07
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SCALE1_MASK            0x18
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_SCALE1_SHIFT           0x03
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SCALE1_PERCENTAGE                  0x00
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SCALE1_ABSOLUTE                    0x01
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_PRECISION1_MASK        0xE0
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES2_PRECISION1_SHIFT       0x05
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SIZE2_MASK             0x07
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SCALE2_MASK            0x18
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_SCALE2_SHIFT           0x03
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SCALE2_PERCENTAGE                  0x00
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_SCALE2_ABSOLUTE                    0x01
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_PRECISION2_MASK        0xE0
#define HUMIDITY_CONTROL_SETPOINT_CAPABILITIES_REPORT_PROPERTIES3_PRECISION2_SHIFT       0x05

/* Humidity Control Mode command class commands */
#define HUMIDITY_CONTROL_MODE_VERSION                                                    0x01
#define HUMIDITY_CONTROL_MODE_SET                                                        0x01
#define HUMIDITY_CONTROL_MODE_GET                                                        0x02
#define HUMIDITY_CONTROL_MODE_REPORT                                                     0x03
#define HUMIDITY_CONTROL_MODE_SUPPORTED_GET                                              0x04
#define HUMIDITY_CONTROL_MODE_SUPPORTED_REPORT                                           0x05
/* Values used for Humidity Control Mode Set command */
#define HUMIDITY_CONTROL_MODE_SET_PROPERTIES1_MODE_MASK                                  0x0F
#define HUMIDITY_CONTROL_MODE_SET_MODE_OFF                                               0x00
#define HUMIDITY_CONTROL_MODE_SET_MODE_HUMIDIFY                                          0x01
#define HUMIDITY_CONTROL_MODE_SET_MODE_DEHUMIDIFY                                        0x02
#define HUMIDITY_CONTROL_MODE_SET_PROPERTIES1_RESERVED_MASK                              0xF0
#define HUMIDITY_CONTROL_MODE_SET_PROPERTIES1_RESERVED_SHIFT                             0x04
/* Values used for Humidity Control Mode Report command */
#define HUMIDITY_CONTROL_MODE_REPORT_PROPERTIES1_MODE_MASK                               0x0F
#define HUMIDITY_CONTROL_MODE_REPORT_MODE_OFF                                            0x00
#define HUMIDITY_CONTROL_MODE_REPORT_MODE_HUMIDIFY                                       0x01
#define HUMIDITY_CONTROL_MODE_REPORT_MODE_DEHUMIDIFY                                     0x02
#define HUMIDITY_CONTROL_MODE_REPORT_PROPERTIES1_RESERVED_MASK                           0xF0
#define HUMIDITY_CONTROL_MODE_REPORT_PROPERTIES1_RESERVED_SHIFT                          0x04

/* Humidity Control Operating State command class commands */
#define HUMIDITY_CONTROL_OPERATING_STATE_VERSION                                         0x01
#define HUMIDITY_CONTROL_OPERATING_STATE_GET                                             0x01
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT                                          0x02
/* Values used for Humidity Control Operating State Report command */
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_PROPERTIES1_OPERATING_STATE_MASK         0x0F
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_OPERATING_STATE_IDLE                     0x00
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_OPERATING_STATE_HUMIDIFYING              0x01
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_OPERATING_STATE_DEHUMIDIFYING            0x02
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_PROPERTIES1_RESERVED_MASK                0xF0
#define HUMIDITY_CONTROL_OPERATING_STATE_REPORT_PROPERTIES1_RESERVED_SHIFT               0x04

/* Entry Control command class commands */
#define ENTRY_CONTROL_VERSION                                                            0x01
#define ENTRY_CONTROL_NOTIFICATION                                                       0x01
#define ENTRY_CONTROL_KEY_SUPPORTED_GET                                                  0x02
#define ENTRY_CONTROL_KEY_SUPPORTED_REPORT                                               0x03
#define ENTRY_CONTROL_EVENT_SUPPORTED_GET                                                0x04
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT                                             0x05
#define ENTRY_CONTROL_CONFIGURATION_SET                                                  0x06
#define ENTRY_CONTROL_CONFIGURATION_GET                                                  0x07
#define ENTRY_CONTROL_CONFIGURATION_REPORT                                               0x08
/* Values used for Entry Control Notification command */
#define ENTRY_CONTROL_NOTIFICATION_PROPERTIES1_DATA_TYPE_MASK                            0x03
#define ENTRY_CONTROL_NOTIFICATION_DATA_TYPE_NA                                          0x00
#define ENTRY_CONTROL_NOTIFICATION_DATA_TYPE_RAW                                         0x01
#define ENTRY_CONTROL_NOTIFICATION_DATA_TYPE_ASCII                                       0x02
#define ENTRY_CONTROL_NOTIFICATION_DATA_TYPE_MD5                                         0x03
#define ENTRY_CONTROL_NOTIFICATION_PROPERTIES1_RESERVED_MASK                             0xFC
#define ENTRY_CONTROL_NOTIFICATION_PROPERTIES1_RESERVED_SHIFT                            0x02
#define ENTRY_CONTROL_NOTIFICATION_CACHING                                               0x00
#define ENTRY_CONTROL_NOTIFICATION_CACHED_KEYS                                           0x01
#define ENTRY_CONTROL_NOTIFICATION_ENTER                                                 0x02
#define ENTRY_CONTROL_NOTIFICATION_DISARM_ALL                                            0x03
#define ENTRY_CONTROL_NOTIFICATION_ARM_ALL                                               0x04
#define ENTRY_CONTROL_NOTIFICATION_ARM_AWAY                                              0x05
#define ENTRY_CONTROL_NOTIFICATION_ARM_HOME                                              0x06
#define ENTRY_CONTROL_NOTIFICATION_EXIT_DELAY                                            0x07
#define ENTRY_CONTROL_NOTIFICATION_ARM_1                                                 0x08
#define ENTRY_CONTROL_NOTIFICATION_ARM_2                                                 0x09
#define ENTRY_CONTROL_NOTIFICATION_ARM_3                                                 0x0A
#define ENTRY_CONTROL_NOTIFICATION_ARM_4                                                 0x0B
#define ENTRY_CONTROL_NOTIFICATION_ARM_5                                                 0x0C
#define ENTRY_CONTROL_NOTIFICATION_ARM_6                                                 0x0D
#define ENTRY_CONTROL_NOTIFICATION_RFID                                                  0x0E
#define ENTRY_CONTROL_NOTIFICATION_BELL                                                  0x0F
#define ENTRY_CONTROL_NOTIFICATION_FIRE                                                  0x10
#define ENTRY_CONTROL_NOTIFICATION_POLICE                                                0x11
#define ENTRY_CONTROL_NOTIFICATION_ALERT_PANIC                                           0x12
#define ENTRY_CONTROL_NOTIFICATION_ALERT_MEDICAL                                         0x13
#define ENTRY_CONTROL_NOTIFICATION_GATE_OPEN                                             0x14
#define ENTRY_CONTROL_NOTIFICATION_GATE_CLOSE                                            0x15
#define ENTRY_CONTROL_NOTIFICATION_LOCK                                                  0x16
#define ENTRY_CONTROL_NOTIFICATION_UNLOCK                                                0x17
#define ENTRY_CONTROL_NOTIFICATION_TEST                                                  0x18
#define ENTRY_CONTROL_NOTIFICATION_CANCEL                                                0x19
/* Values used for Entry Control Event Supported Report command */
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES1_DATA_TYPE_SUPPORTED_BIT_MASK_LENGTH_MASK 0x03
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_MASK                  0xFC
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES1_RESERVED1_SHIFT                 0x02
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES2_EVENT_SUPPORTED_BIT_MASK_LENGTH_MASK 0x1F
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_MASK                  0xE0
#define ENTRY_CONTROL_EVENT_SUPPORTED_REPORT_PROPERTIES2_RESERVED2_SHIFT                 0x05

/* Inclusion Controller command class commands */
#define INCLUSION_CONTROLLER_VERSION                                                     0x01
#define INITIATE                                                                         0x01
#define COMPLETE                                                                         0x02
/* Values used for Initiate command */
#define INITIATE_PROXY_INCLUSION                                                         0x01
#define INITIATE_S0_INCLUSION                                                            0x02
/* Values used for Complete command */
#define COMPLETE_PROXY_INCLUSION                                                         0x01
#define COMPLETE_S0_INCLUSION                                                            0x02
#define COMPLETE_STEP_OK                                                                 0x01
#define COMPLETE_STEP_USER_REJECTED                                                      0x02
#define COMPLETE_STEP_FAILED                                                             0x03
#define COMPLETE_STEP_NOT_SUPPORTED                                                      0x04



/* Max. frame size to allow routing over 4 hops */
#define META_DATA_MAX_DATA_SIZE                      48



#endif

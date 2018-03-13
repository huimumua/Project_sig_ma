package com.askey.firefly.zwave.control.bean;

import android.os.Parcel;
import android.os.Parcelable;


import com.google.gson.annotations.SerializedName;

import java.util.List;

/**
 * Created by skysoft on 2017/8/3.
 */

public class DeviceList {

    @SerializedName("MessageType")
    private String messageType;
    @SerializedName("Node Info List")
    public List<NodeInfoList> nodeList;


    public static class NodeInfoList implements Parcelable {
        @SerializedName("Home id")
        public String homeId;
        @SerializedName("Node id")
        public String nodeId;
        @SerializedName("wakeup interval")
        public String wakeup;
        @SerializedName("Node security inclusion status")
        public String securityStatus;
        @SerializedName("Vendor id")
        public String vendorId;
        @SerializedName("Product type id")
        public String productTypeId;
        @SerializedName("Product id")
        public String productId;
        @SerializedName("Category")
        public String category;
        @SerializedName("Z-wave library type")
        public String librarytype;
        @SerializedName("Z-wave protocol version")
        public String protocolVersion;
        @SerializedName("Application version")
        public String applicationVersion;
        @SerializedName("Hardware version")
        public String hardwareVersion;
        @SerializedName("Firmware")
        public String firmware;
        @SerializedName("Firmware version")
        public String firmwareVersion;
        @SerializedName("EndPoint List")
        public List<EndPoint> endPoint;

        protected NodeInfoList(Parcel in) {
            homeId = in.readString();
            nodeId = in.readString();
            wakeup = in.readString();
            securityStatus = in.readString();
            vendorId = in.readString();
            productTypeId = in.readString();
            productId = in.readString();
            category = in.readString();
            librarytype = in.readString();
            protocolVersion = in.readString();
            applicationVersion = in.readString();
            hardwareVersion = in.readString();
            firmware = in.readString();
            firmwareVersion = in.readString();
            endPoint = in.createTypedArrayList(EndPoint.CREATOR);
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeString(homeId);
            dest.writeString(nodeId);
            dest.writeString(wakeup);
            dest.writeString(securityStatus);
            dest.writeString(vendorId);
            dest.writeString(productTypeId);
            dest.writeString(productId);
            dest.writeString(category);
            dest.writeString(librarytype);
            dest.writeString(protocolVersion);
            dest.writeString(applicationVersion);
            dest.writeString(hardwareVersion);
            dest.writeString(firmware);
            dest.writeString(firmwareVersion);
            dest.writeTypedList(endPoint);
        }

        public static final Creator<NodeInfoList> CREATOR = new Creator<NodeInfoList>() {
            @Override
            public NodeInfoList createFromParcel(Parcel in) {
                return new NodeInfoList(in);
            }

            @Override
            public NodeInfoList[] newArray(int size) {
                return new NodeInfoList[size];
            }
        };

        public String getHomeId() {
            return homeId;
        }

        public void setHomeId(String homeId) {
            this.homeId = homeId;
        }

        public String getNodeId() {
            return nodeId;
        }

        public void setNodeId(String nodeId) {
            this.nodeId = nodeId;
        }

        public String getWakeup() {
            return wakeup;
        }

        public void setWakeup(String wakeup) {
            this.wakeup = wakeup;
        }

        public String getSecurityStatus() {
            return securityStatus;
        }

        public void setSecurityStatus(String securityStatus) {
            this.securityStatus = securityStatus;
        }

        public String getVendorId() {
            return vendorId;
        }

        public void setVendorId(String vendorId) {
            this.vendorId = vendorId;
        }

        public String getProductTypeId() {
            return productTypeId;
        }

        public void setProductTypeId(String productTypeId) {
            this.productTypeId = productTypeId;
        }

        public String getProductId() {
            return productId;
        }

        public void setProductId(String productId) {
            this.productId = productId;
        }

        public String getCategory() {
            return category;
        }

        public void setCategory(String category) {
            this.category = category;
        }

        public String getLibrarytype() {
            return librarytype;
        }

        public void setLibrarytype(String librarytype) {
            this.librarytype = librarytype;
        }

        public String getProtocolVersion() {
            return protocolVersion;
        }

        public void setProtocolVersion(String protocolVersion) {
            this.protocolVersion = protocolVersion;
        }

        public String getApplicationVersion() {
            return applicationVersion;
        }

        public void setApplicationVersion(String applicationVersion) {
            this.applicationVersion = applicationVersion;
        }

        public String getHardwareVersion() {
            return hardwareVersion;
        }

        public void setHardwareVersion(String hardwareVersion) {
            this.hardwareVersion = hardwareVersion;
        }

        public String getFirmware() {
            return firmware;
        }

        public void setFirmware(String firmware) {
            this.firmware = firmware;
        }

        public String getFirmwareVersion() {
            return firmwareVersion;
        }

        public void setFirmwareVersion(String firmwareVersion) {
            this.firmwareVersion = firmwareVersion;
        }

        public List<EndPoint> getEndPoint() {
            return endPoint;
        }

        public void setEndPoint(List<EndPoint> endPoint) {
            this.endPoint = endPoint;
        }

        @Override
        public int describeContents() {
            return 0;
        }

    }

        public static class EndPoint implements Parcelable{
            @SerializedName("Endpoint id")
            public String endPointId;
            @SerializedName("Device class generic")
            public String generic;
            @SerializedName("Device class specific")
            public String classSpecific;
            @SerializedName("Endpoint name")
            public String endPointName;
            @SerializedName("Endpoint location")
            public String endPointLocation;
            @SerializedName("ZWave+ version")
            public String zwaveVersion;
            @SerializedName("ZWave+ node typen")
            public String nodeTyoen;
            @SerializedName("ZWave+ role type")
            public String roleType;
            @SerializedName("ZWave+ installer icon")
            public String installerIcon;
            @SerializedName("ZWave+ user icon")
            public String userIcon;
            @SerializedName("Interface List")
            public List<InterfaceList> interfaceList;

            protected EndPoint(Parcel in) {
                endPointId = in.readString();
                generic = in.readString();
                classSpecific = in.readString();
                endPointName = in.readString();
                endPointLocation = in.readString();
                zwaveVersion = in.readString();
                nodeTyoen = in.readString();
                roleType = in.readString();
                installerIcon = in.readString();
                userIcon = in.readString();
                interfaceList = in.createTypedArrayList(InterfaceList.CREATOR);
            }

            public static final Creator<EndPoint> CREATOR = new Creator<EndPoint>() {
                @Override
                public EndPoint createFromParcel(Parcel in) {
                    return new EndPoint(in);
                }

                @Override
                public EndPoint[] newArray(int size) {
                    return new EndPoint[size];
                }
            };

            public String getEndPointId() {
                return endPointId;
            }

            public void setEndPointId(String endPointId) {
                this.endPointId = endPointId;
            }

            public String getGeneric() {
                return generic;
            }

            public void setGeneric(String generic) {
                this.generic = generic;
            }

            public String getClassSpecific() {
                return classSpecific;
            }

            public void setClassSpecific(String classSpecific) {
                this.classSpecific = classSpecific;
            }

            public String getEndPointName() {
                return endPointName;
            }

            public void setEndPointName(String endPointName) {
                this.endPointName = endPointName;
            }

            public String getEndPointLocation() {
                return endPointLocation;
            }

            public void setEndPointLocation(String endPointLocation) {
                this.endPointLocation = endPointLocation;
            }

            public String getZwaveVersion() {
                return zwaveVersion;
            }

            public void setZwaveVersion(String zwaveVersion) {
                this.zwaveVersion = zwaveVersion;
            }

            public String getNodeTyoen() {
                return nodeTyoen;
            }

            public void setNodeTyoen(String nodeTyoen) {
                this.nodeTyoen = nodeTyoen;
            }

            public String getRoleType() {
                return roleType;
            }

            public void setRoleType(String roleType) {
                this.roleType = roleType;
            }

            public String getInstallerIcon() {
                return installerIcon;
            }

            public void setInstallerIcon(String installerIcon) {
                this.installerIcon = installerIcon;
            }

            public String getUserIcon() {
                return userIcon;
            }

            public void setUserIcon(String userIcon) {
                this.userIcon = userIcon;
            }

            public List<InterfaceList> getInterfaceList() {
                return interfaceList;
            }

            public void setInterfaceList(List<InterfaceList> interfaceList) {
                this.interfaceList = interfaceList;
            }

            @Override
            public int describeContents() {
                return 0;
            }

            @Override
            public void writeToParcel(Parcel parcel, int i) {

                parcel.writeString(endPointId);
                parcel.writeString(generic);
                parcel.writeString(classSpecific);
                parcel.writeString(endPointName);
                parcel.writeString(endPointLocation);
                parcel.writeString(zwaveVersion);
                parcel.writeString(nodeTyoen);
                parcel.writeString(roleType);
                parcel.writeString(installerIcon);
                parcel.writeString(userIcon);
                parcel.writeTypedList(interfaceList);
            }


        }
            public static class InterfaceList implements Parcelable{
                @SerializedName("Interface Class")
                public String interfaceInfo;
                @SerializedName("Group info type")
                public String groupInfoType;
                @SerializedName("Maximum supported groups")
                public String groups;
                @SerializedName("Valid groups")
                public String validGroups;
                @SerializedName("Group Info List")
                public List<GroupInfoList> groupInfoList;
                @SerializedName("Sensor Info List")
                public List<SensorInfoList> sensorInfoList;

                protected InterfaceList(Parcel in) {
                    interfaceInfo = in.readString();
                    groupInfoType = in.readString();
                    groups = in.readString();
                    validGroups = in.readString();
                    groupInfoList = in.createTypedArrayList(GroupInfoList.CREATOR);
                    sensorInfoList = in.createTypedArrayList(SensorInfoList.CREATOR);
                }

                @Override
                public void writeToParcel(Parcel dest, int flags) {
                    dest.writeString(interfaceInfo);
                    dest.writeString(groupInfoType);
                    dest.writeString(groups);
                    dest.writeString(validGroups);
                    dest.writeTypedList(groupInfoList);
                    dest.writeTypedList(sensorInfoList);
                }

                @Override
                public int describeContents() {
                    return 0;
                }

                public static final Creator<InterfaceList> CREATOR = new Creator<InterfaceList>() {
                    @Override
                    public InterfaceList createFromParcel(Parcel in) {
                        return new InterfaceList(in);
                    }

                    @Override
                    public InterfaceList[] newArray(int size) {
                        return new InterfaceList[size];
                    }
                };

                public String getInterfaceInfo() {
                    return interfaceInfo;
                }

                public void setInterfaceInfo(String interfaceInfo) {
                    this.interfaceInfo = interfaceInfo;
                }

                public String getGroupInfoType() {
                    return groupInfoType;
                }

                public void setGroupInfoType(String groupInfoType) {
                    this.groupInfoType = groupInfoType;
                }

                public String getGroups() {
                    return groups;
                }

                public void setGroups(String groups) {
                    this.groups = groups;
                }

                public String getValidGroups() {
                    return validGroups;
                }

                public void setValidGroups(String validGroups) {
                    this.validGroups = validGroups;
                }

                public List<GroupInfoList> getGroupInfoList() {
                    return groupInfoList;
                }

                public void setGroupInfoList(List<GroupInfoList> groupInfoList) {
                    this.groupInfoList = groupInfoList;
                }

                public List<SensorInfoList> getSensorInfoList() {
                    return sensorInfoList;
                }

                public void setSensorInfoList(List<SensorInfoList> sensorInfoList) {
                    this.sensorInfoList = sensorInfoList;
                }
            }
            public static class GroupInfoList implements Parcelable{

                @SerializedName("Group id")
                public String groupId;
                @SerializedName("profile")
                public String profile;
                @SerializedName("event code")
                public String eventCode;
                @SerializedName("Cmd Info List")
                public List<CmdInfoList> cmdInfoList;


                protected GroupInfoList(Parcel in) {
                    groupId = in.readString();
                    profile = in.readString();
                    eventCode = in.readString();
                    cmdInfoList = in.createTypedArrayList(CmdInfoList.CREATOR);
                }

                @Override
                public void writeToParcel(Parcel dest, int flags) {
                    dest.writeString(groupId);
                    dest.writeString(profile);
                    dest.writeString(eventCode);
                    dest.writeTypedList(cmdInfoList);
                }

                public static final Creator<GroupInfoList> CREATOR = new Creator<GroupInfoList>() {
                    @Override
                    public GroupInfoList createFromParcel(Parcel in) {
                        return new GroupInfoList(in);
                    }

                    @Override
                    public GroupInfoList[] newArray(int size) {
                        return new GroupInfoList[size];
                    }
                };

                public String getGroupId() {
                    return groupId;
                }

                public void setGroupId(String groupId) {
                    this.groupId = groupId;
                }

                public String getProfile() {
                    return profile;
                }

                public void setProfile(String profile) {
                    this.profile = profile;
                }

                public String getEventCode() {
                    return eventCode;
                }

                public void setEventCode(String eventCode) {
                    this.eventCode = eventCode;
                }

                public List<CmdInfoList> getCmdInfoList() {
                    return cmdInfoList;
                }

                public void setCmdInfoList(List<CmdInfoList> cmdInfoList) {
                    this.cmdInfoList = cmdInfoList;
                }

                @Override
                public int describeContents() {
                    return 0;
                }

            }
            public static class SensorInfoList implements Parcelable{

                @SerializedName("sensor type")
                public String sensorType;
                @SerializedName("Unit List")
                public String[] unitList;


                protected SensorInfoList(Parcel in) {
                    sensorType = in.readString();
                    unitList = in.createStringArray();
                }

                @Override
                public void writeToParcel(Parcel dest, int flags) {
                    dest.writeString(sensorType);
                    dest.writeStringArray(unitList);
                }

                @Override
                public int describeContents() {
                    return 0;
                }

                public static final Creator<SensorInfoList> CREATOR = new Creator<SensorInfoList>() {
                    @Override
                    public SensorInfoList createFromParcel(Parcel in) {
                        return new SensorInfoList(in);
                    }

                    @Override
                    public SensorInfoList[] newArray(int size) {
                        return new SensorInfoList[size];
                    }
                };

                public String[] getUnitList() {
                    return unitList;
                }

                public void setUnitList(String[] unitList) {
                    this.unitList = unitList;
                }

                public String getSensorType() {
                    return sensorType;
                }

                public void setSensorType(String sensorType) {
                    this.sensorType = sensorType;
                }



            }
            public static class CmdInfoList implements Parcelable{
                @SerializedName("command class")
                public String commandClass;
                @SerializedName("command")
                public String command;

                protected CmdInfoList(Parcel in) {
                    commandClass = in.readString();
                    command = in.readString();
                }

                public static final Creator<CmdInfoList> CREATOR = new Creator<CmdInfoList>() {
                    @Override
                    public CmdInfoList createFromParcel(Parcel in) {
                        return new CmdInfoList(in);
                    }

                    @Override
                    public CmdInfoList[] newArray(int size) {
                        return new CmdInfoList[size];
                    }
                };

                public String getCommandClass() {
                    return commandClass;
                }

                public void setCommandClass(String commandClass) {
                    this.commandClass = commandClass;
                }

                public String getCommand() {
                    return command;
                }

                public void setCommand(String command) {
                    this.command = command;
                }

                @Override
                public int describeContents() {
                    return 0;
                }

                @Override
                public void writeToParcel(Parcel parcel, int i) {
                    parcel.writeString(commandClass);
                    parcel.writeString(command);
                }
            }

    public String getMessageType() {
        return messageType;
    }

    public void setMessageType(String messageType) {
        this.messageType = messageType;
    }

    public List<NodeInfoList> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<NodeInfoList> nodeList) {
        this.nodeList = nodeList;
    }
}

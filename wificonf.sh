#!/bin/sh
proto=$1
ssid=$2
passwd=$3

usage()
{
    echo "Usage:"
    echo "   wificonf.sh sta|ap|ip [ssid] [passwd]"
    echo "   sta  client mode, connect to router, ssid and passwd is requied"
    echo "   ap   access pointer, ssid is required, passwd will be empty"
    echo "   ip   print current ip list"
    echo
    exit 1
}


reset_wifi()
{
    uci commit wireless
    if [ $? -ne 0 ]; then
        echo "TASK ERROR: set wireless error"
        exit 1
    fi
    echo "reseting wifi..."
    wifi
    if [ $? -ne 0 ]; then
        echo "TASK ERROR: reseting wifi failed"
        exit 1
    fi
}

set_ap()
{
    if [ -z $ssid ]; then
        usage
    fi
    uci set wireless.@wifi-iface[0].ssid=$ssid
    uci set wireless.@wifi-iface[0].mode=ap
    uci set wireless.@wifi-iface[0].encryption=none
    uci set wireless.@wifi-iface[0].key=$passwd
    uci set wireless.@wifi-iface[0].network=lan
    reset_wifi
    echo SUCCESS
    echo "AP MODE: SSID="${ssid}", NO PASSWD"
}

set_sta()
{
    if [ -z $ssid ]; then
        usage
    fi
    uci delete network.wan >/dev/null
    uci add network interface >/dev/null
    uci rename network.@interface[-1]='wan'
    uci set network.@interface[-1].ifname='wlan0'
    uci set network.@interface[-1].proto='dhcp'
    uci commit network
    if [ $? -ne 0 ]; then
        echo "TASK ERROR: set network error"
        exit 1
    fi
    uci set wireless.@wifi-iface[0].ssid=$ssid
    uci set wireless.@wifi-iface[0].mode=sta
    uci set wireless.@wifi-iface[0].encryption=psk2
    uci set wireless.@wifi-iface[0].key=$passwd
    uci set wireless.@wifi-iface[0].network=wan
    reset_wifi
    echo SUCCESS
    echo "STA MODE: SSID="${ssid}", PASSWD="${passwd}", ENCRYPTION=PSK2"
}

query_ip()
{
    ifconfig | grep 'inet addr:' | grep -v '127.0.0.1' | awk -F: '{print $2}' | awk '{print $1}'
}

main()
{
    case $proto in
    ap)
        set_ap
        ;;
    sta)
        set_sta
        ;;
    ip)
        query_ip
        ;;
	help)
        usage
        ;;
    *)
        usage
        ;;
    esac
}

main

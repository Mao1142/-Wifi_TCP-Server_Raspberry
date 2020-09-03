# Wifi_TCP-Server_Raspberry
樹苺派server端程式

## 使用VisualStudio編譯

1 - 於VisualStudioInstaller安裝Linux開發套件

2 - 於工具>選項>跨平台>連線管理員 加入或編輯樹苺派位置

## 不使用VisualStudio編譯
將.CPP檔複製到樹苺派

並新增makefile添加以下內容(位置檔名需修改)
## makefile

	CC = g++
	source = server.cpp
	obj = server.out
	location = /home/pi/

	all:$(source)
		$(CC) -o $(location)$(obj) $(location)$(source)
	run:
		sudo $(location)$(obj)
	clean:
		sudo rm -f $(location)$(obj)

## 使用方式
啟動命令列

輸入 make 編譯程式

輸入 make run 執行

## AP發送設定
IP設置為192.168.10.1 可修改 ，但不能跟有線網路同一子網域

### 安裝dnsmasq跟hostapd

	sudo apt-get install dnsmasq hostapd
	
### 設置DHCP

	sudo nano /etc/dhcpcd.conf

添加以下內容

	#DISABLE WLAN0 FROM DHCPCD AS WE ARE USING IT AS A WIFI ACCESS POINT 
	interface wlan0
	static ip_address=192.168.10.1/24
	nohook wpa_supplicant
	
### 設定靜態IP

	sudo nano /etc/network/interfaces

添加以下內容

	allow-hotplug wlan0
	#iface wlan0 inet static
	wireless-mode Master
	address 192.168.10.1
	netmask 255.255.255.0
	network 192.168.10.0
	broadcast 192.168.10.255
	#wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
 
### 設定AP資訊

	sudo nano /etc/hostapd/hostapd.conf

添加以下內容(ssid為熱點名稱wpa_passphrase為連線密碼)

	interface=wlan0

	# Use the nl80211 driver with the brcmfmac driver
	driver=nl80211

	# The name to use for the network
	ssid=OPTiMUM_00000000

	# Use the 2.4GHz band
	hw_mode=g

	# Use channel 6
	channel=6

	# Enable 802.11n
	ieee80211n=1

	# Enable WMM
	wmm_enabled=1

	# Enable 40MHz channels with 20ns guard interval
	ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]

	# Accept all MAC addresses
	macaddr_acl=0

	# Use WPA authentication
	auth_algs=1

	# Broadcast the network name
	ignore_broadcast_ssid=0

	# Use WPA2
	wpa=2

	# Use a pre-shared key
	wpa_key_mgmt=WPA-PSK

	# The WPA2 passphrase (password)
	wpa_passphrase=opt35525211

	# Use AES, instead of TKIP
	rsn_pairwise=CCMP
	
### 檢查熱點
(Ctrl+C停止 ， 非必要)

	sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf
	
### 變更配置文件路徑
	sudo nano /etc/default/hostapd
	
將

	#DAEMON_CONF=""
	
修改為

	DAEMON_CONF="/etc/hostapd/hostapd.conf"
	
### 配置DNSMASQ

	sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
	sudo nano /etc/dnsmasq.conf

添加以下內容

	interface=wlan0 # Use the require wireless interface – usually wlan0
	dhcp-range=192.168.10.2,192.168.10.200,255.255.255.0,24h
 
### 設置IP4轉發
以下操作為發送有線網路訊號，非必要

	sudo nano /etc/sysctl.conf
	
將

	#net.ipv4.ip_forward=1
	
修改為

	net.ipv4.ip_forward=1

輸入以下命令啟動(重新開機也會自動啟動)

	sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

輸入以下命令連接wlan0跟eth0

	sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
	sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
	sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT

設定為開機自動套用規則

	sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"

設定開機啟動熱點

	sudo nano /etc/rc.local

於exit0上方添加以下內容

	# Special for using wlan0 as an access point
	iptables-restore < /etc/iptables.ipv4.nat

啟用服務

	sudo service hostapd start
	sudo service dnsmasq start
	
(若hostapd開啟失敗可輸入以下命令強制關閉)

	sudo systemctl unmask hostapd 


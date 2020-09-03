# Wifi_TCP-Server_Raspberry
樹苺派server端程式

若不使用VisualStudio編譯 
將.CPP檔複製到樹苺派
新增makefile並添加以下內容(位置檔名需修改)
#makefile
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

#使用方式
啟動命令列
輸入 make 編譯程式
輸入 make run 執行

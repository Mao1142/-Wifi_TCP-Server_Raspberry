# Wifi_TCP-Server_Raspberry
樹苺派server端程式

##使用VisualStudio編譯
***
1 - 於VisualStudioInstaller安裝Linux開發套件

2 - 於工具>選項>跨平台>連線管理員 加入或編輯樹苺派位置
***
## 不使用VisualStudio編譯
將.CPP檔複製到樹苺派

並新增makefile添加以下內容(位置檔名需修改)
## makefile
***
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
***
## 使用方式
啟動命令列

輸入 make 編譯程式

輸入 make run 執行

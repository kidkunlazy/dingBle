# dingBle
修改esp32的蓝牙mac地址，模拟各种蓝牙设备  
并且还可以按照指定的raw内容来返回扫描结果  
比如模拟某钉的打卡机，实现蓝牙打卡

### 如何使用
首先，装一个叫 nRF Connect 的app，走到打卡机旁边，找到信号最强那个，记下他的mac地址，复制raw  
然后修改 dingBle.ino 中的 bleMac 和 bleRaw ，去掉最前面的0x，两个一组，前面加上0x，是不是很直观？  
还是不能理解？那就举个🌰：  
0x112233445566778899  
就会变成：  
{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99}  
  
然后用arduino开发环境编译上传到esp32就可以用了  
App需要新一点的版本，例如Android下需要4.7.5以上的，不然不会蓝牙打卡


### 协议 咩License
使用此项目视为您已阅读并同意遵守 [此LICENSE](https://github.com/zanjie1999/LICENSE)   
Using this project is deemed to indicate that you have read and agreed to abide by [this LICENSE](https://github.com/zanjie1999/LICENSE)   


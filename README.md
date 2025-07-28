# dingBle
修改esp32-c6的蓝牙mac地址，模拟各种蓝牙设备  
并且还可以按照指定的raw内容来返回扫描结果  
比如模拟某钉的打卡机，实现蓝牙打卡
仅在开发板esp32版本3.1.1上测试

### 如何使用
首先，装一个叫 nRF Connect 的app，走到打卡机旁边，找到信号最强那个，记下他的mac地址，复制raw  
然后修改 dingBle.ino 中的 bleMac 和 bleRaw ，去掉最前面的0x
请留意，不是啥mac地址都能自定义的，如果发现自定义mac后显示的还是设备原始mac，就说明这个mac地址无法自定义
自定义raw还是不能理解？那就举个🌰：  
0x112233445566778899  复制粘贴进arduino改成  112233445566778899
  
然后用arduino开发环境编译上传到esp32就可以用了  
App需要新一点的版本，例如Android下需要4.7.5以上的，不然不会蓝牙打卡

原项目地址(https://github.com/zanjie1999/dingBle)

### 协议 咩License
使用此项目视为您已阅读并同意遵守 [此LICENSE](https://github.com/zanjie1999/LICENSE)   
Using this project is deemed to indicate that you have read and agreed to abide by [this LICENSE](https://github.com/zanjie1999/LICENSE)   


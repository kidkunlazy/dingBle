/*
 * @file main.cpp
 * @brief 适用于 ESP32-C6 的传统蓝牙广播示例 (带自动RAW数据转换和自定义MAC)
 *
 * 本代码可以自动将一个16进制字符串转换为字节数据，并使用传统(Legacy)模式进行广播。
 * 您只需要在下方的“用户配置区”粘贴您的原始数据和MAC地址即可。
 *
 * 主要特性:
 * - 只需修改一个字符串即可更改广播的RAW数据。
 * - 代码在启动时自动将16进制字符串转换为字节数组。
 * - 自动判断数据长度，如果超过31字节，则自动分割到广播包和扫描响应包中。
 * - 使用传统广播模式，以获得最佳的设备兼容性。
 * - 通过底层API稳定地设置自定义MAC地址。
 */

// 检查芯片是否支持 BLE5 功能。ESP32-C6 支持。
#ifndef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
#error "This SoC does not support BLE5. This code requires an ESP32-C3, S3, C6, or H2."
#else

#include "BLEDevice.h"
#include "BLEAdvertising.h"
#include "esp_sleep.h"
#include "esp_mac.h" // **修正**: 引入底层API头文件以设置基础MAC地址
#include <vector>   // 用于动态数组
#include <string>   // 用于字符串处理
#include <cstdlib>  // 用于字符串到数字的转换

// =================================================================
// ---                       用户配置区                          ---
// =================================================================

// 1. 在这里粘贴您的16进制原始数据 (不含0x，不含空格)
const char* rawHexDataString = "3142546465634635452";

// 2. 在这里设置您希望用于此广播的自定义MAC地址
uint8_t bleMac[6] = {0x12, 0x22, 0x33, 0x44, 0x55, 0x66};

// =================================================================
// ---                     代码实现 (无需修改)                    ---
// =================================================================

// 指向广播对象的指针
BLEAdvertising *pAdvertising = nullptr;

/**
 * @brief 将16进制字符串转换为字节向量的辅助函数
 * @param hex 输入的16进制字符串
 * @return 包含转换后字节的 std::vector<uint8_t>
 */
std::vector<uint8_t> hexStringToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    bytes.reserve(hex.length() / 2);
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

void setup() {
  Serial.begin(115200);
  Serial.println("正在启动带自动数据转换和自定义MAC的 BLE 传统广播...");

  // **修正**: 在初始化任何网络堆栈之前，设置基础MAC地址。
  // 这会影响WiFi和蓝牙的地址。
  esp_err_t err = esp_base_mac_addr_set(bleMac);
  if (err == ESP_OK) {
    Serial.println("基础MAC地址设置成功。设备将使用由此衍生的地址。");
  } else {
    Serial.printf("基础MAC地址设置失败, 错误码: %d\n", err);
  }

  // 初始化BLE，此时它会使用我们刚刚设置的新基础MAC地址来派生自己的地址
  BLEDevice::init("");
  
  // 获取全局广播对象
  pAdvertising = BLEDevice::getAdvertising();
  
  // (不再需要设置地址类型，默认使用公共地址，而该地址已受我们设置的基础MAC影响)

  // --- 自动转换并分割原始数据 ---
  Serial.println("正在将16进制字符串转换为字节...");
  std::vector<uint8_t> combinedRawData = hexStringToBytes(rawHexDataString);
  Serial.printf("转换完成，共 %d 字节。\n", combinedRawData.size());

  // 创建广播数据和扫描响应数据对象
  BLEAdvertisementData advertisementData;
  BLEAdvertisementData scanResponseData;

  // 检查数据长度并自动分割
  if (combinedRawData.size() > 31) {
      Serial.println("数据超过31字节，将自动分割到扫描响应包中。");
      
      // 前31字节放入广播包
      std::string adv_part(combinedRawData.begin(), combinedRawData.begin() + 31);
      advertisementData.addData(String(adv_part.c_str(), adv_part.length()));
      
      // 剩余部分放入扫描响应包
      std::string scan_rsp_part(combinedRawData.begin() + 31, combinedRawData.end());
      scanResponseData.addData(String(scan_rsp_part.c_str(), scan_rsp_part.length()));

  } else {
      Serial.println("数据未超过31字节，全部放入广播包中。");
      advertisementData.addData(String((char*)combinedRawData.data(), combinedRawData.size()));
  }

  // 应用配置好的数据
  pAdvertising->setAdvertisementData(advertisementData);
  if (scanResponseData.getPayload().length() > 0) {
      pAdvertising->setScanResponseData(scanResponseData);
  }

  // 启动广播
  pAdvertising->start();

  Serial.println("传统广播已启动。");
}

void loop() {
  // 闪灯灯 至于为什么是串口输出，因为并没有内置led，但拥有串口指示灯
  Serial.println("Sparkle");
  delay(1000);

  // 20分钟 (1,200,000 毫秒) 后进入深度睡眠以节省电量
  if (millis() > 1200000) {
    Serial.println("20分钟已到，进入深度睡眠。");
    
    pAdvertising->stop();

    esp_deep_sleep_start();
  }
}

#endif // CONFIG_BT_BLE_50_FEATURES_SUPPORTED

# OBD-KLINE
Open-source OBD-II emulator based on an ESP32 + KLINE transceiver IC (xl33290).  

The circuit board drawing is made by using lceda:
https://lceda.cn/editor

<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/front-side.jpg' style='width:250px; float:left'></img>
<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/back-side.jpg' style='width:250px; float:left;margin-left:-6px'></img><br/>

<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/physical-pic.jpg' style='width:250px; float:left'></img>
<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/obd-interface.jpg' style='width:250px; float:left;margin-left:-6px'></img><br/>

<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/daytime.jpg' style='width:250px; float:left'></img>
<img src='https://github.com/Zi-x/OBD-KLINE/blob/main/picture/night.jpg' style='width:250px; float:left;margin-left:-6px'></img><br/>

# Supported protocols
--ISO14230

# Common commands
{0xC1, 0x33, 0xF1, 0x81, 0x66};        // ISO14230 - fast init  
{0xC2, 0x33, 0xF1, 0x01, 0x0C, 0xF3};   // 引擎转速   
{0xC2, 0x33, 0xF1, 0x01, 0x05, 0xEC}; // 发动机冷媒温度   
{0xC2, 0x33, 0xF1, 0x01, 0x0D, 0xF4};    // 车速   
{0xC2, 0x33, 0xF1, 0x01, 0x11, 0xF8}; // 油门位置   
{0xC2, 0x33, 0xF1, 0x01, 0xA4, 0x8B}; // 变速箱挡位  
{0xC2, 0x33, 0xF1, 0x01, 0x0F, 0xF6}; // 进气温度   
{0xC2, 0x33, 0xF1, 0x01, 0x2F, 0x16}; // 燃油剩余百分比 

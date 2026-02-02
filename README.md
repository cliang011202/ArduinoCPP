# RTOS-Based Motor Control System (Arduino UNO)

## 项目简介
本项目基于 Arduino UNO + FreeRTOS，实现了一个典型的嵌入式电机控制系统原型。
系统采用 RTOS 架构，将通信、控制、日志等功能解耦，具备良好的实时性和可扩展性。

## 系统架构
- CommandTask：接收串口命令，生成控制意图
- MotorController：周期性执行电机控制（10ms）
- LoggerTask：异步日志输出
- RTOS 机制：Task / Queue / Semaphore / EventGroup

## 主要特性
- 周期性电机控制任务（vTaskDelayUntil）
- 使用 Queue 传递控制命令，避免参数竞争
- 使用 Mutex 保护串口资源
- 面向对象封装电机控制模块
- 支持系统状态扩展（EventGroup）

## 控制方式
- 串口输入 '1'：启动电机
- 串口输入 '0'：停止电机
- 串口输入 '0'~'9'：设置 PWM 占空比

## 硬件平台
- Arduino UNO
- PWM 控制输出（示例使用 13 号引脚）

## 后续可扩展方向
- 加入 PID 控制
- 加入电流 / 速度反馈
- 增加安全保护与故障状态机
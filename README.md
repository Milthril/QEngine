# QEngine

- 心源 Studio

QEngine是一个简易的开源3D引擎

>它的名称是因为开发过程极大程度受益于Qt（反射+RHI+编辑器）

它的设计注重——高可读性，且不膨胀的代码。但这也意味着较少的性能优化，所以它主要用于引擎架构及渲染的学习与尝试。

## 编辑器

![](./Doc/src/debug.gif)

## 可执行程序

把**EXE.rar**解压到当前文件夹下，其中Example.exe为不包含编辑器的Demo

## 构建环境

- Qt 6.2.3以上版本：在线下载器：https://download.qt.io/archive/online_installers，安装过程中请勾选
  - Qt 6.2.3+
    - MinGW 64 bit
    - MSVC2019 64-bit 
    - Sources：Qt源码
    - Qt 5 Compatibility Module：Qt6对Qt5的支持
    - Qt Shader Tools：Qt的着色器转换工具源码，用于运行时编译Shader
    - Qt Debug Information Files（MSVC调试Qt源码的PDB文件，比较大，可不勾选）

  - Developer and Designer Tools（一般默认即可）
    - Debugging Tools for Windows
    - CMake
    - Ninjia

- Vulkan SDK
- MSVC 2019 

## 涵盖知识点

 子模块已实现，待引入。

- 反射：编辑器核心
  - moc 原理及扩展（用于Lua绑定）。
  - 基于反射的自动序列化。
- 场景元素。
  - 静态网格
    - 网格化 2D/3D 文字。
    - 静态模型。
  - 骨骼模型
  - 天空盒
  - GPU粒子
  - 地形
  - 光源
  - 相机
- 渲染架构：
  - 渲染代理（Proxy）
  - Uniform管理及模块化Shader：用于动态材质 及 GPU 粒子的动态效果
  - 可自定义的渲染器（前向，延迟）
  - Debug管线，支持鼠标拾取，Zmo组件
- 后期效果
  - Bloom：模仿UE中基于HSV色域的Bloom效果
  - 景深
- 音频
  - Windows 声卡采集
  - 频谱分析、可视化

- 编辑器
  - 样式管理
  - Dock窗口及布局管理
  - 基于反射的编辑器架构
  - Lua IDE
  - GLSL IDE （材质、粒子）

### 待完善

- 渲染架构：FrameGraph

- 逻辑架构：EC vs ECS

- 动画模块：以MMD为主

- 资产管理：将运行时的

- 工程管理：支持场景导出

- 视频编码导出

- 撤销、重做管理

  

  

  

  

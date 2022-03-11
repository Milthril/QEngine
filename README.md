# QEngine

QEngine是一个基于Qt（反射+RHI+编辑器）的简易开源3D引擎，它的设计注重——高可读性，且不膨胀的代码。但这也意味着较少的性能优化，所以它主要用于引擎架构及渲染的学习与尝试。

QEngine中涵盖以下知识：

- 反射：编辑器核心
  - moc 自定义扩展（用于Lua绑定）
  - 基于反射的自动序列化
- 场景元素
  - 静态网格
    - 网格化 2D/3D 文字
    - 静态模型
  - 天空盒
  - 骨骼模型
  - GPU粒子
  - 地形
  - 光源
  - 相机
- 渲染架构：
  - 渲染代理（Proxy）
  - Uniform管理及模块化Shader：用于动态材质 及 GPU 粒子的动态效果
  - 可自定义的渲染器（前向，延迟）
- 后期效果
  - Bloom：模仿UE中基于HSV色域的Bloom效果
  - 景深：
    - 静态网格
    - 骨骼动画
- 编辑器
  - 基于反射自动创建编辑面板
  - Lua IDE
  - GLSL IDE （材质、粒子）
  - 资产管理
  - 工程管理
  - 撤销、重做

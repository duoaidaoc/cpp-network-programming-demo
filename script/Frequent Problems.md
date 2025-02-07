1. 按`ctrl`+ `,`打开设置，记得把`C_Cpp.intelliSenseEngine` 设置为 `default`，这是插件`c/c++`的自动补全功能的开关，一定开着。
2. c++ 多文件在`include`时，用到其他类，最好是写`前向引用`然后在cpp文件中`include`，防止循环依赖。
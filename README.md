# TMediaPlayer


**TMediaPlayer** 是基于Qt 5.8 与 qt-vlc库的简易播放器。

## 应好未来面试官 要求制作

## 支持平台：
WIndows 7/8/10

## 编译步骤：
* 安装qt5.8，设置好相关环境变量
* 找到.pro文件，在当前文件夹下运行qmake,生成MakeFile.Debug与MakeFile.Release文件，以及Debug与Release目录
* 运行 mingw32-make.exe -f MakeFile.Debug  在Debug目录中生成Debug版程序
* 运行 mingw32-make.exe -f MakeFile.Release  在Release目录中生成Release版程序。
* 要成功运行可执行程序，首先需要将 $$PWD/vlc/bin目录下的动态库与可执行程序放在同一目录下，或者添加环境变量。（$$PWD为.Pro文件所在目录）
* 然后用qt自带工具查找并拷贝其他依赖库，运行该命令:  windeployqt -xxx.exe, 但注意该工具可能会添加过多多余的动态库。



#TMediaPlayer_release.7z为发布后免安装压缩包。因为时间关系，动态库没有有效精简，暂时较大。


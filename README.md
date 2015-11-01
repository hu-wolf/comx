# 这是什么
这是一个基于windows控制台的串口调试助手,内部集成了超级终端,蓝牙模块设置等功能,可用于基于串口的通信,数据采集等场合.

# 为什么要做这个软件
市面上的串口调试助手很多,其中也有集成了很多功能的,但它们大都无法按需求进行拓展或启动速度慢,造成前者的原因是它们都只有一个.exe文件,而造成后者的原因是它们有着华丽的GUI.为了克服这两个缺点,我制作了这个软件,它的可拓展性体现在你可以拥有并修改它的源代码,它启动迅速是因为它基于控制台.虽然没有GUI的华丽,但它却有着强大的内心.

# 如何使用软件
1.超级终端与数据接收串口参数设置格式:波特率-校验模式-数据长度-停止位长度
  校验模式:N(无校验),O(奇校验),E(偶校验)
  数据长度:4-8之间的任意数字
  停止位长度:1,2对应1个停止位和2个停止位,输入其它数字将处理为1.5个停止位
  
  输入示例:115200-N-8-1  9600-E-6-2

2.收文件的数据起始符为Dst,结束符为Ded,段结束符为#(该协议可以在代码中修改)

3.HC-05串口设置格式:波特率,停止位,校验模式
  校验模式:0为无校验,其余数字不明
  停止位:0为一个停止位,其余数字不明
  输入示例:115200,0,0
  
  Warning:请勿设置高于256000的波特率(256000是windows串口的极限波特率),并且当波特率超过115200后传输将变得相当不稳定

4.在设置HC-06的时候,因为芯片原因,在每发出一个指令后都请耐心等待芯片的回复,
  若发出一个指令10s后仍未收到回复,则需要重启芯片,或者芯片已损坏
  
  Warning:不要狂按发出指令的按键,这可能导致设置失败
  
5.根据数据导出图片的功能暂不可用

# 如何添加功能
编写好完整的功能后,再为其编写驱动函数(原型统一为无返回值和形参),并将其加入Menu()中,如果是一个全新的子菜单,也将其加入Menu()中,详细请看代码中Menu.c中Menu()函数的实现与调用.若代码的独立性很差,添加至Menu()中将会出现问题.所以请重视高内聚,低耦合的内在含义!

# 更多的话(To 学弟学妹)
这是我写的第一个真正意义上的上位机,也是我上可写上位机,下可写下位机的标志.希望大家能共同维护好这份代码,或者编写出更加优秀的上位机软件!

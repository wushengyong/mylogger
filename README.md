# 我的日志程序

功能 
1. 支持多线程
2. .cpp跟.h，没有其他依赖
3. 支持动态读取配置，便于调试
4. 封装不同类型的日志输出
5. 仅输出到文件

# 使用方法
1. 根据自己的需要设置 Logger.h 中的宏 LOG_SETTING_CONF_FILE 及 LOG_SETTING_APP_NAME
2. 读取不到配置文件会按照默认配置执行
3. 配置文件中的日志输出的目录必须存在

# 配置文件的配置方法
1. Level表示当前允许输出的级别：Debug, Info, Warn, Error
2. Reload表示再次读取配置文件的时间间隔，0表示不重新读取， >0表示隔X秒再次读取
3. LogDir指定日志的输出目录

[OxLand]
Level=Debug
Reload=10
LogDir=D:\360安全浏览器下载\LoggerTest\Debug\Log\OxLand

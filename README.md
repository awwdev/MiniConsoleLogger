# MiniConsoleLogger
Tiny console logger I am using for my games. It includes: threadname, timestamp, source and line, log level, and formates the message according to the parameters given. MT and colors are possible.

Using C++17.

![alt text](https://github.com/awwdev/MiniConsoleLogger/blob/master/MiniLoggerImg.png)

LOG_INFO("This is foo: {} and this is bar {}, sir.", 42, "baz")
LOG_WARN("{}{}{}", "warning")
LOG_ERR("unexpected error")

--目的：使用打开文件窗开打卡一个文件流

-- 获取一张贴图文件
fileName = getOpenFileName types:"TGA(*.tga)|*.tga|Bitmapl(*.bmp)|*.bmp|All|*.*|"

-- 获得文件流
fileStream = openFile fileName 

-- 读文件流中的一行
readline fileStream
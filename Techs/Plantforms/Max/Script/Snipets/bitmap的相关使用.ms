-- 目的：bitmap的相关使用

-- 获取一张贴图文件
fileName = getOpenFileName types:"TGA(*.tga)|*.tga|Bitmapl(*.bmp)|*.bmp|All|*.*|"

-- 打开bitmap
tex = openbitmap fileName 

-- 显示bitmap
display tex

-- 获得该bitmap的宽高
nWidth = tex.width
nHeight = tex.height

-- 判断该tex是否有alpha通道
bHasAlpha = tex.hasAlpha


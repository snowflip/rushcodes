-- 目的：使用mesh相关函数创建一个box

-- 填充顶点数组
vertArray = #();
vertArray.count=8
vertArray[1]= [-1,-1,1]
vertArray[2]= [1,-1,1]
vertArray[3]= [1,-1,-1]
vertArray[4]= [-1,-1,-1]
vertArray[5]= [-1,1,1]
vertArray[6]= [1,1,1]
vertArray[7]= [1,1,-1]
vertArray[8]= [-1,1,-1]

-- 填充face数组
--face面是逆时针构建的
faceArray=#()
faceArray.count=3
faceArray[1]=[6,5,1]
faceArray[2]=[6,1,2]
faceArray[3]=[7,6,2]
faceArray[4]=[7,2,3]

faceArray[5]=[8,7,3]
faceArray[6]=[8,3,4]
faceArray[7]=[8,4,5]
faceArray[8]=[5,4,1]

faceArray[9]=[8,5,6]
faceArray[10]=[8,6,7]
faceArray[11]=[4,2,1]
faceArray[12]=[4,3,2]

-- 创建一个mesh
mm = mesh numverts:8 numfaces:12

-- 设置mesh的顶点
for i = 1 to 8 do 
	meshop.setvert mm i vertArray[i]

-- 创建mesh的面
for i = 1 to 12 do 
	meshop.createPolygon mm #(faceArray[i].x,faceArray[i].y,faceArray[i].z) 

--更新mesh
update mm

@echo On
@Rem 删除SVN版本控制目录
@ 另外一个不需要删除.svn的方法是使用export命令，就能导出纯净的文件了。
@PROMPT [Com]

@for /r . %%a in (.) do @if exist "%%a\.svn" rd /s /q "%%a\.svn"
@Rem for /r . %%a in (.) do @if exist "%%a\.svn" @echo "%%a\.svn"

@echo Mission Completed.
@pause
自动生成注释工具

1.enviroment:
	ubuntu12.04
	sudo apt-get install dexygen
	plugin:DoxygenToolkit.vim --->放在/usr/share/vim/vimxx/plugin下
	.vimrc --->放在当前用户home目录下

2.执行 dexygen testconfig
	在浏览器上打开doc/html/index.html查看

3.DoxygenToolkit插件使用说明
	在vim中,支持以下命令:
		Dox --->DoxygenCommentFunc
		DoxLic --->DoxygenLicenseFunc()
		DoxAuthor --->DoxygenAuthorFunc()
		DoxUndoc --->DoxygenUndocumentFunc()
		DoxBlock --->DoxygenBlockFunc()
4. .vimrc中的配置可支持在vim创建文件时，自动生成注释


注意:运行生成的文件不要上传.


"当新建 .h .c .hpp .cpp 等文件时自动调用SetTitle 函数
autocmd BufNewFile *.[ch],*.hpp,*.cpp exec ":call SetTitle()" 

" 加入注释 
func SetComment()
	call setline(1,"/***********************************************************************") 
	call append(line("."),   "*   Copyright (C) ".strftime("%Y")." pilot-lab.inc All rights reserved.")
	call append(line(".")+1, "*   ") 
	"call append(line(".")+2, "*   @file:"."       ".input("input file name: ")) 
	call append(line(".")+2, "*   @file:"."       ".expand("%:t")) 
	call append(line(".")+3, "*   @brief:"."      ") 
	call append(line(".")+4, "*   @author:"."     Pilot labs")
	call append(line(".")+5, "*   @maintainer:"." frank.fu@pilot-lab.com.cn")
	call append(line(".")+6, "*   @version:"."    1.0") 
	call append(line(".")+7, "*   @date:".strftime("       %Y-%m-%d")) 
	call append(line(".")+8, "*   ")
	call append(line(".")+9, "***********************************************************************/") 
endfunc

"定义函数SetTitle，自动插入文件头 
func SetTitle()
	call SetComment()
	if expand("%:e") == 'hpp' 
		call append(line(".")+10, "#ifndef _".toupper(expand("%:t:r"))."_H") 
		call append(line(".")+11, "#define _".toupper(expand("%:t:r"))."_H") 
		call append(line(".")+12, "#ifdef __cplusplus") 
		call append(line(".")+13, "extern \"C\"") 
		call append(line(".")+14, "{") 
		call append(line(".")+15, "#endif") 
		call append(line(".")+16, "") 
		call append(line(".")+17, "#ifdef __cplusplus") 
		call append(line(".")+18, "}") 
		call append(line(".")+19, "#endif") 
		call append(line(".")+20, "#endif //".toupper(expand("%:t:r"))."_H") 
	elseif expand("%:e") == 'h' 
		call append(line(".")+10, "#ifndef _".toupper(expand("%:t:r"))."_H") 
		call append(line(".")+11, "#define _".toupper(expand("%:t:r"))."_H") 
		call append(line(".")+12, "#ifdef __cplusplus") 
		call append(line(".")+13, "extern \"C\"") 
		call append(line(".")+14, "{") 
		call append(line(".")+15, "#endif") 
		call append(line(".")+16, "") 
		call append(line(".")+17, "#ifdef __cplusplus") 
		call append(line(".")+18, "}") 
		call append(line(".")+19, "#endif") 
		call append(line(".")+20, "#endif // _".toupper(expand("%:t:r"))."_H")
		"call append(line(".")+10, "#pragma once") 
	elseif &filetype == 'c' 
		call append(line(".")+10,"#include \"".expand("%:t:r").".h\"") 
	elseif &filetype == 'cpp' 
		call append(line(".")+10, "#include \"".expand("%:t:r").".h\"") 
	endif
endfunc

let g:DoxygenToolkit_briefTag_structName ="@brief"
let g:DoxygenToolkit_briefTag_pre="@brief    "
let g:DoxygenToolkit_paramTag_pre="@param[in]    "
let g:DoxygenToolkit_returnTag="@return    "
"let g:DoxygenToolkit_blockHeader="--------------------------------------------------------------------------"
"let g:DoxygenToolkit_blockFooter="----------------------------------------------------------------------------"
"let g:DoxygenToolkit_authorName="frank"
"let g:DoxygenToolkit_licenseTag="GPL 2.0"

let g:DoxygenToolkit_authorName="    frank.fu@pilot-lab.com.cn"

let s:licenseTag = "*********************************************\<enter>"
let s:licenseTag = s:licenseTag . "Copyright(C) 2016 pilot-lab.inc\<enter>"
let s:licenseTag = s:licenseTag . "All right reserved.\<enter>"
let g:DoxygenToolkit_licenseTag = s:licenseTag
let g:DoxygenToolkit_briefTag_funcName="yes"
let g:doxygen_enhanced_color=1




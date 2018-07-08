"auto add comment header --start
autocmd BufNewFile *.py 0r ~/.vim/vim_templation/vim_python_header
autocmd BufNewFile *.c 0r ~/.vim/vim_templation/vim_c_header
autocmd BufNewFile *.h 0r ~/.vim/vim_templation/vim_ch_header
autocmd BufNewFile *.sh 0r ~/.vim/vim_templation/vim_shell_header
autocmd BufNewFile *.py,*.[ch],*.sh ks|call FileName_CreatedTime()|'s
autocmd BufNewFile *.h ks|call AddchMacro()|'s

fun FileName_CreatedTime()
    exe "g/File Name:/s//File Name: " .expand("%")
    exe "g/Created Time:.*/s//Created Time:" .strftime("%Y-%m-%d %T")
    exe "g/2017/s//".strftime("%Y")
endfun

fun AddchMacro()
    exe "g/MmyFileName/s//" .expand("%:r")
endfun
"auto add header --end

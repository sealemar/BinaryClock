"
" developed by Sergey Markelov (11/10/2013)
"

let s:projectDir = expand('<sfile>:p:h')

"
" Per language setup
"
function! s:initC(type)
"    setlocal notagrelative
"    setlocal tags+=$HOME/.cache/vim-specific/tags/c-home-include
    execute "setlocal path+=" . s:projectDir . "/include"
    execute "setlocal path+=" . s:projectDir . "/lib"

    execute "setlocal tags+=" . s:projectDir . "/etc/tags"

    if a:type == 'arduino'
        execute "setlocal tags+=" . s:projectDir . "/etc/arduino.tags"
        execute "setlocal tags+=" . s:projectDir . "/etc/lib.tags"
    elseif a:type == 'emulator'
        execute "setlocal tags+=" . s:projectDir . "/etc/emulator.tags"
        execute "setlocal tags+=" . s:projectDir . "/etc/lib.tags"
    elseif a:type == 'test'
        execute "setlocal tags+=" . s:projectDir . "/etc/test.tags"
        execute "setlocal tags+=" . s:projectDir . "/etc/lib.tags"
    elseif a:type == 'lib'
        execute "setlocal tags+=" . s:projectDir . "/etc/lib.tags"
    endif
endfunction

execute "autocmd BufEnter " . s:projectDir . "/arduino/*.[ch] call <SID>initC('arduino')"
execute "autocmd BufEnter " . s:projectDir . "/emulator/*.[ch] call <SID>initC('emulator')"
execute "autocmd BufEnter " . s:projectDir . "/test/*.[ch] call <SID>initC('test')"
execute "autocmd BufEnter " . s:projectDir . "/lib/*.[ch] call <SID>initC('lib')"
execute "autocmd BufEnter " . s:projectDir . "/include/*.[ch] call <SID>initC('')"

"
" Ctags files rebuild
"
execute "autocmd BufWritePost " . s:projectDir . "/arduino/*.[ch] silent !make -C " . s:projectDir . "/arduino ctags"
execute "autocmd BufWritePost " . s:projectDir . "/emulator/*.[ch] silent !make -C " . s:projectDir . "/emulator ctags"
execute "autocmd BufWritePost " . s:projectDir . "/test/*.[ch] silent !make -C " . s:projectDir . "/test ctags"
execute "autocmd BufWritePost " . s:projectDir . "/lib/*.[ch] silent !make -C " . s:projectDir . "/lib ctags"
execute "autocmd BufWritePost " . s:projectDir . "/include/*.[ch] silent !make -C " . s:projectDir . "/include ctags"

"
" Session managemenr
"
function! s:saveSession()
    execute "silent !mkdir -p " . s:projectDir . "/etc"
    if filereadable(s:sessionFile)
        execute "silent !mv " . s:sessionFile . " " . s:sessionOldFile
    endif
    execute "mksession " . s:sessionFile
endfunction

function! s:restoreSession()
    if filereadable(s:sessionFile)
        execute "source " .s:sessionFile
    endif
endfunction

set sessionoptions=buffers,curdir,folds,globals,help,localoptions,options,resize,slash,tabpages,unix,winpos,winsize
let s:sessionFile = s:projectDir . '/etc/Session.vim'
let s:sessionOldFile = s:projectDir . '/etc/Session.old.vim'
autocmd VimLeave * call <SID>saveSession()
autocmd VimEnter * call <SID>restoreSession()

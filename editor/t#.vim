" Vim syntax file
" Language: Tsharp

" Usage Instructions
" Put this file in .vim/syntax/tsharp.vim
" and add in your .vimrc file the next line:
" autocmd BufRead,BufNewFile *.t# set filetype=tsharp

if exists("b:current_syntax")
  finish
endif

syntax keyword tsharpTodos TODO XXX FIXME NOTE

" Language keywords
syntax keyword tsharpKeywords func if elif else while do for

" Comments
syntax region tsharpCommentLine start="//" end="$"   contains=tsharpTodos
           
" Strings
syntax region tsharpString start=/\v"/ skip=/\v\\./ end=/\v"/
syntax region tsharpString start=/\v'/ skip=/\v\\./ end=/\v'/

" Set highlights
highlight default link tsharpTodos Todo
highlight default link tsharpKeywords Identifier
highlight default link tsharpCommentLine Comment
highlight default link tsharpString String

let b:current_syntax = "tsharp"
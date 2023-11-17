if exists("b:current_syntax")
	finish
endif

set iskeyword=a-z,48-57,A-Z,-,*,_,!,@
syntax keyword blackTodos TODO

" Language keywords
syntax keyword blackKeywords if
syntax keyword blackKeywords else
syntax keyword blackKeywords while
syntax keyword blackKeywords do
syntax keyword blackKeywords inc
syntax keyword blackKeywords end
syntax keyword blackKeywords cast
syntax keyword blackKeywords macro
syntax keyword blackKeywords syscall1
syntax keyword blackKeywords syscall2
syntax keyword blackKeywords syscall3
syntax keyword blackKeywords syscall4
syntax keyword blackKeywords syscall5
syntax keyword blackKeywords syscall6

" Comments
syntax region blackCommentLine start="//" end="$" contains=blackTodos

" String literals
syntax region blackString start=/\v"/ skip=/\v\\./ end=/\v"/ contains=blackEscapes

" Char literals
syntax region blackChar start=/\v'/ skip=/\v\\./ end=/\v'/ contains=blackEscapes

" Escape literals
syntax match blackEscapes display contained "\\[nr\"']"

" Number literals
syn match blackNumber "<\d+>"
syntax region blackNumber start=/\d/ skip=/\d/ end=/\s/

" Type names the compiler recognizes
syntax keyword blackTypeNames int
syntax keyword blackTypeNames ptr
syntax keyword blackTypeNames bool

" Boolean literals
syn keyword blackBoolean true
syn keyword blackBoolean false


" set highlights
highlight default link blackTodos Todo
highlight default link blackCommentLine Comment
highlight default link blackString String
highlight default link blackNumber Number
highlight default link blackFloat Float
highlight default link blackBoolean Boolean
highlight default link blackKeywords Keyword
highlight default link blackTypeNames Type
highlight default link blackChar Character
highlight default link blackEscapes SpecialChar

let b:current_syntax = "black"

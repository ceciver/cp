set nocompatible
set encoding=utf-8

set cin aw ai is hls ts=4 sw=4 sts=4 tm=50
set nu rnu
set noeb
set bg=dark
set ru cul
set so=3
set nowrap
set hidden
set backspace=indent,eol,start

syntax on
filetype plugin indent on

inoremap jk <Esc>
inoremap kj <Esc>
nnoremap ; :

inoremap {<CR> {<CR>}<Esc>O

set makeprg=g++\ -std=gnu++17\ -O2\ -g\ -Wall\ -Wextra\ -Wshadow\ -fsanitize=address\,undefined\ -DLOCAL\ %\ -o\ %<

nnoremap <F9> :w<CR>:make<CR>
nnoremap <F10> :!./%<<CR>

autocmd QuickFixCmdPost make cwindow


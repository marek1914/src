set nocp
set bs=2 "for vim8
set ts=4
set sw=4
set nu
colorscheme koehler
syntax on "for macos
set cot=menuone,preview
filetype plugin indent on
set pvh=1

set dict+=~/.vim/other/dict/3k
set dict+=~/.vim/other/dict/c
set dict+=~/.vim/other/dict/.phrase
set dict+=~/.vim/other/dict/zh/zh

"bad ~/.vim/other/dict/zh/*

set rtp+=~/.vim/other
set rtp+=~/.vim/tagbar
set rtp+=~/.vim/ultisnips
set rtp+=~/.vim/vim-snippets
set rtp+=~/.vim/lilypond
set rtp+=~/.vim/omnicppcomplete
set rtp+=~/.vim/nerdtree
set rtp+=~/.vim/pydict/after

set tags+=~/.vim/other/sysinc

set cpt=.,w,b,u,k,t,U
set hls
"set fdm=syntax

let g:netrw_banner=0
"let g:netrw_browse_split=4
"let g:netrw_winsize=25
"let g:netrw_liststyle=3
nmap <F8> :Tagbar<CR>
nmap <F10> :NERDTree<CR>
map  <F3> :Trans<CR>
imap jk <Esc>
"imap /* /*   */<ESC>hhhi

" highlight Pmenu    guibg=darkgrey  guifg=black 
" highlight PmenuSel guibg=lightgrey guifg=black

map <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>
let g:pydict = '~/.vim/pydict/dict'


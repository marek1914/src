ln -sf $PWD/base/snippet/ ~/.vim/vim-snippets/UltiSnips
ln -sf $PWD/vim/vimrc ~/.vimrc
ln -sf $PWD/docs/txt/vihelp/ ~/.vim/doc

cd base/sysinc
./ctags.sh

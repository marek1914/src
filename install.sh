# 若不删除，下面的 -f 最造成目录链接的嵌套，奇怪
rm ~/.vim/vim-snippets/UltiSnips
rm ~/.vim/doc

ln -sf $PWD/base/snippet/ ~/.vim/vim-snippets/UltiSnips
ln -sf $PWD/vim/vimrc ~/.vimrc
ln -sf $PWD/docs/txt/vihelp/ ~/.vim/doc

cd base/sysinc
./ctags.sh

# 若不删除，下面的 -f 最造成目录链接的嵌套，奇怪
rm ~/.vim/vim-snippets/UltiSnips
rm ~/.vim/doc

ln -sf $PWD/base/snippet/ ~/.vim/vim-snippets/UltiSnips
ln -sf $PWD/vim/vimrc ~/.vimrc
ln -sf $PWD/docs/txt/vihelp/ ~/.vim/doc
ln -sf $PWD/vim/most3k ~/.vim/other/most3k
ln -sf $PWD/vim/zh ~/.vim/other/zh
cd base/sysinc
./ctags.sh

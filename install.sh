# 若不删除，下面的 -f 最造成目录链接的嵌套，奇怪
rm ~/.vim/vim-snippets/UltiSnips
rm ~/.vim/other/dict
rm ~/.vim/doc

ln -sf $PWD/base/snippet/ ~/.vim/vim-snippets/UltiSnips
ln -sf $PWD/txt/vihelp/ ~/.vim/doc
ln -sf $PWD/txt/dict/ ~/.vim/other/dict
ln -sf $PWD/vimrc ~/.vimrc

tr ' ' '_' < txt/dict/phrase > txt/dict/.phrase
cd base/sysinc
./ctags.sh

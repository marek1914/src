#ctags --list-kinds
ctags -R --c-kinds=+p
mv tags ~/.vim/other/sysinc

# Code Editors
Here's a list of syntax highlighting schemes for editors you may use.

## Installation
Check here to know how to install for your editor

### Vim
Save `black.vim` as `~/.config/vim/syntax/black.vim`. If you use something like ``nvim`` or other Vim implementation, change the second directory in the path to your editor's directory.

Open `~/.vimrc` (or your `init.vim` file if you have it set up) and add the following:
```vim
autocmd BufRead,BufNewFile *.bk set filetype=black
```

### Other
*Coming soon*

dconf write /org/gnome/desktop/input-sources/xkb-options "['caps:escape']"
dconf write /org/gnome/desktop/input-sources/xkb-options "['caps:escape']"
gsettings set org.gnome.desktop.input-sources xkb-options "['caps:escape']"
setxkbmap -option '' -option caps:escape
setxkbmap -layout us -option '' -option caps:escape
xmodmap -e "clear Lock"
xmodmap -e "keycode 66 = Escape"

# above only once on terminal

alias cmp='g++ -std=gnu++17 -O2 -g -Wall -Wextra -Wshadow -fsanitize=address,undefined -DLOCAL'


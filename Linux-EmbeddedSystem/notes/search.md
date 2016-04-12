
find

grep -rnwl '/path/to/somewhere/' -e "pattern"

grep --include=\*.{c,h} -rnw '/path/to/somewhere/' -e "pattern"

grep --exclude=\*.o -rnw '/path/to/somewhere/' -e "pattern"

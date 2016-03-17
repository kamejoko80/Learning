# Cooperation

* check Whitespace  
$ git diff --check

* $ git fetch origin  
//$ git checkout master  
$ git merge origin/master  
$ git push origin master

* heve make somechang in branch issue54  
$ git fetch origin  
$ git log --no-merge origin/master ^issue54  
$ git checkout master  
$ git merge issue54  
$ git merge origin/master  
$ git push origin master

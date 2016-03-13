#!/usr/bin/env bash

#add all the files
git add -A

#commit with a defult message
echo "Default message: $1"
git commit -m "$1"

#push 
git push origin master

#在git中如果想忽略掉某个文件， 不让这个文件提交到版本库中，可以使用修改 .gitignore 文件的方法。
#*.a       # 忽略所有 .a 结尾的文件
#!lib.a    # 但 lib.a 除外
#/TODO     # 仅仅忽略项目根目录下的 TODO 文件，不包括 subdir/TODO
#build/    # 忽略 build/ 目录下的所有文件
#doc/*.txt # 会忽略 doc/notes.txt 但不包括 doc/server/arch.txt
#.gitignore 还有个有意思的小功能， 一个空的 .gitignore 文件 可以当作是一个 placeholder 。当你需要为项目创建一个空的 log 目录时， 这就变的很有用。 你可以创建一个 log 目录 在里面放置一个空的 .gitignore 文件。这样当你 clone 这个 repo 的时候 git 会自动的创建好一个空的 log 目录了。



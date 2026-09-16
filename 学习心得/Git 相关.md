.gitignore 不会清理已经提交到仓库的文件，仅对之后新增文件生效。 `git rm --cached` 可以取消 Git 对已有文件的跟踪，本地文件不会删除；
```bash
git rm --cached -r .obsidian 
git rm --cached -r .copilot
git add .
git commit -m "彻底移除缓存目录，禁止obsidian自动提交忽略文件夹"
git push origin main
```

删掉中间一堆 commit，但保留最终所有文件的内容不变
```bash
# 重置到指定commit
git reset --soft commit哈希

# 把当前全部文件打包成一条新提交
git add .
git commit -m "整理笔记"

# 强制推送到远程
git push --force origin main
```



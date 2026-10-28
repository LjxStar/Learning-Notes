# -*- coding: utf-8 -*-
path = r'D:/笔记仓库/JavaWeb/苍穹外卖学习笔记.md'
with open(path, 'r', encoding='utf-8') as f:
    content = f.read()

start = content.find('# 五、Redis')
end = content.find('# 六、其他知识点')
if start == -1 or end == -1:
    raise SystemExit('章节定位失败')

with open(r'D:/笔记仓库/JavaWeb/redis_new.md', 'r', encoding='utf-8') as f:
    new_redis = f.read()

new_content = content[:start] + new_redis.rstrip() + '\n\n' + content[end:]
with open(path, 'w', encoding='utf-8') as f:
    f.write(new_content)
print('done')

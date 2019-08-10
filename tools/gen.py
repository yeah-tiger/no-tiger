import re
rex = re.compile('KW_(.*),')
with open("test.txt", "r") as f:
    fp = open("new.txt", "w")
    for line in f:
        keyword = rex.search(line).group(1)
        fp.write('res["'+ keyword.lower() + '"] = TokenType::KW_' + keyword + ';\n')
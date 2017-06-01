for line in open('temps.txt', 'r').readlines():
    if 'MODEL' in line: continue
    contents = line.split(" ")
    print "%s %.04f"%(contents[0], 1000.0*(float(contents[2])-float(contents[4])))

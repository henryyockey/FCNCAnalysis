import sys

file1 = sys.argv[1]
file2 = sys.argv[2]

a = []
b = []


with open(file1) as f:
   for line in f:
       a.append(line)
       if 'str' in line:
          break
with open(file2) as f:
   for line in f:
       b.append(line)
       if 'str' in line:
          break
c = set(a) - set(b)
print c

print "       ******      "
d = set(b) - set(a)
print d

f = open("falseNegative.tex", "w")
for event in c:
   f.write(event)

f.close()

f = open("falsePositive.tex", "w")
for event in d:
   f.write(event)

f.close



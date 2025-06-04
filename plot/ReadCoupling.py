a_file = open("Coup.tex", "r")


list_of_lists = []

for line in a_file:
  line_list = line.split()
  list_of_lists.append(line_list)

for A in range(len(list_of_lists[0])):
    print str(A) + ', '+str(len(list_of_lists[0])) + ', '+str(len(list_of_lists[1]))
    print list_of_lists[0][A] + '=' + list_of_lists[1][A]

a_file.close()



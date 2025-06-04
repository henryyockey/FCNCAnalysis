import os

wcs = ['k_cte', 'k_ctl', 'k_cQe', 'k_cQlM', 'k_ctlT', 'k_ctlS']

#for wc in wcs:
#   os.system('./combine_scan.sh {}'.format(wc))

for i in range(len(wcs)):
   for j in range(i+1, len(wcs)):
      os.system("./combine_2d_scan.sh {} {}".format(wcs[i], wcs[j]))

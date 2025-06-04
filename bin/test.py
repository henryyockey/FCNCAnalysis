import Files_ULall_nano
SAMPLES = {}
SAMPLES.update(Files_ULall_nano.UL17)
for key, value in SAMPLES.items():

    if ("DY" not in key and "data" not in value[1]): continue
    print key


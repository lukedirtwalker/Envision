#!/usr/bin/python3

import subprocess, sys

cmd = ['clang-tidy'] + sys.argv[1:]
process = subprocess.Popen(cmd, stdout=subprocess.PIPE, bufsize = 1, universal_newlines = True)

while True:
  line = process.stdout.readline()
  if line != '':
    #the real code does filtering here
    print(line.rstrip(), file=sys.stderr)
  else:
    break
from os import system
from time import time

__author__ = 'gexogen'


def run(cmd):
    for n in [1, 4, 8, 16, 20]:
        for l in xrange(1, 25):
            start = time()
            system(cmd.format(l, n))
            print n, l, "time = ", time() - start

run("./cpu_latency -l {} -n {} >> output/out_2")
run("./cpu_latency -l {} -n {} -r >> output/out_2")
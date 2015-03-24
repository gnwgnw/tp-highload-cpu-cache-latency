from os import system

__author__ = 'gexogen'


def run(cmd):
    for n in [1, 4, 8, 16]:
        for l in xrange(1, 25):
            system(cmd.format(l, n))
            print n, l

run("./cpu_latency -l {} -n {} >> out_1")
run("./cpu_latency -l {} -n {} -r >> out_1")
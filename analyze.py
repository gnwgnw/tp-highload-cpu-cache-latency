from os import system
from time import time

__author__ = 'gexogen'


def run(cmd):
    for n in [16]:
        for l in xrange(1, (32 * 2**10) / n, 2**11 / n):
            start = time()
            system(cmd.format(l, n))
            print n, l, "time =", time() - start

        for l in xrange((32 * 2**10) / n, (256 * 2**10) / n, 2**14 / n):
            start = time()
            system(cmd.format(l, n))
            print n, l, "time =", time() - start

        for l in xrange((256 * 2**10) / n, (16384 * 2**10) / n, 2**20 / n):
            start = time()
            system(cmd.format(l, n))
            print n, l, "time =", time() - start

#run("./cpu_latency -l {} -n {} >> out_conseq")
run("./cpu_latency -l {} -n {} -r >> out_rand")
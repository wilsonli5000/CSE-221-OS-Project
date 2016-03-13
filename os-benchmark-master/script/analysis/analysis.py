#!/usr/bin/env python
from os import path
from collections import defaultdict
import math

root = path.dirname(path.dirname(path.dirname(__file__)))
result_dir = path.join(root, 'results')

def get_file_name(test):
    test = '%s_result' % test
    return path.join(result_dir, test)

def mean(l):
    return float(sum(l))/len(l) if len(l) > 0 else float('nan')

def std_dev(l):
    m = mean(l)
    return math.sqrt(sum((x - m) ** 2 for x in l) / len(l))

def run_timing_overhead_ana():
    test_name = 'timing_overhead'
    file_name = get_file_name(test_name)
    datas = []
    with open(file_name) as f:
        for l in f:
            datas.append(int(l))
    datas = [i for i in datas[:10000]]
    print "%s mean: %f" % (test_name, mean(datas))
    print "%s std dev: %f" % (test_name, std_dev(datas))

def run_loop_overhead_ana():
    test_name = 'loop_overhead'
    file_name = get_file_name(test_name)
    datas = []
    with open(file_name) as f:
        for l in f:
            datas.append(float(l.split(' ')[0]))
    datas = [i for i in datas[:10000]]
    print "%s mean: %f" % (test_name, mean(datas))
    print "%s std dev: %f" % (test_name, std_dev(datas))

def run_proc_call_overhead_ana():
    test_name = 'proc_call_overhead'
    file_name = get_file_name(test_name)
    datas = []
    with open(file_name) as f:
        for l in f:
            if l.startswith('-'):
                datas.append([])
                continue
            datas[-1].append(int(l.split(' ')[0]) * 1.0 / 10)
    print "%s result:" % test_name
    for i, data in enumerate(datas):
        m = mean(data)
        std = std_dev(data)
        print "%f\t%f" % (m, std)
        #print "%s %d mean: %f" % (test_name, i, mean(data))
        #print "%s %d std dev: %f" % (test_name, i, std_dev(data))

def run_process_context_switch_ana():
    test_name = 'process_context_switch'
    file_name = get_file_name(test_name)
    datas = []
    with open(file_name) as f:
        for l in f:
            try:
                datas.append(int(l.split(' ')[1]))
            except:
                pass
    datas = [i for i in datas[:100]]
    print "%s mean: %f" % (test_name, mean(datas))
    print "%s std dev: %f" % (test_name, std_dev(datas))

def run_thread_context_switch_ana():
    test_name = 'thread_context_switch'
    file_name = get_file_name(test_name)
    datas = []
    with open(file_name) as f:
        for l in f:
            datas.append(int(l.split(' ')[1]))
    datas = [i for i in datas[:100]]
    print "%s mean: %f" % (test_name, mean(datas))
    print "%s std dev: %f" % (test_name, std_dev(datas))

def run_mem_acc_ana():
    test_name = 'mem_acc'
    filename = get_file_name(test_name)
    datas = defaultdict(lambda: defaultdict(list))
    with open(filename) as f:
        for l in f:
            ll = l.split(' ')
            step = int(ll[7])
            offset = int(ll[1])
            cycle = float(ll[3])
            datas[step][offset].append(cycle)

    results = {}
    offsets = set()
    for step, v in sorted(datas.items()):
        result = []
        for offset, cycles in sorted(v.items()):
            offsets.add(offset)
            m = mean(cycles)
            result.append(m)
        results[step] = (result)
    print "mem access time result"
    fl = "step/offset\t%s" % "\t".join(str(i) for i in sorted(offsets))
    print fl
    for step, means in sorted(results.items()):
        line = "\t".join(str(i) for i in means)
        line = "%s\t%s" % (str(step), line)
        print line

if __name__ == '__main__':
    run_timing_overhead_ana()
    run_loop_overhead_ana()
    run_proc_call_overhead_ana()
    run_process_context_switch_ana()
    run_thread_context_switch_ana()
    run_mem_acc_ana()

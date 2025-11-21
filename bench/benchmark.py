#!/usr/bin/env python3
"""Simple benchmark harness for the trapezoid implementations.

Usage:
  python bench/benchmark.py --n 1000000 --repeats 3

This script looks for binaries in `bin/` named `serial`, `openmp`, and `mpi`.
It runs each (when present), records execution time and result, and writes
`bench/results.csv` with the stats.
"""
import csv
import os
import subprocess
import sys
import time
from statistics import mean, median

ROOT = os.path.dirname(os.path.dirname(__file__))
BIN_DIR = os.path.join(ROOT, 'bin')
OUT_CSV = os.path.join(ROOT, 'bench', 'results.csv')

def run(cmd, repeats=3):
    times = []
    last_result = None
    for _ in range(repeats):
        t0 = time.perf_counter()
        try:
            proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        except subprocess.CalledProcessError as e:
            print('Command failed:', cmd)
            print(e.stderr.decode())
            return None, None
        t1 = time.perf_counter()
        times.append(t1-t0)
        last_result = proc.stdout.decode().strip()
    return {'min': min(times), 'median': median(times), 'mean': mean(times)}, last_result

def find_bin(name):
    path = os.path.join(BIN_DIR, name)
    if os.name == 'nt':
        path = path + '.exe'
    return path if os.path.exists(path) else None

def parse_result_line(output):
    # try to extract the numeric result from program output
    for line in output.splitlines()[::-1]:
        parts = line.split('result=')
        if len(parts) > 1:
            try:
                val = float(parts[1].split()[0])
                return val
            except Exception:
                continue
    return None

def main():
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument('--a', type=float, default=0.0)
    p.add_argument('--b', type=float, default=1.0)
    p.add_argument('--n', type=int, default=1000000)
    p.add_argument('--func', type=int, default=0)
    p.add_argument('--repeats', type=int, default=3)
    args = p.parse_args()

    bins = {
        'serial': find_bin('serial'),
        'openmp': find_bin('openmp'),
        'mpi': find_bin('mpi'),
    }

    os.makedirs(os.path.dirname(OUT_CSV), exist_ok=True)
    with open(OUT_CSV, 'w', newline='') as csvf:
        writer = csv.DictWriter(csvf, fieldnames=['program','workers','n','time_min','time_median','time_mean','result'])
        writer.writeheader()

        # serial
        if bins['serial']:
            cmd = [bins['serial'], str(args.a), str(args.b), str(args.n), str(args.func)]
            stats, out = run(cmd, repeats=args.repeats)
            if stats:
                result = parse_result_line(out)
                writer.writerow({'program':'serial','workers':1,'n':args.n,'time_min':stats['min'],'time_median':stats['median'],'time_mean':stats['mean'],'result':result})

        # openmp: try a few thread counts
        if bins['openmp']:
            for th in [1,2,4,8]:
                cmd = [bins['openmp'], str(args.a), str(args.b), str(args.n), str(args.func), str(th)]
                stats, out = run(cmd, repeats=args.repeats)
                if stats:
                    result = parse_result_line(out)
                    writer.writerow({'program':'openmp','workers':th,'n':args.n,'time_min':stats['min'],'time_median':stats['median'],'time_mean':stats['mean'],'result':result})

        # mpi: try a few ranks
        if bins['mpi']:
            for r in [2,4]:
                cmd = ['mpirun','-np',str(r),bins['mpi'],str(args.a),str(args.b),str(args.n),str(args.func)]
                stats, out = run(cmd, repeats=args.repeats)
                if stats:
                    result = parse_result_line(out)
                    writer.writerow({'program':'mpi','workers':r,'n':args.n,'time_min':stats['min'],'time_median':stats['median'],'time_mean':stats['mean'],'result':result})

    print('Benchmark complete — results written to', OUT_CSV)

if __name__ == '__main__':
    main()

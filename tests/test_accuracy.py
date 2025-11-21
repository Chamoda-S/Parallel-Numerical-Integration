#!/usr/bin/env python3
"""Simple accuracy tests that call the serial binary and compare
the numerical result to known analytic integrals.

Run with: `python -m pytest tests/test_accuracy.py` or `python tests/test_accuracy.py`
"""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(__file__))
BIN = os.path.join(ROOT, 'bin', 'serial')
if os.name == 'nt':
    BIN = BIN + '.exe'

def run_serial(a,b,n,func):
    if not os.path.exists(BIN):
        print('serial binary not found at', BIN)
        return None
    proc = subprocess.run([BIN,str(a),str(b),str(n),str(func)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out = proc.stdout.decode()
    for line in out.splitlines():
        if 'result=' in line:
            try:
                val = float(line.split('result=')[1].split()[0])
                return val
            except Exception:
                pass
    return None

def test_sin_pi():
    # integral sin(x) from 0..pi == 2.0
    val = run_serial(0,3.141592653589793,1000000,0)
    assert val is not None
    assert abs(val - 2.0) < 1e-6

def test_x2_unit():
    # integral x^2 from 0..1 == 1/3
    val = run_serial(0,1,200000,3)
    assert val is not None
    assert abs(val - (1.0/3.0)) < 1e-6

if __name__ == '__main__':
    # allow running directly for convenience
    if not os.path.exists(BIN):
        print('Serial binary not found — build first (make serial)')
        sys.exit(1)
    print('Running accuracy tests...')
    test_sin_pi()
    test_x2_unit()
    print('All tests passed')

#!/bin/bash
ulimit -c unlimited
cd "$(dirname "$0")/Horizon/py-files" && exec python3 DraughtMain.py

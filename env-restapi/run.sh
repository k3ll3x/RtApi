#!/bin/bash
#python3 -m uvicorn main:app --reload
FLASK_APP="main.py"
python3 -m flask run --host=0.0.0.0 --port=1234

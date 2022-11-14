from flask import Flask
from flask import request
from flask import jsonify
import yaml
import os

app = Flask(__name__)

@app.route("/startcomposeraw", methods=['POST'])
def startcompose():
    txt = request.form['raw']
    # return f"<p>{txt}</p>"
    obj = yaml.load(txt,Loader=yaml.Loader)
    compose = os.path.join(".", "user-compose.yml")         
    f = open(compose, "w")
    f.write(txt)
    f.close()
    out = {
        'compose': obj,
        'result': os.popen("docker-compose -f user-compose.yml up -d").read()
    }
    return jsonify(out)
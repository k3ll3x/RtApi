from flask import Flask, render_template, request, jsonify
import yaml
import json
import os

app = Flask(__name__)

# Define the root route
@app.route('/')
def index():
    return render_template("litegraph/index.html")

@app.route("/startcomposeraw", methods=['POST'])
def startcompose():
    txt = request.form['raw']
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

@app.route("/saveraw", methods=['POST'])
def saveraw():
    txt = request.form['raw']
    filename = request.form['savename']
    obj = json.load(txt)
    fpath = os.path.join("./static", filename)
    f = open(fpath, "w")
    f.write(txt)
    f.close()
    out = {
        'fpath': fpath
    }
    return jsonify(out)

lambdas = {}

@app.route("/sink", methods=['GET','POST'])
def sink():
    return jsonify(request)

@app.route("/lambda", methods=['POST'])
def savelambda():
    _id = request.form['id']
    lmd = request.form['lambda']
    lambdas[_id] = eval(lmd)
    out = {
        'LMDs': lambdas,
        'out': lambdas[_id]
    }
    return jsonify(out)

if __name__ == "__main__":
    app.run(debug=True)
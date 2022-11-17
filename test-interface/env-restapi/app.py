from flask import Flask, render_template, request, jsonify
import yaml
import json
import os

app = Flask(__name__)

# Define the root route
@app.route('/')
def index():
    return render_template("index.html")

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

if __name__ == "__main__":
    app.run(debug=True)
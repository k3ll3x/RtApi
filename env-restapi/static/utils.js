var Environment = {};
// save Environment
function save(){
    download("env.json", JSON.stringify(Environment, null, 4));
}

function load(json){
    Environment = json;
}

function download(filename, text) {
    var pom = document.createElement('a');
    pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    pom.setAttribute('download', filename);

    if (document.createEvent) {
        var event = document.createEvent('MouseEvents');
        event.initEvent('click', true, true);
        pom.dispatchEvent(event);
    }
    else {
        pom.click();
    }
}

// Create nodes
function createNode(node){
    if(Environment.usernodes === undefined){
        Environment.usernodes = []
    }
    func = eval(node.lambda)
    Environment.usernodes.push({tree: node, lambda: func});
    LiteGraph.wrapFunctionAsNode(node.type, func, node.inputs, node.outputs);
}

function load_nodes_from_env(){
    if(Environment.usernodes === undefined){
        Environment.usernodes = []
    }
    for(let node in Environment.usernodes){
        LiteGraph.wrapFunctionAsNode(node.tree.type, func, node.tree.inputs, node.tree.outputs);
    }
}

// Editor save js
function editor_save(){
    let json;
    editor.save().then( savedData => {
        json = JSON.stringify(savedData, null, 4);
    })
    return json;
}

function fitToContainer(canvas){
    console.log("update size canvas");
    // Make it visually fill the positioned parent
    canvas.style.width ='100%';
    canvas.style.height='100%';
    // ...then set the internal size to match
    canvas.width  = canvas.offsetWidth;
    canvas.height = canvas.offsetHeight;
}

LiteGraph.LGraph.prototype.save = function () {
    var data = JSON.stringify(this.serialize());
    var file = new Blob([data]);
    var url = URL.createObjectURL(file);
    var element = document.createElement("a");
    element.setAttribute('href', url);
    element.setAttribute('download', "untitled.tcgraph");
    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
    setTimeout(function () { URL.revokeObjectURL(url); }, 1000 * 60); 
}

LiteGraph.LGraph.prototype.load = function () {

    var this_graph = this;

    if (typeof FileReader === 'undefined') {
        console.log('File loading not supported by your browser');
        return;
    }

    var inputElement = document.createElement('input');

    inputElement.type = 'file';
    inputElement.accept = '.tcgraph';
    inputElement.multiple = false;

    inputElement.addEventListener('change', function (data) {

        if (inputElement.files) {

            var file = inputElement.files[0];
            var reader = new FileReader();

            reader.addEventListener('loadend', function (load_data) {

                if (reader.result)
                    this_graph.configure(JSON.parse(reader.result));

            });
            reader.addEventListener('error', function (load_data) {
                console.log('File load error');
            });

            reader.readAsText(file);

        }
    });

    inputElement.click();
    return;

}


var Environment = {};
// save Environment
function save(){
    download("env.json", JSON.stringify(Environment, null, 4));
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
    // if(!Environment.usernodes){
    //     Environment.usernodes = []
    // }
    // Environment.usernodes.push(node)
    func = eval(node.lambda)
    LiteGraph.wrapFunctionAsNode(node.type, func, node.inputs, node.outputs);
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
    
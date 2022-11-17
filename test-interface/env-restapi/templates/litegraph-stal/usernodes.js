// Docker compose

//node constructor class
function Service(){
    this.addInput("Generate","trigger");
    this.widget = this.addWidget("text", "Name", "", "Name");
    this.widget = this.addWidget("text", "Env", "", "Env");
    this.addProperty("wtf", "build");
    this.addOutput("port","number");
    this.addOutput("port","number");
}

//name to show
Service.title = "Docker Service";

//function to call when the node is executed
Service.prototype.onExecute = function()
{
//   var A = this.getInputData(0);
//   if( A === undefined )
//     A = 0;
//   var B = this.getInputData(1);
//   if( B === undefined )
//     B = 0;
//   this.setOutputData( 0, A + B );
}

//register in the system
LiteGraph.registerNodeType("docker/service", Service );
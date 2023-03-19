use std::sync::{Arc, Mutex};

use rocket::{get, post, routes, Rocket};
use rocket_contrib::json::Json;

use rlua::{Lua, Result};

struct GisState {
    // Implement the GIS state here
}

#[get("/")]
fn index() -> &'static str {
    "Hello, world!"
}

#[post("/run_script", format = "json", data = "<script>")]
fn run_script(script: Json<String>, gis_state: State<Arc<Mutex<GisState>>>) -> Result<Json<String>> {
    let lua = Lua::new();

    let result = lua.context(|lua_ctx| {
        // Execute the script in the Lua runtime
        let globals = lua_ctx.globals();
        globals.set("gis_state", gis_state)?;

        let script_result: String = lua_ctx.load(script.as_str()).eval()?;

        Ok(Json(script_result))
    });

    result.map_err(|err| {
        println!("Error executing script: {}", err);
        Json(format!("Error: {}", err))
    })
}

fn rocket() -> Rocket {
    let gis_state = Arc::new(Mutex::new(GisState { /* Initialize the GIS state */ }));

    rocket::ignite()
        .mount("/", routes![index, run_script])
        .manage(gis_state)
}

fn main() {
    rocket().launch();
}
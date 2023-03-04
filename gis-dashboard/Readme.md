# GIS dashboard

ChatGPT Rust

`
To create a complete graphics web UI GIS dashboard with features that can be programmed in any scripting runtime language, we will need to use multiple technologies and frameworks.

Here's an example implementation that uses the following components:

    Actix-Web for the web API
    Yew for the web UI
    WebAssembly as the target language for the web UI
    Python as the scripting runtime language
`


This code creates a Yew application with a GIS dashboard that allows the user to add points and polygons to the map and run Python code to add features. The Python code is executed in a separate process using the `std::process::Command` API, and the resulting GeoJSON is parsed and added to the GIS.

To use this application, you will need to replace `<your access token here>` in the `iframe` with your Mapbox access token. You will also need to have Python installed on your system and available on your `PATH`.

The application can be run using `cargo web start` and accessed in a web browser at `http://localhost:8000`.

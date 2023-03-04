use std::sync::{Arc, Mutex};
use std::process::{Command, Stdio};
use std::io::Write;
use actix_web::{get, post, web, App, HttpResponse, HttpServer, Responder};
use geo::{Coordinate, Geometry, Point, Polygon};
use yew::{html, Component, ComponentLink, Html, ShouldRender};
use wasm_bindgen::prelude::*;
use dotenv::dotenv;
use serde_json::{self, Value};

struct GIS {
    features: Vec<Geometry<f64>>,
}

impl GIS {
    fn new() -> Self {
        Self {
            features: Vec::new(),
        }
    }

    fn add_point(&mut self, x: f64, y: f64) {
        let point = Point::new(x, y);
        self.features.push(Geometry::Point(point));
    }

    fn add_polygon(&mut self, points: &[Coordinate<f64>]) {
        let polygon = Polygon::new(points.to_vec(), vec![]);
        self.features.push(Geometry::Polygon(polygon));
    }

    fn to_geojson(&self) -> String {
        let mut geojson = String::new();
        geojson.push_str("{\n");
        geojson.push_str("  \"type\": \"FeatureCollection\",\n");
        geojson.push_str("  \"features\": [\n");

        for feature in &self.features {
            let geometry_type = match feature {
                Geometry::Point(_) => "Point",
                Geometry::Line(_) => "LineString",
                Geometry::Polygon(_) => "Polygon",
                Geometry::MultiPoint(_) => "MultiPoint",
                Geometry::MultiLine(_) => "MultiLineString",
                Geometry::MultiPolygon(_) => "MultiPolygon",
                Geometry::GeometryCollection(_) => "GeometryCollection",
            };

            let geometry = format!("{:?}", feature);
            let feature = format!("    {{ \"type\": \"Feature\", \"geometry\": {}, \"properties\": {{}} }},\n", geometry);

            geojson.push_str(&feature);
        }

        if geojson.ends_with(",\n") {
            geojson.pop();
            geojson.pop();
            geojson.push('\n');
        }

        geojson.push_str("  ]\n");
        geojson.push_str("}\n");

        geojson
    }
}

struct Model {
    link: ComponentLink<Self>,
    gis: Arc<Mutex<GIS>>,
    python_code: String,
}

enum Msg {
    AddPoint,
    AddPolygon,
    PythonCodeChanged(String),
    RunPythonCode,
}

impl Component for Model {
    type Message = Msg;
    type Properties = ();

    fn create(_: Self::Properties, link: ComponentLink<Self>) -> Self {
        Self {
            link,
            gis: Arc::new(Mutex::new(GIS::new())),
            python_code: String::new(),
        }
    }

    fn update(&mut self, msg: Self::Message) -> ShouldRender {
        match msg {
            Msg::AddPoint => {
                self.gis.lock().unwrap().add_point(0.0, 0.0);
            }
            Msg::AddPolygon => {
                self.gis.lock().unwrap().add_polygon(&[
                    Coordinate { x: 0.0, y: 0.0 },
                    Coordinate { x: 1.0, y: 0.0 },
                    Coordinate { x: 1.0, y: 1.0 },
                    Coordinate { x: 0.0, y: 1.0 },
                    Coordinate { x: 0.0, y: 0.0 },
                ]);
            }
            Msg::PythonCodeChanged(code) => {
                self.python_code = code;
            }
            Msg::RunPythonCode => {
                let gis = self.gis.clone();
    
                let mut child = Command::new("python")
                    .stdin(Stdio::piped())
                    .stdout(Stdio::piped())
                    .spawn()
                    .expect("failed to execute child process");
    
                {
                    let stdin = child.stdin.as_mut().expect("failed to get stdin");
                    stdin.write_all(self.python_code.as_bytes()).expect("failed to write to stdin");
                }
    
                let output = child.wait_with_output().expect("failed to wait on child");
    
                if output.status.success() {
                    let stdout = String::from_utf8(output.stdout).expect("stdout is not valid UTF-8");
                    println!("Python script executed successfully");
                    println!("{}", stdout);
    
                    // Parse the output and update the GIS
                    let new_features = parse_geojson(&stdout);
                    gis.lock().unwrap().features.extend(new_features);
                } else {
                    let stderr = String::from_utf8(output.stderr).expect("stderr is not valid UTF-8");
                    eprintln!("Python script failed to execute");
                    eprintln!("{}", stderr);
                }
            }
        }
    
        true
    }
    
    fn view(&self) -> Html {
        let mapbox_api_token: String = std::env::var("MAPBOX_API_TOKEN").expect("MAPBOX_API_TOKEN must be set.");

        let mut ta: String = String::from("<html><head><script src='https://api.mapbox.com/mapbox-gl-js/v2.3.1/mapbox-gl.js'></script><link href='https://api.mapbox.com/mapbox-gl-js/v2.3.1/mapbox-gl.css' rel='stylesheet' /></head><body><div id='map' style='width: 100%; height: 500px;'></div><script>mapboxgl.accessToken = '";
        let mut tb: String = String::from("';var map = new mapboxgl.Map({container: 'map',style: 'mapbox://styles/mapbox/streets-v11',zoom: 2,center: [0, 0]});var geojson = JSON.parse('{}');map.on('load', function () {{map.addSource('gis', {{type: 'geojson',data: geojson}});map.addLayer({id: 'gis',type: 'circle',source: 'gis',paint: {{'circle-color': '#FF0000', 'circle-radius': 5}}});}});</script></body></html>");
        let template = format!("{ta}{mapbox_api_token}{tb}");

        let gis = self.gis.lock().unwrap();
        let geojson = gis.to_geojson();
    
        html! {
            <div>
                <div>
                    <button onclick=self.link.callback(|_| Msg::AddPoint)>{"Add Point"}</button>
                    <button onclick=self.link.callback(|_| Msg::AddPolygon)>{"Add Polygon"}</button>
                </div>
                <div>
                    <textarea value=&self.python_code oninput=self.link.callback(|e: yew::InputData| Msg::PythonCodeChanged(e.value))></textarea>
                    <button onclick=self.link.callback(|_| Msg::RunPythonCode)>{"Run Python Code"}</button>
                </div>
                <div>
                    <iframe srcdoc=&format!(template, geojson)>
                </div>
            </div>
        }
    }
}

fn parse_geojson(path: &str) -> Vec<Geometry> {
    // Read the file contents into a string
    let mut file = File::open(path).expect("failed to open file");
    let mut contents = String::new();
    file.read_to_string(&mut contents).expect("failed to read file");

    // Parse the GeoJSON using the serde_json crate
    let json: Value = serde_json::from_str(&contents).expect("failed to parse JSON");
    match GeoJson::from_value(json) {
        Ok(geojson) => match geojson {
            GeoJson::FeatureCollection(collection) => collection
                .features
                .into_iter()
                .filter_map(|feature| feature.geometry)
                .collect(),
            GeoJson::Feature(feature) => match feature.geometry {
                Some(geometry) => vec![geometry],
                None => panic!("feature has no geometry"),
            },
            GeoJson::Geometry(geometry) => vec![geometry],
        },
        Err(err) => panic!("failed to parse GeoJSON: {}", err),
    }
}

fn main() {
    dotenv().ok();
    yew::start_app::<Model>();
}
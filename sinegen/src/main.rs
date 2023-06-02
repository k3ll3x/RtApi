#[macro_use] extern crate rocket;
use std::net::{UdpSocket, SocketAddrV4, Ipv4Addr};
use std::{thread, time};
use serde::{Serialize, Deserialize};
use clap::Parser;
use rand;
// use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::fs;
use rocket::{serde::json::Json, State};
use toml;
use dashmap::DashMap;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long, required = false)]
    multicast_addr: String,
    #[arg(short, long)]
    configfile: String
}

#[derive(Debug, Serialize, Deserialize)]
struct Frame {
    points: Vec<f32>,
}

#[derive(Debug, Serialize, Deserialize)]
struct WaveConf {
    frequency: f32,
    offset: f32,
    phase_shift: f32,
    lst: Vec<f32>
}

#[derive(Debug, Serialize, Deserialize)]
struct Line{
    appid: u16,
    channels: Vec<WaveConf>
}

#[derive(Debug, Default, Serialize, Deserialize)]
struct Config{
    lines: Vec<Line>
}

// type TWaveMap = HashMap<u16, Vec<WaveConf>>;
type TWaveMap = DashMap<u16, Vec<WaveConf>>;
type TSession = Arc<Mutex<Session>>;

#[derive(Debug)]
struct Session{
    wavemap: TWaveMap,
    // socket: UdpSocket,
    // sockets: HashMap<u16, UdpSocket>,
    sockets: DashMap<u16, UdpSocket>,
    multicast_addr: String
}

#[get("/")]
async fn index() -> Json<Frame> {
    let frame = Frame {
        points: randf32_vec(3)
    };
    Json(frame)
}

#[get("/config")]
async fn getwavemap(_session: &State<TSession>) -> String {
    let session = _session.clone().lock().unwrap();
    let wavemap = &session.wavemap;
    format!("{:?}", wavemap)
}

#[post("/config", format = "json", data = "<incfg>")]
async fn api_config(session: &State<TSession>, incfg: Json<Config>) -> Json<Frame> {
    // load_config(session, incfg.into_inner());
    // create_threads(session.lock());
    Json(Frame { points: randf32_vec(3) })
}

fn load_config(_session: Arc<Mutex<Session>>, cfg: Config) {
    let session_clone = _session.clone();
    let mut session = session_clone.lock().unwrap();
    session.wavemap.clear();
    for line in &cfg.lines{
        let mut channels = Vec::new();
        for channel in &line.channels{
            channels.push(
                WaveConf {
                    lst: channel.lst.iter().map(|a| a * (2.0 as f32).sqrt()).collect(),
                    frequency: channel.frequency,
                    offset: channel.offset,
                    phase_shift: channel.phase_shift,
                }
            );
        }
        
        let appid = line.appid.clone();
        session.wavemap.insert(appid, channels);

        let socket = UdpSocket::bind((Ipv4Addr::UNSPECIFIED, appid)).unwrap();
        let multicast_addr: SocketAddrV4 = format!("{}:{}", session.multicast_addr, appid).as_str().parse().unwrap();
        socket.join_multicast_v4(&multicast_addr.ip(), &Ipv4Addr::UNSPECIFIED).unwrap();
        session.sockets.insert(appid, socket);
    }
}

fn get_smv(config: &Vec<WaveConf>) -> Vec<f32>{
    let mut smv = Vec::<f32>::new();
    for channel in config{
        smv.push(channel.lst[0] * (2.0 * 3.14159 * channel.frequency * rand::random::<f32>() + channel.phase_shift + channel.offset).sin());
    }
    smv
}

fn randf32_vec(n: i32) -> Vec<f32>{
    (1..n).map(|x| x as f32 * rand::random::<f32>()).collect()
}

fn create_threads(session: Arc<Mutex<Session>>){
    let session = session.clone();
    thread::spawn(move || {
        let session = session.lock().unwrap();
        // println!("{:?}", session.wavemap.iter().count());
        for e in session.wavemap.iter() {
            let appid = e.key().clone();
            let config = e.value().clone();
            let socket = session.sockets.get(&appid).unwrap().try_clone();
            let multicast_addr = session.multicast_addr.to_string();
            thread::spawn(move || {
                // let config = config.clone();
                println!("{:?}", config);
                loop {
                    let multicast_addr = multicast_addr.clone();
                    let frame = Frame {
                        // points: get_smv(config)
                        // points: randf32_vec(rand::random::<i32>())
                        points: randf32_vec(8)
                    };
                    let buffer = bincode::serialize(&frame).unwrap();
                    socket.as_ref().unwrap().send_to(&buffer, (multicast_addr, appid)).unwrap();
                    thread::sleep(std::time::Duration::from_secs(1));
                }
            });
        }
        thread::sleep(std::time::Duration::from_secs(1));
    }).join().unwrap();
}

#[tokio::main]
async fn main() {
    let args = Args::parse();
    println!("{args:?}");

    let session = Session {
        wavemap: TWaveMap::new(),
        // socket: UdpSocket::bind(format!("{}", args.bind_addr)).unwrap(),
        // sockets: HashMap::new(),
        sockets: DashMap::new(),
        multicast_addr: args.multicast_addr
        // start: bool
    };
    let shsession = Arc::new(Mutex::new(session));
    
    //load toml config
    let configfile = args.configfile.as_str();
    let contents = fs::read_to_string(configfile).unwrap();
    let mut config: Config = Config::default();
    match configfile {
        f if f.ends_with(".toml") => {
            config = toml::from_str(&contents.as_str()).unwrap();
        },
        f if f.ends_with(".json") => {
            config = serde_json::from_str(&contents.as_str()).unwrap();
        },
        _ => {
            println!("Unknown file type: {}", configfile);
        }
    }

    // println!("{:?}", config);

    load_config(shsession.clone(), config);
    create_threads(shsession.clone());
    
    rocket::build()
        .manage(shsession.clone())
        .mount("/", routes![index, api_config, getwavemap])
        .launch().await;
}
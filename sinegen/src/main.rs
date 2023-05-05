#[macro_use] extern crate rocket;
use std::net::{UdpSocket, SocketAddrV4, Ipv4Addr};
use std::{thread, time};
use serde::{Serialize, Deserialize};
use clap::Parser;
use rand;
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use rocket::{serde::json::Json, State};

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    // #[arg(short, long, required = true)]
    // protocol: String,
    #[arg(short, long, required = false)]
    bind_addr: String,
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

#[derive(Debug, Serialize, Deserialize)]
struct Config{
    lines: Vec<Line>
}

type TWaveMap = HashMap<u16, Vec<WaveConf>>;
type TSession = Arc<Mutex<Session>>;

#[derive(Debug)]
struct Session{
    wavemap: TWaveMap,
    socket: UdpSocket,
    multicast_addr: String
}

#[get("/")]
async fn index() -> Json<Frame> {
    let frame = Frame {
        points: randf32_vec(3)
    };
    Json(frame)
}

fn load_config(_session: &State<TSession>, cfg: Config) {
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
        
        let session_clone = _session.clone();

        let mut session = session_clone.lock().unwrap();
        session.wavemap.insert(appid, channels);
        let multicast_addr: SocketAddrV4 = format!("{}:{}", session.multicast_addr, appid).as_str().parse().unwrap();
        session.socket.join_multicast_v4(&multicast_addr.ip(), &Ipv4Addr::UNSPECIFIED).unwrap();
        drop(session);
    }
}

#[post("/config", format = "json", data = "<incfg>")]
async fn api_config(session: &State<TSession>, incfg: Json<Config>) -> Json<Frame> {
    load_config(session, incfg.into_inner());
    Json(Frame { points: randf32_vec(16) })
}

fn randf32_vec(n: i32) -> Vec<f32>{
    (1..n).map(|x| x as f32 * rand::random::<f32>()).collect()
}

#[tokio::main]
async fn main() {
    let args = Args::parse();
    println!("{args:?}");
    
    let session = Session {
        wavemap: TWaveMap::new(),
        socket: UdpSocket::bind(format!("{}", args.bind_addr)).unwrap(),
        multicast_addr: args.multicast_addr
    };
    let shsession = Arc::new(Mutex::new(session));
    
    let session_clone = shsession.clone();
    thread::spawn( move || {
        let s = session_clone.lock().unwrap();
        loop {
            // let mut session = session_clone.lock().unwrap();
            // get frame
            let frame = Frame {
                points: randf32_vec(rand::random::<i32>() % 16)
            };
            let buffer = bincode::serialize(&frame).unwrap();
            s.socket.send_to(&buffer, &s.multicast_addr).unwrap();
            thread::sleep(time::Duration::from_secs(1));
        }
    });

    rocket::build()
        .manage(shsession.clone())
        .mount("/", routes![index, api_config])
        .launch().await;
}
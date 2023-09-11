#[macro_use] extern crate rocket;
use std::net::{UdpSocket, SocketAddrV4, Ipv4Addr};
use std::{thread, time};
use serde::{Serialize, Deserialize};
use clap::Parser;
use rand;
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::fs;
use rocket::{serde::json::Json, State};
use toml;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long, required = false)]
    multicast_addr: String,
    #[arg(short, long)]
    configfile: String
}

#[derive(Debug, Serialize, Deserialize)]
struct Asdu {
    svID: [u8; 15],
    smpCnt: u16,
    confRev: u32,
    smpSynch: u8,
    seqData: Vec<f32>,
}

#[derive(Debug, Serialize, Deserialize)]
struct GFrame {
    appid: u16,
    length: u16,
    reserved: u32,
    idk: [u8; 3],
    savPdu: u16,
    noASDU: u8,
    idkid: u8,
    seqAsdu: Vec<Asdu>,
}

#[derive(Debug, Serialize, Deserialize)]
struct Frame {
    points: Vec<f32>,
}

#[derive(Clone, Debug, Default, Serialize, Deserialize)]
struct WaveConf {
    frequency: f32,
    offset: f32,
    phase_shift: f32,
    lst: Vec<f32>
}

#[derive(Debug, Default, Serialize, Deserialize)]
struct Line{
    appid: u16,
    channels: Vec<WaveConf>
}

#[derive(Debug, Default, Serialize, Deserialize)]
struct RawConfig{
    lines: Vec<Line>,
    sample_rate: u16,
    noAsdu: u8
}

#[derive(Debug)]
struct Config{
    noAsdu: u8,
    time: f32,
    sample_count: u16,
    waveconf: Vec<WaveConf>,
    sample_rate: u16,
    socket: UdpSocket
}

impl Config {
    fn next(&mut self){
        self.sample_count = match self.sample_count.checked_add(1) {
            Some(r) => r,
            None => 0
        };
        self.time = match self.time.checked_add(1.0 / self.sample_rate as f32){
            Some(r) => r,
            None => 0.0,
        };
    }
}

// type TWaveMap = HashMap<u16, Vec<WaveConf>>;
// type TWaveMap = DashMap<u16, Vec<WaveConf>>;
// type TConfig = DashMap<u16, Config>;
type TConfig = HashMap<u16, Config>;
type TSession = Arc<Mutex<Session>>;

#[derive(Debug)]
struct Session{
    // wavemap: TWaveMap,
    config: TConfig,
    // socket: UdpSocket,
    // sockets: HashMap<u16, UdpSocket>,
    // sockets: DashMap<u16, UdpSocket>,
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
async fn getconfig(_session: &State<TSession>) -> String {
    let session = _session.clone().lock().unwrap();
    format!("{:?}", &session.config)
}

// #[get("/createthreads")]
// async fn getconfig(_session: &State<TSession>) -> String {
//     let session = _session.clone().lock().unwrap();
//     create_threads(session);
//     format!("{:?}", &session.config)
// }

#[post("/config", format = "json", data = "<incfg>")]
async fn api_config(session: &State<TSession>, incfg: Json<RawConfig>) -> Json<Frame> {
    // load_config(session, incfg.into_inner());
    let session = session.clone();//.lock().unwrap();
    create_threads(session.lock().unwrap());
    Json(Frame { points: randf32_vec(3) })
}

fn load_config(_session: Arc<Mutex<Session>>, cfg: RawConfig) {
    let session_clone = _session.clone();
    let mut session = session_clone.lock().unwrap();
    session.config.clear();
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

        let socket = UdpSocket::bind((Ipv4Addr::UNSPECIFIED, appid)).unwrap();
        let multicast_addr: SocketAddrV4 = format!("{}:{}", session.multicast_addr, appid).as_str().parse().unwrap();
        socket.join_multicast_v4(&multicast_addr.ip(), &Ipv4Addr::UNSPECIFIED).unwrap();
        // session.sockets.insert(appid, socket);

        session.config.insert(appid,
            Config {
                noAsdu: cfg.noAsdu,
                time: 0.0,
                sample_count: 0,
                waveconf: channels,
                sample_rate: cfg.sample_rate,
                socket: socket
            }
        );
    }
}

fn get_smv(config: &Vec<WaveConf>, time: f32) -> Vec<f32>{
    let mut smv = Vec::<f32>::new();
    for channel in config{
        smv.push(channel.lst[0] * (2.0 * 3.14159 * channel.frequency * time + channel.phase_shift + channel.offset).sin());
    }
    smv
}

fn randf32_vec(n: i32) -> Vec<f32>{
    (1..n).map(|x| x as f32 * rand::random::<f32>()).collect()
}

fn create_threads(session: Arc<Mutex<Session>>){
    let session = session.clone();
    thread::spawn(move || {
        let mut appids: Vec<_> = {
            let session = session.lock().unwrap();
            session.config.keys().cloned().collect()
        };
        for appid in appids {
            let session = session.clone();
            let mut sample_rate: u16 = 0;
            thread::spawn(move || {
                loop {

                    let start = time::Instant::now();
                    let mut elapsed = 0;
                    let mut config: &mut Config;
                    let mut multicast_addr: String;
                    {
                        let mut session = session.lock().unwrap();
                        multicast_addr = session.multicast_addr.to_string();
                        config = session.config.get_mut(&appid).unwrap();
                        sample_rate = config.sample_rate;
                        
                        let mut asdu = Vec::new();
                        for a in 0..config.noAsdu {
                            asdu.push(
                                Asdu {
                                    svID: *b"cpcsmvgenerator",
                                    smpCnt: config.sample_count,
                                    confRev: 1,
                                    smpSynch: 2,
                                    seqData: get_smv(&config.waveconf, config.time)
                                }
                            );
    
                            config.next();
                        }
    
                        let frame = GFrame {
                            appid: appid.to_be(),
                            length: ((std::mem::size_of_val(&asdu) + 8) as u16).to_be(),
                            reserved: 0,
                            idk: [ 0x60, 0x81, 0xd4],
                            savPdu: 0x0180,
                            noASDU: config.noAsdu,
                            idkid: 0xa2,
                            seqAsdu: asdu
                        };
    
                        let buffer = bincode::serialize(&frame).unwrap();
                        config.socket.send_to(&buffer, (multicast_addr, appid)).unwrap();
                        elapsed = start.elapsed().as_nanos();
                    }
                    let dtime = (1000000 / sample_rate as u128) * 1000;
                    let nanos = match dtime.checked_sub(elapsed) {
                        Some(r) => r,
                        None => {
                            dtime
                        }
                    };
                    thread::sleep(std::time::Duration::from_nanos(nanos.try_into().unwrap()));
                }
            });
        }
    }).join().unwrap();
}

#[tokio::main]
async fn main() {
    let args = Args::parse();
    println!("{args:?}");

    let session = Session {
        rconfig: RawConfig::default(),
        config: TConfig::new(),
        multicast_addr: args.multicast_addr
        // start: bool
    };
    
    //load config
    let configfile = args.configfile.as_str();
    let contents = fs::read_to_string(configfile).unwrap();
    // let mut config: RawConfig = RawConfig::default();
    match configfile {
        f if f.ends_with(".toml") => {
            session.rconfig = toml::from_str(&contents.as_str()).unwrap();
        },
        f if f.ends_with(".json") => {
            session.rconfig = serde_json::from_str(&contents.as_str()).unwrap();
        },
        _ => {
            println!("Unknown file type: {}", configfile);
        }
    }
    
    let shsession = Arc::new(Mutex::new(session));
    // load_config(shsession.clone(), config);
    // create_threads(shsession.clone());
    
    rocket::build()
        .manage(shsession.clone())
        .mount("/", routes![index, api_config, getconfig])
        .launch().await;
}
use std::io::{Write};
use std::net::{TcpListener, UdpSocket, SocketAddrV4, Ipv4Addr};
use std::{thread, time};
use serde::{Serialize, Deserialize};
use clap::Parser;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long, required = true)]
    protocol: String,
    #[arg(short, long, required = false)]
    bind_addr: String,
    #[arg(short, long, required = false)]
    multicast_addr: String
}

#[derive(Debug, Serialize, Deserialize)]
struct MyStruct {
    value1: i32,
    value2: f64,
}

fn main() -> std::io::Result<()> {
    // Parse command line arguments
    let args = Args::parse();

    println!("{args:?}");

    // Create a struct
    let my_struct = MyStruct {
        value1: 42,
        value2: 3.14,
    };

    match args.protocol.as_str() {
        "tcp" => {
            // Create and listen on a TCP socket
            // let listener = TcpListener::bind(format!("{}:{}", args.bind_addr, args.bind_port))?;
            let listener = TcpListener::bind(args.bind_addr)?;

            // Accept incoming connections
            for stream in listener.incoming() {
                let mut stream = stream.unwrap();

                // Serialize the struct to a byte buffer
                let buffer = bincode::serialize(&my_struct).unwrap();

                // Send the buffer over the TCP stream
                stream.write_all(&buffer)?;
            }
        },
        "udp" => {
            // Create a UDP socket and bind to an address
            // let socket = UdpSocket::bind(format!("{}:{}", args.bind_addr, args.bind_port))?;
            let socket = UdpSocket::bind(args.bind_addr)?;

            let multicast_addr: SocketAddrV4 = args.multicast_addr.as_str().parse().unwrap();

            // Join the multicast group
            socket.join_multicast_v4(&multicast_addr.ip(), &Ipv4Addr::UNSPECIFIED)?;

            // Serialize the struct to a byte buffer
            let buffer = bincode::serialize(&my_struct).unwrap();

            // Send the buffer over the UDP socket to the multicast address
            for _ in 0..10 {
                socket.send_to(&buffer, &multicast_addr).unwrap();
                thread::sleep(time::Duration::from_millis(10));
            }
        },
        _ => {
            println!("Invalid protocol selected: {}", args.protocol);
            return Ok(());
        }
    }

    Ok(())
}
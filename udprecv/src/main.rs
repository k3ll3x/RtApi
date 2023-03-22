use std::net::{UdpSocket, SocketAddrV4, Ipv4Addr};
use std::str::from_utf8;
use serde::{Serialize, Deserialize};
// use std::io::{BufReader, Cursor};
// use byteorder::{LittleEndian, ReadBytesExt};
use clap::Parser;

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[arg(short, long)]
    bind_addr: String,
    #[arg(short, long)]
    multicast_addr: String,
}

#[derive(Debug, Serialize, Deserialize)]
struct Frame {
    points: Vec<f32>,
}

fn main() -> std::io::Result<()> {    
    let args = Args::parse();

    println!("{args:?}");

    let bind_address = args.bind_addr;

    let socket = UdpSocket::bind(bind_address)?;
    let multicast_addr: SocketAddrV4 = args.multicast_addr.parse().unwrap_or("224.0.0.0:3000".parse().unwrap());

    // socket.join_multicast_v4(&multicast_addr.ip(), &socket.local_addr()?.ip())?;
    socket.join_multicast_v4(&multicast_addr.ip(), &Ipv4Addr::UNSPECIFIED)?;

    let mut buf = [0u8; 65536];
    loop {
        let (len, addr) = socket.recv_from(&mut buf)?;
        // let message = from_utf8(&buf[..len]).unwrap();

        println!("{:?}\t{:?}", &buf[..len], addr);

        // let mut cursor = Cursor::new(&buf[..len]);
        // let field1 = cursor.read_i32::<LittleEndian>().unwrap();
        // let field2_len = cursor.read_u32::<LittleEndian>().unwrap() as usize;
        // let field2_bytes = cursor.get_ref()[cursor.position() as usize..][..field2_len].to_vec();
        // let field2 = String::from_utf8(field2_bytes).unwrap();

        // let frame = frame {
        //     points
        // };

        // println!("Received struct {:?} from {}", frame, addr);
    }
}

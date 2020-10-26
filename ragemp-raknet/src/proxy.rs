use std::net::SocketAddr;
use std::time::Duration;

use crate::raknet::bitstream::BitStream;
use crate::raknet::messages::*;
use crate::raknet::packet_priority::*;
use crate::raknet::rakpeer::*;
use crate::raknet::types::*;

use crate::handler::{Handler, Response};

pub struct Proxy {
    remote_addr: SocketAddr,
    client_addr: Option<SocketAddr>,

    server: RakPeerInterface,
    remote: RakPeerInterface,

    connected_to_remote: bool,
    unsended: Vec<BitStream<'static>>,

    key: PublicKey,
    password: Option<&'static [u8]>,

    pub handlers: Vec<Box<Handler>>,
}

impl Proxy {
    pub fn new(remote_addr: SocketAddr) -> Proxy {
        let server = RakPeerInterface::new();
        let remote = RakPeerInterface::new();

        let password = b"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x8e\xdf\x05\xee\x27\xf7\x1a\x7d\x1f\xaf\x0a\x00\x20\x87\x1f\x6a\x1c\x00\x00\x00";

        Proxy {
            remote_addr,
            client_addr: None,
            server,
            remote,
            connected_to_remote: false,
            unsended: vec![],
            password: None,
            key: PublicKey::new(PublicKeyMode::AcceptAnyPublicKey),
            // key: PublicKey::new(PublicKeyMode::InsecureConnection),
            handlers: Vec::new(),
        }
    }

    pub fn startup(&self) -> StartupResult {
        let addr = "0.0.0.0:22005".parse().unwrap();
        let mut socket = SocketDescriptor::new(addr);

        self.server.startup(8, &socket)?;
        self.server.set_maximum_incoming_connections(8);

        socket.set_port(0);
        self.remote.startup(8, &socket)?;
        self.remote.set_maximum_incoming_connections(8);

        Ok(())
    }

    pub fn run(mut self) {
        // self.remote
        //     .connect(self.remote_addr, self.password.clone(), &self.key);

        loop {
            self.process_from_proxy();
            self.process_from_remote();

            std::thread::sleep(Duration::from_millis(10));
        }
    }

    fn process_from_proxy(&mut self) -> Option<()> {
        let packet = self.server.receive()?;
        let mut bs = packet.as_bitstream();

        match bs.read::<u8>().map(|id| id as i32) {
            Some(ID_NEW_INCOMING_CONNECTION) => {
                println!("PROXY: new incoming connection!");
                self.client_addr = Some(packet.address());
                self.remote
                    .connect(self.remote_addr, self.password.clone(), &self.key);
            }

            Some(ID_CONNECTION_LOST) | Some(ID_DISCONNECTION_NOTIFICATION) => {
                self.remote.close_connection(self.remote_addr);
                self.connected_to_remote = false;
            }

            Some(packet_id) => {
                for handler in self.handlers.iter_mut() {
                    match handler
                        .handle_outcoming(packet_id, &mut bs)
                        .unwrap_or(Response::Forward)
                    {
                        Response::Break => return None,
                        Response::Forward => (),
                        Response::Change(msg) => bs = msg,
                    }
                }

                if !self.connected_to_remote {
                    self.unsended.push(bs.clone_static());
                    return None;
                }

                self.send_to_remote(&bs);
            }

            None => self.send_to_remote(&bs),
        }

        None
    }

    fn process_from_remote(&mut self) -> Option<()> {
        let packet = self.remote.receive()?;
        let mut bs = packet.as_bitstream();

        match bs.read::<u8>().map(|id| id as i32) {
            Some(ID_CONNECTION_REQUEST_ACCEPTED) => {
                self.connected_to_remote = true;
                println!("ID_CONNECTION_REQUEST_ACCEPTED");

                while let Some(bs) = self.unsended.pop() {
                    self.send_to_remote(&bs);
                }
            }

            Some(ID_OUR_SYSTEM_REQUIRES_SECURITY) => {
                println!("ID_OUR_SYSTEM_REQUIRES_SECURITY");
                self.remote.close_connection(self.remote_addr.clone());
                self.remote
                    .cancel_connection_attempt(self.remote_addr.into());
                self.key.set_mode(PublicKeyMode::InsecureConnection);
                self.remote
                    .connect(self.remote_addr, self.password.clone(), &self.key);
            }

            Some(ID_CONNECTION_ATTEMPT_FAILED) => {
                println!("ID_CONNECTION_ATTEMPT_FAILED");
                //self.remote.cancel_connection_attempt(self.remote_addr.into());
                //self.remote.connect(self.remote_addr, None, &self.key);
                //self.send_to_client(&bs);
            }

            Some(packet_id) => {
                if self.connected_to_remote == false {
                    self.connected_to_remote = true;
                    println!("someshit bu tconnected");

                    while let Some(bs) = self.unsended.pop() {
                        self.send_to_remote(&bs);
                    }
                }

                for handler in self.handlers.iter_mut() {
                    match handler
                        .handle_incoming(packet_id, &mut bs)
                        .unwrap_or(Response::Forward)
                    {
                        Response::Break => return None,
                        Response::Forward => (),
                        Response::Change(msg) => bs = msg,
                    }
                }
                self.send_to_client(&bs);
            }

            _ => {
                self.send_to_client(&bs);
            }
        }

        None
    }

    fn send_to_remote(&self, bs: &BitStream) {
        self.remote.send_bitstream(
            &bs,
            PacketPriority::High,
            PacketReliability::Reliable,
            0,
            self.remote_addr,
            false,
        );
    }

    fn send_to_client(&self, bs: &BitStream) {
        if let Some(client) = self.client_addr {
            self.server.send_bitstream(
                &bs,
                PacketPriority::High,
                PacketReliability::Reliable,
                0,
                client,
                false,
            );
        }
    }
}

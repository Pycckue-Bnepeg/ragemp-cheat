use std::hash::Hasher;
use std::io::{Cursor, Write};

use crate::handler::*;
use crate::raknet::bitstream::BitStream;
use crate::rage::*;

mod replacer;
mod server;

pub struct HashReplace {
    replacement: u64,
    server_started: bool,
}

impl HashReplace {
    pub fn new() -> HashReplace {
        HashReplace {
            replacement: 0,
            server_started: false,
        }
    }
}

impl Handler for HashReplace {
    fn handle_incoming(&mut self, packet_id: i32, bs: &mut BitStream) -> Option<Response> {
        if packet_id != ID_RPC {
            return None;
        }

        bs.read::<u16>().filter(|rpc_id| *rpc_id == RPC_CLIENT_PACKAGES)?;

        let mut out = BitStream::new();

        out.write(&(ID_RPC as u8));
        out.write(&RPC_CLIENT_PACKAGES);
        out.write(&self.replacement);

        bs.ignore_bytes(8);

        let bytes_to_read = if bs.read_bit() {
            bs.read::<u16>()?
        } else {
            2
        };

        let _host = bs.read_bytes(bytes_to_read as usize)?;
        let voice_sample_rate = bs.read::<u16>()?;

        out.write_bit(false);
        out.write(&22006u16);
        out.write(&voice_sample_rate);

        Some(Response::Change(out))
    }

    fn handle_outcoming(&mut self, _packet_id: i32, _bs: &mut BitStream) -> Option<Response> {
        None
    }
}

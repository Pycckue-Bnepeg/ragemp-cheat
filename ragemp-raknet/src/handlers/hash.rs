use std::hash::Hasher;
use std::io::{Cursor, Write};

use crate::handler::*;
use crate::rage::*;
use crate::raknet::bitstream::BitStream;

mod replacer;
mod server;

pub struct HashReplace {
    replacement: u64,
    server_started: bool,
}

impl HashReplace {
    pub fn new(replacement: u64) -> HashReplace {
        HashReplace {
            replacement,
            server_started: false,
        }
    }
}

impl Handler for HashReplace {
    fn handle_incoming(&mut self, packet_id: i32, bs: &mut BitStream) -> Option<Response> {
        if packet_id != ID_RPC {
            return None;
        }

        bs.read::<u16>()
            .filter(|rpc_id| *rpc_id == RPC_CLIENT_PACKAGES)?;

        if self.server_started {
            return None;
        }

        self.server_started = true;

        bs.print_hex();

        let mut out = BitStream::new();

        out.write(&(ID_RPC as u8));
        out.write(&RPC_CLIENT_PACKAGES);
        out.write(&self.replacement);

        bs.ignore_bytes(8);

        let bytes_to_read = if bs.read_bit() { bs.read::<u16>()? } else { 2 };

        let _host = bs.read_bytes(bytes_to_read as usize)?;
        let voice_sample_rate = bs.read::<u16>()?;

        println!(
            "{} {} {} {}",
            bytes_to_read,
            _host.len(),
            voice_sample_rate,
            bs.read_offset()
        );

        println!("{:?}", std::str::from_utf8(&_host));

        out.write_bit(false);
        out.write(&22006u16);
        out.write(&voice_sample_rate);
        out.align_write_to_byte_boundary();

        println!(
            "change hash (orig len {}, new len {} (could be {}))",
            bs.len_bits(),
            out.len_bits(),
            out.len_bits() + (2 << 3) + ((bytes_to_read as usize) << 3)
        );

        out.print_hex();

        Some(Response::Change(out))
        // None
    }

    fn handle_outcoming(&mut self, _packet_id: i32, _bs: &mut BitStream) -> Option<Response> {
        None
    }
}

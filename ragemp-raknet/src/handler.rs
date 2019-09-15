use crate::raknet::bitstream::BitStream;

pub trait Handler {
    fn handle_incoming(&mut self, packet_id: i32, bs: &mut BitStream) -> Option<Response>;
    fn handle_outcoming(&mut self, packet_id: i32, bs: &mut BitStream) -> Option<Response>;
}

pub enum Response {
    Change(BitStream<'static>),
    Forward,
    Break,
}

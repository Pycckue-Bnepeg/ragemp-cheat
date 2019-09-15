use super::bindings as sys;
use super::bitstream::BitStream;
use super::packet_priority::*;
use super::types::{ConnectionAttemptResult, Packet, PublicKey, SocketDescriptor, StartupResult};

use std::ffi::CString;
use std::net::SocketAddr;

pub struct RakPeerInterface(*mut sys::RakPeerInterface);

impl RakPeerInterface {
    pub fn new() -> RakPeerInterface {
        unsafe { RakPeerInterface(&mut *sys::RakPeerInterface::GetInstance()) }
    }

    pub fn startup(&self, max_connections: u32, socket: &SocketDescriptor) -> StartupResult {
        let result = (self.vtable().Startup)(
            self.0 as *mut _,
            max_connections,
            socket.as_raw(),
            1,
            -99999,
        );

        if result == 0 {
            return Ok(());
        } else {
            return Err(result.into());
        }
    }

    // todo: builder
    pub fn connect(
        &self,
        address: SocketAddr,
        password: Option<String>,
        public_key: &PublicKey,
    ) -> ConnectionAttemptResult {
        let c_host = CString::new(format!("{}", address.ip())).unwrap();
        let port = address.port();

        let (password_ptr, password_len) = match password.as_ref() {
            Some(password) => (
                password.as_bytes().as_ptr() as *const i8,
                password.as_bytes().len() as i32,
            ),
            None => (std::ptr::null(), 0),
        };

        let result = (self.vtable().Connect)(
            self.0 as *mut _,
            c_host.as_ptr(),
            port,
            password_ptr,
            password_len,
            public_key.as_raw(),
            0,
            12,
            500,
            0,
        );

        if result == 0 {
            return Ok(());
        } else {
            return Err(result.into());
        }
    }

    pub fn send_bitstream<T: Into<sys::AddressOrGUID>>(
        &self,
        bitstream: &BitStream,
        priority: PacketPriority,
        reliability: PacketReliability,
        channel: i8,
        address: T,
        broadcast: bool,
    ) {
        let bs = bitstream.inner();
        (self.vtable().SendBitstream)(
            self.0 as *mut _,
            bs,
            priority.into(),
            reliability.into(),
            channel,
            address.into(),
            broadcast,
            0,
        );
    }

    pub fn receive<'a>(&'a self) -> Option<Packet<'a>> {
        let packet = (self.vtable().Receive)(self.0 as *mut _);

        if packet.is_null() {
            None
        } else {
            Some(Packet::from_raw(packet, self))
        }
    }

    pub fn cancel_connection_attempt(&self, target: sys::SystemAddress) {
        (self.vtable().CancelConnectionAttempt)(self.0 as *mut _, target);
    }

    pub fn set_maximum_incoming_connections(&self, max: u16) {
        (self.vtable().SetMaximumIncomingConnections)(self.0 as *mut _, max);
    }

    pub fn connection_state<A: Into<sys::AddressOrGUID>>(&self, addr: A) -> sys::ConnectionState::Type {
        (self.vtable().GetConnectionState)(self.0 as *mut _, addr.into())
    }

    pub fn close_connection<T: Into<sys::AddressOrGUID>>(&self, target: T) {
        (self.vtable().CloseConnection)(self.0 as *mut _, target.into(), true, 0, PacketPriority::High.into());
    }

    pub(crate) fn deallocate_packet(&self, packet: *mut sys::Packet) {
        (self.vtable().DeallocatePacket)(self.0 as *mut _, packet);
    }

    #[inline(always)]
    fn vtable(&self) -> &sys::RakPeerInterface__bindgen_vtable {
        unsafe { &(*(*self.0).vtable_) }
    }
}

impl Drop for RakPeerInterface {
    fn drop(&mut self) {
        unsafe {
            sys::RakPeerInterface::DestroyInstance(self.0 as *mut _);
        }
    }
}

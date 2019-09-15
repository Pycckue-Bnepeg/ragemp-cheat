use super::bindings as sys;
use super::bitstream::BitStream;
use super::rakpeer::RakPeerInterface;

use std::ffi::CString;
use std::net::SocketAddr;

pub use sys::AddressOrGUID;
pub use sys::RakNetGUID;
pub use sys::SystemAddress;

const UNASSIGNED_RAKNET_GUID: RakNetGUID = RakNetGUID {
    g: std::u64::MAX,
    systemIndex: std::u16::MAX,
};

#[derive(Debug, Clone, Copy)]
pub enum StartupError {
    RakNetAlreadyStarted,
    InvalidSocketDescriptors,
    InvalidMaxConnections,
    SocketFamilyNotSupported,
    SocketPortAlreadyInUse,
    SocketFailedToBind,
    SocketFailedTestSend,
    PortCannotBeZero,
    FailedToCreateNetworkThread,
    CouldNotGenerateGuid,
    OtherFailure,
}

#[derive(Debug, Clone, Copy)]
pub enum ConnectionAttemptError {
    InvalidParameter,
    CannotResolveDomainName,
    AlreadyConnectedToEndpoint,
    ConnectionAttemptAlreadyInProgress,
    SecurityInitializationFailed,
}

pub type StartupResult = Result<(), StartupError>;
pub type ConnectionAttemptResult = Result<(), ConnectionAttemptError>;

impl From<sys::StartupResult::Type> for StartupError {
    fn from(value: sys::StartupResult::Type) -> StartupError {
        match value {
            1 => StartupError::RakNetAlreadyStarted,
            2 => StartupError::InvalidSocketDescriptors,
            3 => StartupError::InvalidMaxConnections,
            4 => StartupError::SocketFamilyNotSupported,
            5 => StartupError::SocketPortAlreadyInUse,
            6 => StartupError::SocketFailedToBind,
            7 => StartupError::SocketFailedTestSend,
            8 => StartupError::PortCannotBeZero,
            9 => StartupError::FailedToCreateNetworkThread,
            10 => StartupError::CouldNotGenerateGuid,
            _ => StartupError::OtherFailure,
        }
    }
}

impl From<sys::ConnectionAttemptResult::Type> for ConnectionAttemptError {
    fn from(value: sys::ConnectionAttemptResult::Type) -> ConnectionAttemptError {
        match value {
            1 => ConnectionAttemptError::InvalidParameter,
            2 => ConnectionAttemptError::CannotResolveDomainName,
            3 => ConnectionAttemptError::AlreadyConnectedToEndpoint,
            4 => ConnectionAttemptError::ConnectionAttemptAlreadyInProgress,
            _ => ConnectionAttemptError::SecurityInitializationFailed,
        }
    }
}

#[derive(Debug)]
pub struct SocketDescriptor {
    raw_desc: sys::SocketDescriptor,
    addr: SocketAddr,
}

impl SocketDescriptor {
    pub fn new(addr: SocketAddr) -> SocketDescriptor {
        let c_host = CString::new(format!("{}", addr.ip())).unwrap();

        unsafe {
            SocketDescriptor {
                raw_desc: sys::SocketDescriptor::new1(addr.port(), c_host.as_ptr()),
                addr,
            }
        }
    }

    pub fn set_port(&mut self, port: u16) {
        self.addr.set_port(port);
        self.raw_desc.port = port;
    }

    pub fn addr(&self) -> SocketAddr {
        self.addr
    }

    pub(crate) fn as_raw(&self) -> *mut sys::SocketDescriptor {
        &self.raw_desc as *const _ as *mut _
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd)]
pub enum PublicKeyMode {
    InsecureConnection,
    AcceptAnyPublicKey,
    UseKnownPublicKey,
    UseTwoWayAuthentication,
}

impl Into<sys::PublicKeyMode::Type> for PublicKeyMode {
    fn into(self) -> sys::PublicKeyMode::Type {
        match self {
            PublicKeyMode::InsecureConnection => 0,
            PublicKeyMode::AcceptAnyPublicKey => 1,
            PublicKeyMode::UseKnownPublicKey => 2,
            PublicKeyMode::UseTwoWayAuthentication => 3,
        }
    }
}

pub struct PublicKey(sys::PublicKey);

impl PublicKey {
    pub fn new(mode: PublicKeyMode) -> PublicKey {
        assert!(mode < PublicKeyMode::UseKnownPublicKey);

        let pk = sys::PublicKey {
            publicKeyMode: mode.into(),
            remoteServerPublicKey: std::ptr::null_mut(),
            myPublicKey: std::ptr::null_mut(),
            myPrivateKey: std::ptr::null_mut(),
        };

        PublicKey(pk)
    }

    pub fn set_mode(&mut self, mode: PublicKeyMode) {
        self.0.publicKeyMode = mode.into();
    }

    pub(crate) fn as_raw(&self) -> *mut sys::PublicKey {
        &self.0 as *const _ as *mut _
    }
}

pub struct Packet<'a> {
    packet: *mut sys::Packet,
    rakpeer: &'a RakPeerInterface,
}

impl<'a> Packet<'a> {
    pub(crate) fn from_raw(packet: *mut sys::Packet, rakpeer: &'a RakPeerInterface) -> Packet<'a> {
        Packet { packet, rakpeer }
    }

    pub fn len(&self) -> usize {
        unsafe { (*self.packet).length as usize }
    }

    pub fn address(&self) -> SocketAddr {
        unsafe { (*self.packet).systemAddress.into() }
    }

    pub fn as_bitstream(&'a self) -> BitStream<'a> {
        let slice = unsafe {
            let packet = &mut (*self.packet);
            std::slice::from_raw_parts_mut(packet.data, packet.length as usize)
        };

        BitStream::from(slice)
    }
}

impl Drop for Packet<'_> {
    fn drop(&mut self) {
        self.rakpeer.deallocate_packet(self.packet);
    }
}

impl From<SystemAddress> for AddressOrGUID {
    fn from(addr: SystemAddress) -> AddressOrGUID {
        AddressOrGUID {
            systemAddress: addr,
            rakNetGuid: UNASSIGNED_RAKNET_GUID,
        }
    }
}

impl From<RakNetGUID> for AddressOrGUID {
    fn from(guid: RakNetGUID) -> AddressOrGUID {
        AddressOrGUID {
            systemAddress: unsafe { SystemAddress::new() },
            rakNetGuid: guid,
        }
    }
}

impl From<SocketAddr> for SystemAddress {
    fn from(addr: SocketAddr) -> SystemAddress {
        unsafe {
            let c_host = CString::new(format!("{}", addr.ip())).unwrap();
            let port = addr.port();

            SystemAddress::new2(c_host.as_ptr() as *const i8, port)
        }
    }
}

impl From<SocketAddr> for AddressOrGUID {
    fn from(addr: SocketAddr) -> AddressOrGUID {
        let addr = SystemAddress::from(addr);
        addr.into()
    }
}

impl From<SystemAddress> for SocketAddr {
    fn from(addr: SystemAddress) -> SocketAddr {
        unsafe {
            let bytes = addr.address.addr4.as_ref().sin_addr.S_un.S_un_b.as_ref();
            SocketAddr::from(([bytes.s_b1, bytes.s_b2, bytes.s_b3, bytes.s_b4], addr.GetPort()))
        }
    }
}

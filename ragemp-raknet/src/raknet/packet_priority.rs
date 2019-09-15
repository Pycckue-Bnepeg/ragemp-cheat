#[derive(Debug, Clone, Copy)]
pub enum PacketPriority {
    Immediate,
    High,
    Medium,
    Low,
}

impl From<i32> for PacketPriority {
    fn from(priority: i32) -> PacketPriority {
        match priority {
            0 => PacketPriority::Immediate,
            1 => PacketPriority::High,
            2 => PacketPriority::Medium,
            _ => PacketPriority::Low,
        }
    }
}

impl Into<i32> for PacketPriority {
    fn into(self) -> i32 {
        match self {
            PacketPriority::Immediate => 0,
            PacketPriority::High => 1,
            PacketPriority::Medium => 2,
            PacketPriority::Low => 3,
        }
    }
}

#[derive(Debug, Clone, Copy)]
pub enum PacketReliability {
    Unreliable,
    UnreliableSequenced,
    Reliable,
    ReliableOrdered,
    ReliableSequenced,
    UnreliableWithAckReceipt,
    ReliableWithAckReceipt,
    ReliableOrderedWithAckReceipt,
}

impl From<i32> for PacketReliability {
    fn from(priority: i32) -> PacketReliability {
        match priority {
            0 => PacketReliability::Unreliable,
            1 => PacketReliability::UnreliableSequenced,
            2 => PacketReliability::Reliable,
            3 => PacketReliability::ReliableOrdered,
            4 => PacketReliability::ReliableSequenced,
            5 => PacketReliability::UnreliableWithAckReceipt,
            6 => PacketReliability::ReliableWithAckReceipt,
            _ => PacketReliability::ReliableOrderedWithAckReceipt,
        }
    }
}

impl Into<i32> for PacketReliability {
    fn into(self) -> i32 {
        match self {
            PacketReliability::Unreliable => 0,
            PacketReliability::UnreliableSequenced => 1,
            PacketReliability::Reliable => 2,
            PacketReliability::ReliableOrdered => 3,
            PacketReliability::ReliableSequenced => 4,
            PacketReliability::UnreliableWithAckReceipt => 5,
            PacketReliability::ReliableWithAckReceipt => 6,
            PacketReliability::ReliableOrderedWithAckReceipt => 7,
        }
    }
}

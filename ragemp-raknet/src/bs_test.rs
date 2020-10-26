use crate::rage::*;
use crate::raknet::bitstream::BitStream;

pub fn test() -> Option<()> {
    let bytes = [
        0xd2u8, 0x31, 0x00, 0xb3, 0xed, 0x95, 0x31, 0x37, 0xe1, 0xd9, 0x1d, 0x8b, 0x80, 0x33, 0xb9,
        0x30, 0xb7, 0x32, 0x16, 0xb2, 0x37, 0xbb, 0xb7, 0x36, 0x37, 0xb0, 0xb2, 0x17, 0x39, 0xb9,
        0x3b, 0x1a, 0x17, 0x31, 0x34, 0xbd, 0x40, 0x5d, 0x80,
    ];

    let mut out = BitStream::new();
    let mut bs = BitStream::new();
    bs.write_bytes(&bytes);
    bs.reset_read_pointer();

    bs.ignore_bytes(3); // ID + RPC_ID
    bs.ignore_bytes(8); // hash

    let bytes_to_read = if bs.read_bit() { bs.read::<u16>()? } else { 2 };

    let _host = bs.read_bytes(bytes_to_read as usize)?;
    let voice_sample_rate = bs.read::<u16>()?;

    println!("bytes_to_read (2 bytes) {}", bytes_to_read);
    println!("host ({} bytes) {:?}", bytes_to_read, unsafe {
        std::str::from_utf8_unchecked(&_host)
    });
    println!("voice_sample_rate (2 bytes) {}", voice_sample_rate);

    println!(
        "bits count {}. read offset {}",
        bs.len_bits(),
        bs.read_offset_bits()
    );

    println!("origin bytes");
    bs.print_hex();

    out.write(&(ID_RPC as u8));
    out.write(&RPC_CLIENT_PACKAGES);
    out.write(&0xfe9fbf3fc5506e5bu64);
    out.write_bit(false);
    out.write(&22006u16);
    out.write(&voice_sample_rate);
    out.align_write_to_byte_boundary();

    println!("modified bytes");
    out.print_hex();

    println!(
        "origin length {}. modified length {}. could be {}",
        bs.len_bits(),
        out.len_bits(),
        out.len_bits() + ((bytes_to_read as usize) << 3)
    );

    Some(())
}
